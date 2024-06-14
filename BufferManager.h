#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include "./BufferPool.cpp"
#include "./PageTable.cpp"
#include "./Pagina.h"
using namespace std;

class BufferManager {
    public:
        BufferPool bufferPool;
        PageTable pageTable;
        Pagina pagina;
    public:
        BufferManager();
        ~BufferManager();
        void crearBufferPoolSegunNumFrames(int numFrames);
        void establecerLimiteDeFrames(int pesoBytesBLoque);
        void obtenerUnaPagina(int numPagina);
        void mostrarUnaPagina(int numPagina);
        void mostrarPageTAble();
        void dejarDeUsarUnaPagina(int numPagina);
        void liberarPinnedPagina(int numPagina);
        void verificarDirtyPagina(int numPagina);
        void agregarContenido(int numPaginaEliminada);
};

#endif