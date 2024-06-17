#include <iostream>
#include <queue>
#include "./BufferManager.cpp"
using namespace std;

struct Requerimiento {
    int numPagina;
    char accion;
};

queue<Requerimiento> colaDeRequerimientos;
queue<Requerimiento> colaDeEspera;

/* Funcion para ver REQUERIMIENTOS incial*/

void visualizarCola() {
    queue<Requerimiento> tempCola = colaDeRequerimientos; // Copiar la cola para no modificar la original
    cout << "Cola de Requerimientos:" << endl;
    if (tempCola.empty()) {
        cout << "La cola está vacía." << endl;
        return;
    }
    while (!tempCola.empty()) {
        Requerimiento req = tempCola.front();
        tempCola.pop();
        cout << "Pagina: " << req.numPagina << ", Accion: " << (req.accion == 'L' || req.accion == 'l' ? "Leer" : "Escribir") << endl;
    }
}

/*
void visualizarCola() {
    queue<Requerimiento> tempCola = colaDeRequerimientos; // Copiar la cola para no modificar la original
    queue<Requerimiento> tempColaEspera = colaDeEspera; // Copiar la cola de espera para no modificar la original
    
    cout << "Cola de Requerimientos:" << endl;
    if (tempCola.empty()) {
        cout << "La cola está vacía." << endl;
        return;
    }
    
    while (!tempCola.empty()) {
        Requerimiento req = tempCola.front();
        tempCola.pop();

        // Chequear si el requerimiento está en la cola de espera
        bool enEspera = false;
        queue<Requerimiento> tempEspera = tempColaEspera; // Copia temporal de la cola de espera para buscar el requerimiento
        while (!tempEspera.empty()) {
            Requerimiento reqEspera = tempEspera.front();
            tempEspera.pop();
            if (req.numPagina == reqEspera.numPagina && req.accion == reqEspera.accion) {
                enEspera = true;
                break;
            }
        }

        cout << "Pagina: " << req.numPagina << ", Accion: " << (req.accion == 'L' ? "Leer" : "Escribir");
        if (enEspera) {
            cout << " (Cola de Espera)";
        }
        cout << endl;
    }
}
*/

void menuBufferManager(BufferManager &bufferManagerPrincipal) {
    cout << "Bienvenido a implementación de BUFFER" << endl;
    int pesoBytesBLoque;
    cout << "Ingrese el peso de su bloque de Disco: " << endl;
    cin >> pesoBytesBLoque;
    int numFrames;
    cout << "Ingrese la cantidad de Frames a tener en el Buffer Pool: " << endl;
    cin >> numFrames;
    
    bufferManagerPrincipal.crearBufferPoolSegunNumFrames(numFrames);
    bufferManagerPrincipal.establecerLimiteDeFrames(pesoBytesBLoque);
    
    cout << "Creación de estructura Básica de BufferManager implementado." << endl;
}

void menu() {
    BufferManager bufferManagerPrincipal;
    int opcion = 0;
    int numPagina;
    int numFrame;
    int opcionPaginaGuardado;

    while (opcion != 8) { // Cambiado a 8 para incluir la nueva opción
        cout << endl;
        cout << "------- Menú Principal Programa Buffer Manager -------" << endl;
        cout << "1. Crear Buffer Pool" << endl;
        cout << "2. Obtener Una Página" << endl;
        cout << "3. Mostrar Page Table" << endl;
        cout << "4. Liberar uso de Página" << endl;
        cout << "5. Unpinnear Una Página" << endl;
        cout << "6. Visualizar Cola de Requerimientos" << endl;
        cout << "7. Salir del programa principal" << endl;
        cout << "Ingrese el numero de opcion" << endl;
        cin >> opcion;

        switch (opcion) {
            case 1: {
                menuBufferManager(bufferManagerPrincipal);
                break;
            }
            case 2: {
                cout << "Ingrese el numero de pagina que desea OBTENER (Modo Prueba 1 - 6): " << endl;
                cin >> numPagina;
                bufferManagerPrincipal.obtenerUnaPagina(numPagina);
                
                char accionPagina;
                cout << "Leer (L/l) o Escribir (W/w) en Pagina?: ";
                cin >> accionPagina;

                //bool requerimientoAgregado = false;

                Requerimiento req = {numPagina, accionPagina};
                colaDeRequerimientos.push(req);
                cout << "Requerimiento agregado a la cola." << endl;

                numFrame = bufferManagerPrincipal.pageTable.getNumFrameDeUnaPagina(numPagina);

                if (accionPagina == 'L' || accionPagina == 'l') {
                    cout << "Proceso de Lectura." << endl;
                    if (bufferManagerPrincipal.pageTable.verificarDirtyPagina(numPagina) == true) {
                        bufferManagerPrincipal.bufferPool.mostrarFramePagina(numFrame);
                        bufferManagerPrincipal.pageTable.aumentarPinCountDePagina(numPagina);
                    }
                    else {
                        cout << "Dirty Bit de la Pagina " << numPagina << " esta en 1." << endl;
                        cout << "No se puede procesar el requerimiento de Lectura hasta que Dirty Bit sea 0." << endl;
                        cout << "Si se quiere procesar el requerimiento de Lectura, debe guardar el archivo." << endl;
                        cout << "Guardar contenido? 0/NO GUARDAR  1/GUARDAR: ";
                        cin >> opcionPaginaGuardado;

                        if (opcionPaginaGuardado == 0) {
                            cout << "Requerimiento a cola de espera." << endl;
                            Requerimiento colaEspera = {numPagina, accionPagina};
                            colaDeEspera.push(colaEspera);
                            //requerimientoAgregado = true;
                        }
                        else if (opcionPaginaGuardado == 1) {
                            bufferManagerPrincipal.pageTable.cambiarDirtyBitDePagina(numPagina);
                            bufferManagerPrincipal.bufferPool.agregarContenidoPaginaAbloque(numFrame, numPagina);
                            bufferManagerPrincipal.pageTable.aumentarPinCountDePagina(numPagina);
                        }
                    }
                    break;
                }
                else if (accionPagina == 'W' || accionPagina == 'w') {
                    bufferManagerPrincipal.bufferPool.lecturaOescrituraPagina(numFrame); //escribir
                    bufferManagerPrincipal.pageTable.aumentarPinCountDePagina(numPagina);
                    bufferManagerPrincipal.pageTable.cambiarDirtyBitDePagina(numPagina);
                    break;
                }

                /*
                if (!requerimientoAgregado) {
                    Requerimiento req = {numPagina, accionPagina};
                    colaDeRequerimientos.push(req);
                    cout << "Requerimiento agregado a la cola." << endl;
                }
                */

                //Requerimiento req = {numPagina, accionPagina};
                //colaDeRequerimientos.push(req);
                //cout << "Requerimiento agregado a la cola." << endl;
                break;
            }
            case 3: {
                bufferManagerPrincipal.mostrarPageTAble();
                cout << endl;
                break;
            }
            case 4: {
                cout << "Ingrese el numero de la pagina a liberar (descontar PinCount)" << endl;
                cin >> numPagina;

                if (bufferManagerPrincipal.pageTable.verificarDirtyPagina(numPagina) == false) {
                    cout << "Dirty = 1 " << endl;
                    cout << "Para continuar, desea guardar el contenido? 0/NO GUARDAR  1/GUARDAR" << endl;
                    int opcionPaginaGuardado;
                    cin >> opcionPaginaGuardado;

                    if (opcionPaginaGuardado == 0) {
                        cout << "Página no guardada, no se descuenta pin count." << endl;
                    } else if (opcionPaginaGuardado == 1) {
                        bufferManagerPrincipal.pageTable.cambiarDirtyBitDePagina(numPagina);
                        bufferManagerPrincipal.bufferPool.agregarContenidoPaginaAbloque(numFrame, numPagina);
                        bufferManagerPrincipal.pageTable.descontarPinCountApagina(numPagina);
                    } else {
                        cout << "Opción inválida" << endl;
                    }
                } else {
                    bufferManagerPrincipal.pageTable.descontarPinCountApagina(numPagina);
                }
                break;
            }
            case 5: {
                cout << "Ingrese Pagina a Despinnear " << endl;
                cin >> numPagina;
                bufferManagerPrincipal.pageTable.unpinnearPagina(numPagina);
                break;
            }
            case 6: {
                visualizarCola();
                break;
            }
            case 7: {
                cout << "Saliendo del programa..." << endl;
                break;
            }
            default:
                cout << "Opción inválida, por favor intente de nuevo." << endl;
                break;
        }
    }
}

int main() {
    menu();
    return 0;
}