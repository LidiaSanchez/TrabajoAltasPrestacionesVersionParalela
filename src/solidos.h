#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "funcionesMain.h"
#include "funcionesPersonales.h"
#include "variables.h"
#include "funcionesA.h"
#include "funcionesB.h"
#include "funcionesC.h"
#include "funcionesD.h"
#include "funcionesCuda.h"

#ifndef SOLIDOS_H_
#define SOLIDOS_H_

enum TIM_TYPES
{
    VACIO   = 1,
    GAS     = 2,
    LIQUIDO = 3,
    SOLIDO  = 4,
};

enum TIP_TYPES
{
    CONVECCION_LIBRE = 1,
    CONVECCION_FORZADA = 2,
};

#endif /* SOLIDOS_H_ */