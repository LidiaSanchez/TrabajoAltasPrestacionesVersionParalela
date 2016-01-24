#include "variables.h"
#include <stdlib.h>

#ifndef FUNCIONESCHECK_H_
#define FUNCIONESCHECK_H_

void host_printCheck_conT(int** conT);
__device__ void device_printCheck_conT(int** conT);

void host_printCheck_exT(double ** exT);
__device__ void device_printCheck_exT(double ** exT);

#endif /* FUNCIONESCHECK_H_ */

