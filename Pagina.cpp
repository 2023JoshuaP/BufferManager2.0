#include "./Pagina.h"

Pagina::Pagina() {}

Pagina::~Pagina(){}

int tamanioString = 0;
string rutaCompleta = "";

bool isNumber(const std::string &s) {
    for (char const &ch : s) {
        if (std::isdigit(ch) == 0 && ch != '.')
            return false;
    }
    return true;
}

void Pagina::recogerRegistros(int numPagina) {
    string numPaginaString = to_string(numPagina);
    string nameBloque = "Bloque";
    rutaCompleta = "../Bloques/" + nameBloque + numPaginaString + ".txt";
    ifstream fileReadBloques(rutaCompleta);
    
    if (!fileReadBloques.is_open()) {
        cerr << "Error, no se pudo abrir el archivo: " << rutaCompleta << endl;
        return;
    }
    
    cout << "Exito, si se pudo abrir el archivo." << endl;
    
    cout << "Longitud de string: ";
    int tamanioString;

    while (!(cin >> tamanioString) || tamanioString <= 0 ) {
        cout << "Por favor, introduce un número válido mayor que 0: ";
        cin.clear();
        cin.ignore();
    }

    string lineaDeRegistroAux;
    while (getline(fileReadBloques, lineaDeRegistroAux)) {
        if (lineaDeRegistroAux.empty()) {
            cout << "Línea vacía, omitiendo." << endl;
            continue;
        }
        
        cout << "Procesando línea: " << lineaDeRegistroAux << endl;
        
        string linea_registro;
        stringstream ss(lineaDeRegistroAux);
        string valor;
        bool primerValor = true;
        
        while (getline(ss, valor, ',')) {
            if (!primerValor) {
                linea_registro += ',';
            }
            else {
                primerValor = false;
            }
            int peso = valor.size();
            
            if (isNumber(valor)) {
                linea_registro += valor;
            } else {
                while (peso < tamanioString) {
                    valor += ' ';
                    peso += 1;
                }
                linea_registro += valor;
            }
        }
        
        this->vectorRegistrosEnPagina.push_back(linea_registro);
    }
    
    cout << "Recogida de datos terminado." << endl;
}


void Pagina::mostrarContenidoDePagina() {
    
    int columnaDirtyBit = 1;

    for (size_t i =0 ; i < this->vectorRegistrosEnPagina.size(); i++) {
        cout << this->vectorRegistrosEnPagina[i] << endl;
    }
    cout << "------------------------------------------------------------------------" << endl;
    
}

void Pagina::leerOescribirEnPagina() {
    int filaAencontrar;
    string registroNuevo;
    string cadenaFinal = "";
    cout << "Fila a editar: ";
    cin >> filaAencontrar;
    cin.ignore();

    string& registro = this->vectorRegistrosEnPagina[filaAencontrar];
    cout << "Registro actual: " << registro << endl;

    vector<string> valoresRegistroNuevo;
    stringstream ss(registro);
    string valor;

    cout << "Registro nuevo: ";

    getline(cin, registroNuevo);

    stringstream ssNuevo(registroNuevo);

    while (getline(ssNuevo, valor, ',')) {
        valoresRegistroNuevo.push_back(valor);
    }

    for (size_t i = 0; i < valoresRegistroNuevo.size();++i) {
        if (isNumber(valoresRegistroNuevo[i]) == true) {
            cadenaFinal += valoresRegistroNuevo[i] + ',';
        }
        if (isNumber(valoresRegistroNuevo[i]) == false) {
            cadenaFinal += valoresRegistroNuevo[i];
            int pesoValor = valoresRegistroNuevo[i].size();
            
            do {
                cadenaFinal += " ";
                pesoValor += 1;
            } while (pesoValor < tamanioString);
            
            if (i < valoresRegistroNuevo.size() - 1) {
                cadenaFinal += ',';
            }
        }
    }
    this->vectorRegistrosEnPagina[filaAencontrar] = cadenaFinal;
    cout << "Guardado" << endl;

    cout << "------------------------------------------------------------------------" << endl;
    
}

bool Pagina::verificarPaginaVacia() {
    if (this->vectorRegistrosEnPagina.empty()) {
        return true;
    }
    else {
        return false;
    }    
}

void Pagina::agregarContenido(int numPaginaEliminada) {
    string rutaBloqueEscribir = "./Bloques/Bloque"+ to_string(numPaginaEliminada) + ".txt";
    ofstream rutaBloque(rutaBloqueEscribir, ios::trunc);
    if (!rutaBloque.is_open()) {
        cout<<"No abrio correctamente: "<<rutaBloqueEscribir<<endl;
    }
    else {
        for (int_least32_t i = 0; i <= this->vectorRegistrosEnPagina.size(); ++i) {
            rutaBloque << this->vectorRegistrosEnPagina[i] << endl;
        }
        cout << "Contenido de pagina en memoria mandado a bloque correctamente" << endl;
    }
}

