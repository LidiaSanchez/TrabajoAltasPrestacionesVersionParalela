#include "funcionesPersonales.h"
#include "variables.h"
#include <math.h>

#ifndef FUNCIONESD_H_
#define FUNCIONESD_H_

//******************************************************************************
//*                                                                            *
//*  Input:       el = elemento sobre el que se integra                        *
//*               nd = nodo desde el que se integra                            *
//*  Output:     puntos de integracion y funciones peso                        *
//*  Usage:                                                                    *
//*  Description: Obtiene los puntos de integracion de Gauss                   *
//*                                                                            *
//*                                                                            *
//*                                                                            *
//******************************************************************************
void GAUSS(VarPack* varPack, double punt[7][3]);

#endif /* FUNCIONESD_H_ */

