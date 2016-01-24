#include "funcionesCuda.h"

void inicializar(EntradaCuerpo* entradaCuerpo, CONTEXTO contexto)
{
    entradaCuerpo->contexto = contexto;

    entradaCuerpo->exT  = (double **)generarMatriz(sizeof(double), 5000, 3, contexto);
    entradaCuerpo->conT = (int **)generarMatriz(sizeof(int), 5000, 3, contexto);
    entradaCuerpo->ndT  = (double **)generarMatriz(sizeof(double), 5000, 3, contexto);
    entradaCuerpo->locT = (double **)generarMatriz(sizeof(double), 5000, 9, contexto);

    entradaCuerpo->AE_T   = (double **)generarMatriz(sizeof(double), entradaCuerpo->nelT * 3, entradaCuerpo->nelT * 3, contexto);
    entradaCuerpo->BE_T   = (double **)generarMatriz(sizeof(double), entradaCuerpo->nelT * 3, entradaCuerpo->nelT * 3, contexto);
    entradaCuerpo->AT_T   = (double **)generarMatriz(sizeof(double), entradaCuerpo->nelT, entradaCuerpo->nelT, contexto);
    entradaCuerpo->BT_T   = (double **)generarMatriz(sizeof(double), entradaCuerpo->nelT, entradaCuerpo->nelT, contexto);
    entradaCuerpo->CTE_T  = (double **)generarMatriz(sizeof(double), entradaCuerpo->nelT * 3, entradaCuerpo->nelT, contexto);
    entradaCuerpo->DTE_T  = (double **)generarMatriz(sizeof(double), entradaCuerpo->nelT * 3, entradaCuerpo->nelT, contexto);

    entradaCuerpo->AE     = (double **)generarMatriz(sizeof(double), 3, 3, contexto);
    entradaCuerpo->BE     = (double **)generarMatriz(sizeof(double), 3, 3, contexto);
    entradaCuerpo->CTE    = (double *)generarVector(sizeof(double), 3, contexto);
    entradaCuerpo->DTTE   = (double *)generarVector(sizeof(double), 3, contexto);
    entradaCuerpo->DTE    = (double **)generarMatriz(sizeof(double), 3, 3, contexto);

    entradaCuerpo->ndCol  = (double *)generarVector(sizeof(double), 3, contexto);
    entradaCuerpo->extr   = (double **)generarMatriz(sizeof(double), 4, 3, contexto);

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

void* generarVector(int tamanioElemento, int longitud, CONTEXTO contexto)
{
    void* variable;

    switch (contexto)
    {
        case HOST:
            variable = (void*)malloc(tamanioElemento * longitud);
            break;

        case CUDA:
            cudaMalloc(&variable, tamanioElemento * longitud);
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

void copiarVector(void* vectorDestino, void* vectorOrigen, int tamanioElemento, int longitud, int direccion)
{
    switch (direccion)
    {
        case MEMCPY_HOST_TO_HOST:
            memcpy(vectorDestino, vectorOrigen,
                   longitud*tamanioElemento);
            break;
        case cudaMemcpyDeviceToHost:
            cudaMemcpy(vectorDestino, vectorOrigen,
                       longitud*tamanioElemento, cudaMemcpyDeviceToHost);
            break;
        case cudaMemcpyHostToDevice:
            cudaMemcpy(vectorDestino, vectorOrigen,
                       longitud*tamanioElemento, cudaMemcpyHostToDevice);
            break;
    }
}

void liberarVector(void* vector, CONTEXTO contexto)
{
    switch (contexto)
    {
        case HOST:
            free(vector);
            break;

        case CUDA:
            cudaFree(vector);
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

void copiarEstructura(EntradaCuerpo* destino, EntradaCuerpo* origen, int direccion)
{
    destino->nexT = origen->nexT;
    destino->nelT = origen->nelT;
    destino->ET = origen->ET;
    destino->alT = origen->alT;
    destino->nuT = origen->nuT;
    destino->GT = origen->GT;

    destino->simXY = origen->simXY;// Flag de simetría respecto del plano xOy
    destino->simXZ = origen->simXZ;// Flag de simetría respecto del plano xOz
    destino->simYZ = origen->simYZ;// Flag de simetría respecto del plano yOz
    destino->tpproE = origen->tpproE;// Flag de tipo de problema elastico
    destino->tpproT = origen->tpproT;// Flag de tipo de problema termico
    destino->tpproTE = origen->tpproTE;// Flag de tipo de problema termoelastico
    destino->tpcarFP = origen->tpcarFP;// Flag de tipo de carga térmica. Fuentes puntuales
    destino->tpcarFL = origen->tpcarFL;// Flag de tipo de carga térmica. Fuentes lineales
    destino->tpcarFD = origen->tpcarFD;// Flag de tipo de carga térmica. Fuentes distribuidas
    destino->tpcarFC = origen->tpcarFC;// Flag de tipo de carga elástica. Fuerza centrífuga
    destino->tpcarPP = origen->tpcarPP;// Flag de tipo de carga elástica. Peso propio

    destino->cte1 = origen->cte1;// Constante en la integracion elastica
    destino->cte2 = origen->cte2;// Constante en la integracion elastica
    destino->cte3 = origen->cte3;// Constante en la integracion elastica
    destino->cte4 = origen->cte4;// Constante en la integracion termica
    destino->cte5 = origen->cte5;// Constante en la integracion termoelastica

    destino->reg = origen->reg;

    destino->enExcepcion = origen->enExcepcion;

    copiarMatriz((void**)destino->exT, (void**)origen->exT, sizeof(double), 5000, 3, direccion);
    copiarMatriz((void**)destino->conT, (void**)origen->conT, sizeof(int), 5000, 3, direccion);
    copiarMatriz((void**)destino->ndT, (void**)origen->ndT, sizeof(double), 5000, 3, direccion);
    copiarMatriz((void**)destino->locT, (void**)origen->locT, sizeof(double), 5000, 9, direccion);

    copiarMatriz((void**)destino->AE_T, (void**)origen->AE_T, sizeof(double), origen->nelT * 3, origen->nelT * 3, direccion);
    copiarMatriz((void**)destino->BE_T, (void**)origen->BE_T, sizeof(double), origen->nelT * 3, origen->nelT * 3, direccion);
    copiarMatriz((void**)destino->AT_T, (void**)origen->AT_T, sizeof(double), origen->nelT, origen->nelT, direccion);
    copiarMatriz((void**)destino->BE_T, (void**)origen->BE_T, sizeof(double), origen->nelT, origen->nelT, direccion);
    copiarMatriz((void**)destino->CTE_T, (void**)origen->CTE_T, sizeof(double), origen->nelT * 3, origen->nelT, direccion);
    copiarMatriz((void**)destino->DTE_T, (void**)origen->DTE_T, sizeof(double), origen->nelT * 3, origen->nelT, direccion);

    copiarMatriz((void**)destino->AE, (void**)origen->AE, sizeof(double), 3, 3, direccion);
    copiarMatriz((void**)destino->BE, (void**)origen->BE, sizeof(double), 3, 3, direccion);
    copiarVector((void*)destino->CTE, (void*)origen->CTE, sizeof(double), 3, direccion);
    copiarVector((void*)destino->DTTE, (void*)origen->DTTE, sizeof(double), 3, direccion);
    copiarMatriz((void**)destino->DTE, (void**)origen->DTE, sizeof(double), 3, 3, direccion);

    copiarVector((void*)destino->ndCol, (void*)origen->ndCol, sizeof(double), 3, direccion);
    destino->intenum = origen->intenum;
    destino->tipoSimetria = origen->tipoSimetria;
    copiarMatriz((void**)destino->extr, (void**)origen->extr, sizeof(double), 4, 3, direccion);

}


EntradaCuerpo* pasarADispositivo(EntradaCuerpo* entradaCuerpo)
{
    EntradaCuerpo* resultado;
    cudaMalloc(&resultado, sizeof(EntradaCuerpo));
    cudaMemcpy(resultado, entradaCuerpo,
               sizeof(EntradaCuerpo), cudaMemcpyHostToDevice);
    return resultado;
}

EntradaCuerpo  obtenerDesdeDispositivo(EntradaCuerpo* entradaCuerpoDeDispositivo)
{
    EntradaCuerpo resultado;
    cudaMemcpy(&resultado, entradaCuerpoDeDispositivo,
               sizeof(EntradaCuerpo), cudaMemcpyDeviceToHost);
    cudaFree(entradaCuerpoDeDispositivo);
    return resultado;
}

void finalizar(EntradaCuerpo* entradaCuerpo)
{
    liberarMatriz((void**)entradaCuerpo->exT, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->conT, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->ndT, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->locT, entradaCuerpo->contexto);

    liberarMatriz((void**)entradaCuerpo->AE_T, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->BE_T, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->AT_T, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->BT_T, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->CTE_T, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->DTE_T, entradaCuerpo->contexto);

    liberarMatriz((void**)entradaCuerpo->AE, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->BE, entradaCuerpo->contexto);
    liberarVector((void*)entradaCuerpo->CTE, entradaCuerpo->contexto);
    liberarVector((void*)entradaCuerpo->DTTE, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->DTE, entradaCuerpo->contexto);

    liberarVector((void*)entradaCuerpo->ndCol, entradaCuerpo->contexto);
    liberarMatriz((void**)entradaCuerpo->extr, entradaCuerpo->contexto);
}
