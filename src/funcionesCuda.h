#include "variables.h"

#ifndef FUNCIONES_CUDA_H_
#define FUNCIONES_CUDA_H_


/**
 * <p>Inicializa una estructura de entrada de cuerpo en el contexto especificado.</p><br/>
 * <p>Los diferentes tipos de contexto se especifican en el parámetro tipoEntrada. <b>NOTA:</b> aunque los componentes
 * de la estructura estén inicializados en el contexto especificado, la estructura sólo es accesible desde el HOST.</p>
 * <p>Cuando el contexto especificado es CUDA, los elementos de la estructura se inicializan con memoria del
 * dispositivo. Estos elementos no pueden ser accedidos desde un contexto externo.</p>
 *
 * @param   entradaCuerpo   puntero a estructura a inicializar.
 * @param   contexto        contexto en el que se inicializa la estructura.
 */
void inicializar(EntradaCuerpo* entradaCuerpo, CONTEXTO contexto);

/**
 * <p>Genera una matriz de filas x columnas genérica. Esta matriz reúne las siguientes características:</p>
 * <ul>
 *  <li>Se inicializa en el contexto especificado en el parámetro tipoEntrada.</li>
 *  <li>Es una matriz genérica que debe castearse al tipo que se desee.</li>
 *  <li>El tipo de elemento accedido se especifica por su tamaño en el parámetro tamanioElemento.</li>
 *  <li>Se puede acceder a sus elementos de la forma matriz[i][j] en el código.</li>
 *  <li>Se libera con una sola llamada a liberarMatriz().</li>
 * </ul>
 *  <br/>
 *  <p><b>ADVERTENCIA:</b> esta función solo puede invocarse desde el contexto de HOST</p>
 * @param   tamanioElemento     tamaño en bytes del elemento que contendrá la matriz.
 * @param   filas               número de filas en la matriz.
 * @param   columnas            número de columnas en la matriz.
 * @param   contexto            Contexto en el que se inicializa la matriz.
 *                              (CUDA o HOST).
 *
 * @return  puntero a matriz.
 */
void** generarMatriz(int tamanioElemento, int filas, int columnas, CONTEXTO contexto);

/**
 * <p>Permite copiar una matriz en otra, entre contextos diferentes o el mismo contexto.</p>
 * <p>Esta función no inicializa la memoria de las matrices ni comprueba sus tipos en el proceso de copiado.</p>
 * <p>Si se intenta copiar una matriz sin conocer su contexto el comportamiento <b>no está asegurado</b>.</p>
 *
 * @param   matrizDestino       puntero a matriz destino, El contexto de la matriz se especifica inherentemente
 *                                  en el parámetro dirección
 * @param   matrizOrgien        puntero a matriz origen. El contexto de la matriz se especifica inherentemente
 *                                  en el parámetro dirección
 * @param   tamanioElemento     tamaño del elemento que compone la matriz, en bytes.
 * @param   filas               número de filas de la matriz.
 * @param   columnas            número de columnas de la matriz
 * @param   direccion           Especifica el contexto de las matrices de copia, dependiendo de los siguientes valores:
 *                              <br/>
 *                              <ul>
 *                                  <li><b>MEMCPY_HOST_TO_HOST</b>: Ambas matrices están en el host.</li>
 *                                  <li><b>cudaMemcpyDeviceToHost</b>: Matriz origen en el dispositivo,
 *                                  matriz destino en el host.</li>
 *                                  <li><b>cudaMemcpyHostToDevice</b>: Matriz origen en el host,
 *                                  matriz destino en el dispositivo.</li>
 *                              </ul>
 */
void copiarMatriz(void** matrizDestino, void** matrizOrigen, int tamanioElemento, int filas, int columnas,
                  int direccion);

/**
 * Libera una matriz en el contexto especificado.
 *
 * @param   matriz      Puntero a matriz en el contexto especificado.
 * @param   contexto    Contexto en el que está la memoria de la matriz (CUDA O HOST).
 */
void liberarMatriz(void** matriz, CONTEXTO contexto);

/**
 * Genera un vector del tamaño especificado, válido dentro del contexto especificado.
 */
void* generarVector(int tamanioElemento, int longitud, CONTEXTO contexto);

/**
 * Copia un vector de un contexto a otro.
 */
void copiarVector(void* vectorDestino, void* vectorOrigen, int tamanioElemento, int longitud, int direccion);

/**
 * Libera el vector del contexto especificado.
 */
void liberarVector(void* vector, CONTEXTO contexto);

/**
 * Allocates a 2 dimensional array at the host.
 *
 * Its elements are initialized to 0 by default.
 * It can be freed with a single free() call.
 * It is possible to access this array as array[n][m] in the code.
 * If you want a deep explanation, please, email me at
 * ipazce00@estudiantes.unileon.es
 *
 * @param rows  number of rows.
 * @param columns number of columns.
 * @param sizeOfElement size in bytes of each element.
 *
 * @return a 2 dimensional array.
 */
void** alloc2DOnHost(int rows, int cols, int sizeOfElement);

/**
 * Allocates a 2 dimensional array at the device.
 *
 * Its elements are initialized to 0 by default.
 * It can be freed with a single free() call.
 * It is possible to access this array as array[n][m] in the device code.
 * If you want a deep explanation, please, email me at
 * ipazce00@estudiantes.unileon.es
 *
 * @param rows  number of rows.
 * @param columns number of columns.
 * @param sizeOfElement size in bytes of each element.
 *
 * @return a 2 dimensional array, only accessible in the device.
 */
void ** alloc2DOnDevice(int rows, int cols, int sizeOfElement);

/**
 * Organiza el contenido de una matriz inicializada en el dispositivo. Esta función se invoca desde alloc2DOnDevice()
 * para operar la matriz, puesto que su contexto está en el dispositivo y no se puede acceder desde el host.
 *
 * La organización consiste en colocar los punteros a cada fila correctamente, de forma que se pueda acceder mediante
 * matriz[i][j].
 */
__global__ void __organize2DMatrix(void** matrix, int rows, int cols, int sizeOfElement);

/**
 * Copia el contenido de una estructura en otra. El contexto de los componentes de la estructura se especifican en
 * dirección.
 */
void copiarEstructura(EntradaCuerpo* destino, EntradaCuerpo* origen, int direccion);

/**
 * Pasa una estructura al contexto del dispositivo. La estructura DEBE estar inicializada en el contexto del
 * dispositivo, de lo contrario, la estructura usada en el dispositivo podría acceder a miembros de la estructura
 * apuntando a memoria del host (operación ilegal).
 */
EntradaCuerpo* pasarADispositivo(EntradaCuerpo* entradaCuerpo);


/**
 * Obtiene una estructura pasada previamente al dispositivo de nuevo al host.
 * La estructura referenciada por parámetro es liberada de memoria del dispositivo.
 */
EntradaCuerpo  obtenerDesdeDispositivo(EntradaCuerpo* entradaCuerpoDeDispositivo);

/**
 * Libera el contenido de una estructura de cuerpo en función del contexto en el que se encuentre inicializada.
 *
 * @param   entradaCuerpo       puntero a estructura de entrada.
 */
void finalizar(EntradaCuerpo* entradaCuerpo);

#endif /* FUNCIONES_CUDA_H_ */