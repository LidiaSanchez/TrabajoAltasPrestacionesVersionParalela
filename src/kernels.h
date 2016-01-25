#pragma once

#include <math.h>
#include "variables.h"

void kernelImplicit(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T);
void kernelOXY(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T);
void kernelOXZ(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T);
void kernelOYZ(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T);
void kernelOXYOXZ(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T);
void kernelOXYOYZ(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T);
void kernelOXZOYZ(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T);
void kernelAll(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T);
