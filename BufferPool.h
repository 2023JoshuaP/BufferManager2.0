#ifndef BUFFERPOOL_H
#define BUFFERPOOL_H

#include <vector>
#include <iostream>
#include "./Frame.cpp"
#include "./PageTable.h"
using namespace std;

class BufferPool {
    public:
        int capacidadDeCadaFrame;
        vector<Frame> vectorFramesBufferPool;
        PageTable pageTable;
    public:
        BufferPool();
        ~BufferPool();
        void agregarNuevaPaginaBufferPool(int numFrame, int numPagina);
        void mostrarFramePagina(int numPagina);
        void lecturaOescrituraPagina(int numPagina);
        void agregarContenidoPaginaAbloque(int numPagina,int numPaginaEliminada);
};

#endif