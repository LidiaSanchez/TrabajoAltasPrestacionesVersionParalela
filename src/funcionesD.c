#include "funcionesD.h"

void GAUSS(double punt[7][3])
{
    //* Declaracion de variables

    double  cnat[7][3]; //valores de las coordenadas naturales
    double  acnat;  //auxiliares
    double  bcnat;  //auxiliares
    int  i,p,j;

    //* Coordenadas naturales
    acnat=(6.0+sqrt(15.0))/21.0;
    bcnat=4.0/7.0-acnat;

    cnat[0][0]=1.0/3.0;
    cnat[1][0]=1.0-2.0*acnat;
    cnat[2][0]=acnat;
    cnat[3][0]=acnat;
    cnat[4][0]=1.0-2.0*bcnat;
    cnat[5][0]=bcnat;
    cnat[6][0]=bcnat;

    cnat[0][1]=1.0/3.0;
    cnat[1][1]=acnat;
    cnat[2][1]=1.0-2.0*acnat;
    cnat[3][1]=acnat;
    cnat[4][1]=bcnat;
    cnat[5][1]=1.0-2.0*bcnat;
    cnat[6][1]=bcnat;

    cnat[0][2]=1.0/3.0;
    cnat[1][2]=acnat;
    cnat[2][2]=acnat;
    cnat[3][2]=1.0-2.0*acnat;
    cnat[4][2]=bcnat;
    cnat[5][2]=bcnat;
    cnat[6][2]=1.0-2.0*bcnat;

    //* Inicializacion de los puntos de integracion
    for( p=0; p<7; p++)        
        for( i=0; i<3; i++)            
            punt[p][i]=0.0;

    //* Calculo de los puntos de integracion
    for( p=0; p<7; p++)        
        for( i=0; i<3; i++)            
            for( j=0; j<3; j++)                
                punt[p][i]=punt[p][i]+cnat[p][j]*extr[j][i];

    return;
}