#include "funcionesD.h"

void GAUSS(EntradaCuerpo* entradaCuerpo, double punt[7][3])
{
    //* Declaracion de variables
    // Aprovechamos que son punteros para direccionarlos a las matrices del cuerpo.
    // Nos ahorra muchas modificaciones.
    double** AE_T = entradaCuerpo->AE_T;
    double** BE_T = entradaCuerpo->BE_T;
    double** AT_T = entradaCuerpo->AT_T;
    double** BT_T = entradaCuerpo->BT_T;
    double** CTE_T = entradaCuerpo->CTE_T;
    double** DTE_T = entradaCuerpo->DTE_T;

    double ** AE = entradaCuerpo->AE;
    double ** BE = entradaCuerpo->BE;
    double &AT = entradaCuerpo->AT;
    double &BT = entradaCuerpo->BT;
    double * CTE = entradaCuerpo->CTE;
    double * DTTE = entradaCuerpo->DTTE;
    double ** DTE = entradaCuerpo->DTE;


    // Reemplazamos las referencias globales de variables.h por las del cuerpo. No son punteros, pero el '&' nos permite
    // vincular variables en C++:
    int &nelT = entradaCuerpo->nelT;
    double &GT = entradaCuerpo->GT;
    double &nuT = entradaCuerpo->nuT;
    double &alT = entradaCuerpo->alT;

    double ** exT = entradaCuerpo->exT;
    int    ** conT = entradaCuerpo->conT;
    double ** ndT = entradaCuerpo->ndT;
    double ** locT = entradaCuerpo->locT;

    double * ndCol = entradaCuerpo->ndCol;
    double ** extr = entradaCuerpo->extr;

    int &tipoSimetria = entradaCuerpo->tipoSimetria;
    char &intenum = entradaCuerpo->intenum;

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