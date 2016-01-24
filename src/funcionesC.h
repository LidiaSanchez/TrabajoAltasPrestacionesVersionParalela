#include <math.h>
#include "funcionesPersonales.h"
#include "variables.h"

#include "funcionesD.h"


#ifndef FUNCIONESC_H_
#define FUNCIONESC_H_

//****************************************************************************************
//*                                                                                      *
//*  Input:                                                                              *
//*                                                                                      *
//*  Output:      AET,BET,ATT,BTT,CT,DT = coeficientes de integracion                    *
//*  Usage:                                                                              *
//*  Description: Realiza las integraciones analiticas.                                  *
//*                                                                                      *
//*                                                                                      *
//****************************************************************************************
__device__
void ANALITICA(EntradaCuerpo* entradaCuerpo, double AEA[3][3],double BEA[3][3],double* punteroA_ATA,double* punteroA_BTA,double * CTEA,double DTEA[3][3],double * n,double * bar);


//******************************************************************************
//*                                                                            *
//*  Input:       extrs(4,3) = Coordenadas de los vertices del triangulo a     *
//*                           dividir                                          *
//*  Output:      subextr(4,3,3) = Coordenadas de los vertices de los cuatro   *
//*                                subtriangulos                               *
//*  Usage:                                                                    *
//*  Description: Divide un triangulo en cuatro triangulos segun esquema       *
//*                                                                            *
//*          1  ----------------------------  3                                *
//*             \              /\          /                                   *
//*              \     1     /    \   2   /                                    *
//*                \       /   3    \    /                                     *
//*                  \   /            \ /                                      *
//*                    \ --------------/                                       *
//*                      \            /                                        *
//*                        \    4    /                                         *
//*                          \      /                                          *
//*                            \   /                                           *
//*                              \/                                            *
//*                               2                                            *
//*                                                                            *
//******************************************************************************
__device__
void SUBDIVIDE(double extrs[4][3],double subextr[4][4][3]);

//****************************************************************************************
//*                                                                                      *
//*  Input:       el = elemento sobre el que se integra                                  *
//*               nd = nodo desde el que se integra                                      *
//*  Output:      AET,BET,ATT,BTT,CT,DT = coeficientes de integracion                    *
//*  Usage:                                                                              *
//*  Description: Realiza las integraciones numericas.                                   *
//*                                                                                      *
//*                                                                                      *
//****************************************************************************************
__device__
void NUMERICA(EntradaCuerpo* entradaCuerpo, double AEN[3][3],double BEN[3][3],double* punteroA_ATN,double* punteroA_BTN,double * CTEN,double DTEN[3][3],double * n,double* punteroA_distancia);
#endif /* FUNCIONESC_H_ */

