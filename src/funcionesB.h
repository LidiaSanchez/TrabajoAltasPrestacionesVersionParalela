#include "funcionesPersonales.h"
#include "variables.h"

#include "funcionesC.h"
#include "funcionesD.h"

#ifndef FUNCIONESB_H_
#define FUNCIONESB_H_

//******************************************************************************
//*                                                                            *
//*  Input:       Fichero 'Entrada.dat'                                        *
//*               ndInicial = Nodo en el que se comienza la asignacion         *
//*               ndFinal = Nodo en el que se finaliza la asignacion           *
//*               tpCod = 1 para codigos elasticos y 2 para codigos termicos   *
//*  Output:                                                                   *
//*  Usage:                                                                    *
//*  Description: Lee los codigos de carga                                     *
//*                                                                            *
//******************************************************************************

void CODIGOS(int* punteroA_ndInicial,int* punteroA_ndFinal,int* punteroA_tpCod);
//******************************************************************************
//*                                                                            *
//*  Input:       intenum = 1 -> Integracion numerica                          *
//*                        = 0 -> Integracion analitica                        *
//*  Output:      AET,BET,ATT,BTT,CT,DT = coeficientes de integracion          *
//*  Usage:                                                                    *
//*  Description: Coordina las integraciones. LLama a las subrutina de         *
//*               integracion apropiada                                        *
//*                                                                            *
//******************************************************************************

void INTEGRA(int* punteroA_intenum,double AET[3][3],double BET[3][3],double* punteroA_ATT,double* punteroA_BTT,double CT[3],double DT[3][3]);
//******************************************************************************
//*                                                                            *
//*  Input:       el = elemento sobre el que se ha integrado                   *
//*  Output:      Coeficientes elasticos transformados                         *
//*  Usage:                                                                    *
//*  Description: Cambia a coordenadas locales los coeficientes de integracion *
//*               elasticos                                                    *
//*                                                                            *
//******************************************************************************

void TRANSFORMA(double AETR[3][3],double BETR[3][3],int* punteroA_el);
//******************************************************************************
//*                                                                            *
//*  Input:                                                                    *
//*  Output:      Ficheros de coeficientes de integracion                      *
//*  Usage:                                                                    *
//*  Description: Almacena los coeficientes de integracion                     *
//*                                                                            *
//******************************************************************************

void ALMACENA(int* punteroA_el,int* punteroA_nd,double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T);


#endif /* FUNCIONESB_H_ */

