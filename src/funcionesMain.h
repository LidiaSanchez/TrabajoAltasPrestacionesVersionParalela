#include "funcionesPersonales.h"
#include "variables.h"

#include "funcionesA.h"
#include "funcionesB.h"
#include "funcionesC.h"
#include "funcionesD.h"
#include <time.h>
#include <stdlib.h>

#ifndef FUNCIONESMAIN_H_
#define FUNCIONESMAIN_H_

//******************************************************************************
//*                                                                            *
//*  Input:       Fichero 'Entrada.dat'                                        *
//*  Output:                                                                   *
//*  Usage:                                                                    *
//*  Description: Entrada de datos fuertemente codificada                      *
//*                                                                            *
//******************************************************************************
void CODIFICADA();

//******************************************************************************
//*                                                                            *
//*  Input:       Fichero 'Entrada.dat'                                        *
//*  Output:                                                                   *
//*  Usage:                                                                    *
//*  Description: Entrada de datos parcialmente codificada                     *
//*                                                                            *
//******************************************************************************
void SEMICODIFICADA();

//******************************************************************************
//*                                                                            *
//*  Input:                                                                    *
//*  Output:                                                                   *
//*  Usage:                                                                    *
//*  Description: Calcula numero de nodos cargados en ambos solidos            *
//*                                                                            *
//******************************************************************************
void NODOS_CARGADOS();

//******************************************************************************
//*                                                                            *
//*  Input:                                                                    *
//*  Output:                                                                   *
//*  Usage:                                                                    *
//*  Description: Calcula direcciones de contacto                              *
//*                                                                            *
//******************************************************************************
void DIR_CONTACTO();

//******************************************************************************
//*                                                                            *
//*  Input:                                                                    *
//*  Output:                                                                   *
//*  Usage:                                                                    *
//*  Description: Calcula las separaciones iniciales en los pares de contacto  *
//*                                                                            *
//******************************************************************************
void GAP_INICIAL();

//******************************************************************************
//*                                                                            *
//*  Name:        Integral.f90                                                 *
//*  Date:        Jueves, 26 de julio de 2005 158:32                           *
//*  Input:       Fichero 'DatosAux.dat'                                       *
//*  Output:      Ficheros 'Coe_AE_A.dat', 'Coe_BE_A.dat', 'Coe_CTE_A.dat',    *
//*               'Coe_DTE_A.dat', 'Coe_AT_A.dat', 'Coe_BT_A.dat',             *
//*               'Coe_AE_B.dat', 'Coe_BE_B.dat', 'Coe_CTE_B.dat',             *
//*               'Coe_DTE_B.dat', 'Coe_AT_B.dat', 'Coe_BT_B.dat',             *
//*  Usage:       integral.exe                                                 *
//*  Description: Realiza las integraciones                                    *
//*                                                                            *
//******************************************************************************
void INTEGRAL();

//===========================================================================
//******************************************************************************
//*                                                                            *
//*  Name:        Potencial.f90                                                *
//*  Date:        Lunes, 7 de febrero de 2005                         *
//*  Input:       Ficheros:'CondCont.dat','Coe _AT_A.dat', 'Coe_BT_A.dat',     *
//*               'Coe_AT_B.dat','Coe_BT_B.dat',                               *
//*  Output:      Ficheros: salpot.sal                                         *
//*               Temps.dat y Flujos.dat                                       *
//*                                                                            *
//*  Usage:       potencial.exe                                                *
//*  Description: Resuelve el problema térmico de contacto entre dos sólidos   *
//*                                                                            *
//******************************************************************************
void TERMICO ();


//************************************************************************
//*            SUBRUTINA PARA CONSTRUIR EL SISTEMA DE ECUACIONES         *
//************************************************************************
void MONTAJE();


//************************************************************************
//*     SUBRUTINA PARA INTERPRETAR LOS RESULTADOS ELASTICOS              *
//************************************************************************
void INTERPR();


//************************************************************************
//*        SUBRUTINA PARA CALCULAR LOS DESLIZAMIENTOS RELATIVOS          *
//************************************************************************
void DESLIZA();

//************************************************************************
//*          SUBRUTINA PARA CHEQUEAR LOS RESULTADOS OBTENIDOS
//*
//************************************************************************
void CHEQUEO();


//************************************************************************
//*            SUBRUTINA PARA LA COMPARACIÓN DE RESISTENCIAS TÉRMICAS    *
//*                            EN LA ZONA DE CONTACTO                    *
//************************************************************************
void COMPRESIS();



//************************************************************************
//*           SUBRUTINA PARA ESCRIBIR LOS RESULTADOS DEFINITIVOS         *
//************************************************************************
void SALIDA_SOLU();


#endif /* FUNCIONESMAIN_H_ */

