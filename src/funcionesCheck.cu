#include "funcionesCheck.h"


void host_printCheck_conT(int** conT)
{
    char prefix[10] = "HOST\0";
    printf("[%s] conT[0][0] = %d\n", prefix, conT[0][0]);
    printf("[%s] conT[0][1] = %d\n", prefix, conT[0][1]);
    printf("[%s] conT[0][2] = %d\n", prefix, conT[0][2]);

    printf("[%s] conT[2500][0] = %d\n", prefix, conT[2500][0]);
    printf("[%s] conT[2500][1] = %d\n", prefix, conT[2500][1]);
    printf("[%s] conT[2500][2] = %d\n", prefix, conT[2500][2]);

    printf("[%s] conT[4999][0] = %d\n", prefix, conT[4999][0]);
    printf("[%s] conT[4999][1] = %d\n", prefix, conT[4999][1]);
    printf("[%s] conT[4999][2] = %d\n", prefix, conT[4999][2]);
}

__device__ void device_printCheck_conT(int** conT)
{
    char prefix[10] = "DEVICE\0";
    printf("[%s] conT[0][0] = %d\n", prefix, conT[0][0]);
    printf("[%s] conT[0][1] = %d\n", prefix, conT[0][1]);
    printf("[%s] conT[0][2] = %d\n", prefix, conT[0][2]);

    printf("[%s] conT[2500][0] = %d\n", prefix, conT[2500][0]);
    printf("[%s] conT[2500][1] = %d\n", prefix, conT[2500][1]);
    printf("[%s] conT[2500][2] = %d\n", prefix, conT[2500][2]);

    printf("[%s] conT[4999][0] = %d\n", prefix, conT[4999][0]);
    printf("[%s] conT[4999][1] = %d\n", prefix, conT[4999][1]);
    printf("[%s] conT[4999][2] = %d\n", prefix, conT[4999][2]);
}

void host_printCheck_exT(double** exT)
{
    char prefix[10] = "HOST\0";
    printf("[%s] exT[0][0] = %.4f\n", prefix, exT[0][0]);
    printf("[%s] exT[0][1] = %.4f\n", prefix, exT[0][1]);
    printf("[%s] exT[0][2] = %.4f\n", prefix, exT[0][2]);

    printf("[%s] exT[2500][0] = %.4f\n", prefix, exT[2500][0]);
    printf("[%s] exT[2500][1] = %.4f\n", prefix, exT[2500][1]);
    printf("[%s] exT[2500][2] = %.4f\n", prefix, exT[2500][2]);

    printf("[%s] exT[4999][0] = %.4f\n", prefix, exT[4999][0]);
    printf("[%s] exT[4999][1] = %.4f\n", prefix, exT[4999][1]);
    printf("[%s] exT[4999][2] = %.4f\n", prefix, exT[4999][2]);
}

__device__ void device_printCheck_exT(double ** exT)
{
    char prefix[10] = "DEVICE\0";
    printf("[%s] exT[0][0] = %.4f\n", prefix, exT[0][0]);
    printf("[%s] exT[0][1] = %.4f\n", prefix, exT[0][1]);
    printf("[%s] exT[0][2] = %.4f\n", prefix, exT[0][2]);

    printf("[%s] exT[2500][0] = %.4f\n", prefix, exT[2500][0]);
    printf("[%s] exT[2500][1] = %.4f\n", prefix, exT[2500][1]);
    printf("[%s] exT[2500][2] = %.4f\n", prefix, exT[2500][2]);

    printf("[%s] exT[4999][0] = %.4f\n", prefix, exT[4999][0]);
    printf("[%s] exT[4999][1] = %.4f\n", prefix, exT[4999][1]);
    printf("[%s] exT[4999][2] = %.4f\n", prefix, exT[4999][2]);
}
