#include "funcionesCuda.h"

void inicializar(EntradaCuerpo* entradaCuerpo, CONTEXTO contexto)
{
    entradaCuerpo->contexto = contexto;

    entradaCuerpo->exT  = (double **)generarMatriz(sizeof(double), 5000, 3, contexto);
    entradaCuerpo->conT = (int **)generarMatriz(sizeof(int), 5000, 3, contexto);
    entradaCuerpo->ndT  = (double **)generarMatriz(sizeof(double), 5000, 3, contexto);
    entradaCuerpo->locT = (double **)generarMatriz(sizeof(double), 5000, 9, contexto);

    entradaCuerpo->AE   = (double **)generarMatriz(sizeof(double), entradaCuerpo->nelT * 3, entradaCuerpo->nelT * 3, contexto);
    entradaCuerpo->BE   = (double **)generarMatriz(sizeof(double), entradaCuerpo->nelT * 3, entradaCuerpo->nelT * 3, contexto);
    entradaCuerpo->AT   = (double **)generarMatriz(sizeof(double), entradaCuerpo->nelT, entradaCuerpo->nelT, contexto);
    entradaCuerpo->BT   = (double **)generarMatriz(sizeof(double), entradaCuerpo->nelT, entradaCuerpo->nelT, contexto);
    entradaCuerpo->CTE  = (double **)generarMatriz(sizeof(double), entradaCuerpo->nelT * 3, entradaCuerpo->nelT, contexto);
    entradaCuerpo->DTE  = (double **)generarMatriz(sizeof(double), entradaCuerpo->nelT * 3, entradaCuerpo->nelT, contexto);
}

void** generarMatriz(int tamanioElemento, int filas, int columnas, CONTEXTO contexto)
{
    void** variable;

    switch (contexto)
    {
        case HOST:
            variable = (void **)alloc2DOnHost(filas, columnas, tamanioElemento);
            break;

        case CUDA:
            variable = (void **)alloc2DOnDevice(filas, columnas, tamanioElemento);
            break;
    }

    return variable;
}

void** alloc2DOnHost(int rows, int columns, int sizeOfElement)
{
    int header = rows * sizeof(void*);
    int body = rows * columns * sizeOfElement;

    void** rowptr = (void**)malloc(header + body);
    char* values = (char*)(rowptr + rows);

    for (int i=0; i< rows*columns * sizeOfElement; i++)
        values[i] = 0;

    for (int i=0; i<rows; i++)
        rowptr[i] = (void*) &values[i*columns*sizeOfElement];

    return rowptr;
}

void ** alloc2DOnDevice(int rows, int cols, int sizeOfElement)
{
    int header = rows * sizeof(void*);
    int body = rows * cols * sizeOfElement;

    void** rowptr;

    cudaMalloc(&rowptr, header+body);

    __organize2DMatrix<<<1,1>>>(rowptr, rows, cols, sizeOfElement);

    cudaDeviceSynchronize();

    return rowptr;
}

__global__ void __organize2DMatrix(void** matrix, int rows, int cols, int sizeOfElement)
{
    char* values = (char*)(matrix + rows);

    for (int i=0; i<rows; i++)
        matrix[i] = (void*) &values[i*cols*sizeOfElement];
}

void copiarMatriz(void** matrizDestino, void** matrizOrigen, int tamanioElemento, int filas, int columnas,
                  int direccion)
{
    switch (direccion)
    {
        case MEMCPY_HOST_TO_HOST:
            memcpy(matrizDestino+filas, matrizOrigen+filas,
                   filas*columnas*tamanioElemento);
            break;
        case cudaMemcpyDeviceToHost:
            cudaMemcpy(matrizDestino+filas, matrizOrigen+filas,
                       filas*columnas*tamanioElemento, cudaMemcpyDeviceToHost);
            break;
        case cudaMemcpyHostToDevice:
            cudaMemcpy(matrizDestino+filas, matrizOrigen+filas,
                       filas*columnas*tamanioElemento, cudaMemcpyHostToDevice);
            break;
    }
}

void liberarMatriz(void** matriz, CONTEXTO contexto)
{
    switch (contexto)
    {
        case HOST:
            free(matriz);
            break;

        case CUDA:
            cudaFree(matriz);
            break;
    }
}

void finalizar(EntradaCuerpo* entradaCuerpo)
{
    liberarMatriz((void**)entradaCuerpo->exT, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->conT, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->ndT, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->locT, entradaCuerpo->contexto);

    liberarMatriz((void**)entradaCuerpo->AE, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->BE, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->AT, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->BT, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->CTE, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->DTE, entradaCuerpo->contexto);
}
