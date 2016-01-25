#include "funcionesPersonales.h"
#include "variables.h"

#include "funcionesB.h"
#include "funcionesC.h"
#include "funcionesD.h"

#include "cublas_v2.h"
#include "magma.h"
#include "magma_lapack.h"

#include <time.h>

#ifndef FUNCIONESA_H_
#define FUNCIONESA_H_

#define CPU 1
#define GPU 1

//******************************************************************************
//*                                                                            *
//*  Input:       Fichero 'Entrada.dat'                                        *
//*               elInicial = Numero de elemento inicial                       *
//*               exInicial = Numero de extremo inicial                        *
//*               bContacto = 1 si es superficie de contacto                   *
//*  Output:      elInicial = Numero de elemento final                         *
//*               exInicial = Numero de extremo final                          *
//*  Usage:                                                                    *
//*  Description: Superficie de contacto codificada de 4 extremos              *
//*                                                                            *
//*                                                                            *
//******************************************************************************

void SUPERFICIE_CUATRO(int* punteroA_elInicial,int* punteroA_exInicial,int* punteroA_bContacto);

//******************************************************************************
//*                                                                            *
//*  Input:       Fichero 'Entrada.dat'                                        *
//*  Output:                                                                   *
//*  Usage:                                                                    *
//*  Description: Superficie de contacto codificada de 3 extremos              *
//*                                                                            *
//******************************************************************************

void SUPERFICIE_TRES(int* punteroA_elInicial,int* punteroA_exInicial,int* punteroA_bContacto);
//******************************************************************************
//*                                                                            *
//*  Input:       Fichero 'Entrada.dat'                                        *
//*               ndInicial = Nodo en el que se comienza la asignacion         *
//*               ndFinal = Nodo en el que se finaliza la asignacion           *
//*  Output:                                                                   *
//*  Usage:                                                                    *
//*  Description: Lee los codigos de carga                                     *
//*                                                                            *
//******************************************************************************

void LEE_CODIGOS(int* punteroA_ndInicial,int* punteroA_ndFinal);
//******************************************************************************
//*                                                                            *
//*  Input:       Fichero 'Entrada.dat'                                        *
//*               ndInicial = Nodo en el que se comienza la asignacion         *
//*               ndFinal = Nodo en el que se finaliza la asignacion           *
//*  Output:                                                                   *
//*  Usage:                                                                    *
//*  Description: Lee los condiciones de contorno                              *
//*                                                                            *
//******************************************************************************

void LEE_CC(int* punteroA_ndInicial,int* punteroA_ndFinal);
//******************************************************************************
//*                                                                            *
//*  Input:                                                                    *
//*  Output:                                                                   *
//*  Usage:                                                                    *
//*  Description: Calcula los nodos en ambos sólidos                           *
//*                                                                            *
//******************************************************************************

void CALCNODOS();
//******************************************************************************
//*                                                                            *
//*  Input:                                                                    *
//*  Output:                                                                   *
//*  Usage:                                                                    *
//*  Description: Calcula el sistema local en ambos sólidos                    *
//*                                                                            *
//******************************************************************************

void SISTLOCAL();
//******************************************************************************
//*                                                                            *
//*  Input:                                                                    *
//*  Output:                                                                   *
//*  Usage:                                                                    *
//*  Description: Subrutina maestra de integracion                             *
//*                                                                            *
//******************************************************************************
//********************************* NOTA ***************************************
//*    ---  Se integra desde el nodo   "nd"   sobre el elemento   "el"  ---    *
//******************************************************************************

void COEFICIENTES(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T);

//******************************************************************************
//*                                                                            *
//*  Input:  ?                                                                 *
//*  Output: coeficientes del sistema ax=b                                     *
//*  Usage:                                                                    *
//*  Description: Monta el sistema de ecuaciones lineales en funcion de codigos*
//*               1= temperatura conocida; 2= flujo conocido;3= contacto       *
//******************************************************************************

void MONTAJETER();
//******************************************************************************
//*                                                                            *
//*  Input:                                                                    *
//*  Output: variables                                                         *
//*  Usage:                                                                    *
//*  Description:  algoritmo de resolucion de un sistema de                    *
//*            ecuaciones por el metodo de GAUSS                               *
//*                                                                            *
//******************************************************************************
void GAUSS_SOLU();
//******************************************************************************
//*                                                                            *
//*  Input:  ?                                                                 *
//*  Output: RESULTADOS TERMICOS EN EL CONTORNO DE LOS DOS CUERPOS             *
//*  Usage:                                                                    *
//*  Description: Ordena e interpreta el vector que contiene las variables     *
//*              termicas e impone las condiciones de contorno en la zona      *
//*            de contacto.                                                    *
//******************************************************************************


void CONTORNOTER();
//**********************************************************************

void SALIDATER();
//************************************************************************
//*       SUBRUTINA QUE MONTA LA PARTE DE LA  MATRIZ  PERTENECIENTE      *
//*                        A LA ZONA LIBRE.                              *
//************************************************************************

void NOCONTA(int* punteroA_el,int* punteroA_nd);
//************************************************************************
//*       SUBRUTINA PARA INTERPRETAR RESULTADOS EN LA ZONA LIBRE         *
//************************************************************************

void INTLIBR(int* punteroA_j,int* punteroA_ix);
//************************************************************************
//*              SUBRUTINA PARA EL CHEQUEO DE TRACCIONES                 *
//************************************************************************

void CHETRAC();
//************************************************************************
//*       SUBRUTINA PARA EL CALCULO Y CHEQUEO DE INTERPENETRACIONES      *
//************************************************************************

void CHEINTE();
#endif /* FUNCIONESA_H_ */

