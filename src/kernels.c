#include "kernels.h"

void GAUSS(double punt[7][3]) {
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
    for( p=0; p<7; p++) {
        for( i=0; i<3; i++) {
            punt[p][i]=0.0;
        }
    }

    //* Calculo de los puntos de integracion
    for( p=0; p<7; p++) {

        for( i=0; i<3; i++) {

            for( j=0; j<3; j++) {

                punt[p][i]=punt[p][i]+cnat[p][j]*extr[j][i];
            }                
        }            
    }        

    return;
}

void ANALITICA(double AEA[3][3],double BEA[3][3],double* punteroA_ATA,double* punteroA_BTA,double CTEA[3],double DTEA[3][3],double n[3],double bar[3]) {
    //* Declaracion de variables


    /*double AEA[3][3];*    //*double BEA[3][3];*/;      //Coeficientes de integracion elasticos
    double ATA=*punteroA_ATA;double BTA=*punteroA_BTA;; //Coeficientes de integracion termicos
    /*double CTEA[3];*  //*double DTEA[3][3];*/;      //Coeficientes de integracion termoelasticos

    int  i,j;
    /*double n[3];*/;   //vector normal al elemento
    /*double bar[3];*/; //baricentro del elemento


    double  XB,YB,ZB,R,DD,SE,CO,T2,XI1,C,C1,C4,C5;
    double  E11,E12,E13,E21,E22,E23,E31,E32,E33;


    //*---------------------------------------------------------------------------------------?


    //*Inicializamos los coeficientes
    //coeficientes elasticos
    if((tpproE == 1) || (tpproTE == 1)) {
        for( i=1; i<=3; i++) {
            for( j=1; j<=3; j++) {
                AEA[i-1][j-1]=0.0;
                BEA[i-1][j-1]=0.0;
            }
        }
    }
    //*coeficientes termicos
    if((tpproT == 1) || (tpproTE == 1)) {
        ATA=0.50;
        BTA=0.0;
    }
    //*coeficientes termoelasticos
    if(tpproTE == 1) {
        for( i=1; i<=3; i++) {
            CTEA[i-1]=0.0;
            for( j=1; j<=3; j++) {
                DTEA[i-1][j-1]=0.0;
            }
        }
    }
    //*=========================================================================================


    //* Como ANTONIO


    C=1.0/(16.0*4.0*atan(1.0)*GT*(1-nuT));
    C1=(1-2.0*nuT)/(2.0*(1-nuT)*4*4.0*atan(1.0));
    C4=1/(4.0*4.0*atan(1.0));
    C5=alT*(1.0+nuT)/(8.0*4.0*atan(1.0)*(1.0-nuT));

    //C=1.D0/(16.D0*3.141592654D0*GT*(1-nuT))
    //C1=(1-2.D0*nuT)/(2.D0*(1-nuT)*4*3.141592654D0)
    //C4=1/(4.D0*3.141592654D0)
    //C5=alT*(1.D0+nuT)/(8.D0*3.141592654D0*(1.D0-nuT))

    XB=bar[1-1];
    YB=bar[2-1];
    ZB=bar[3-1];
    E31=n[1-1];
    E32=n[2-1];
    E33=n[3-1];

    for( i=1; i<=3; i++) {
        R=sqrt(pow((extr[i-1][1-1]-extr[i+1-1][1-1]),2)+pow((extr[i-1][2-1]-extr[i+1-1][2-1]),2)+pow((extr[i-1][3-1]-extr[i+1-1][3-1]),2));
        E11=(extr[i-1][1-1]-extr[i+1-1][1-1])/R;
        E12=(extr[i-1][2-1]-extr[i+1-1][2-1])/R;
        E13=(extr[i-1][3-1]-extr[i+1-1][3-1])/R;
        E21=E32*E13-E33*E12;
        E22=E33*E11-E31*E13;
        E23=E31*E12-E32*E11;
        DD=fabs(E21*(extr[i-1][1-1]-XB)+E22*(extr[i-1][2-1]-YB)+E23*(extr[i-1][3-1]-ZB));
        for( j=i; j<=i+1; j++) {
            R=sqrt(pow((XB-extr[j-1][1-1]),2)+pow((YB-extr[j-1][2-1]),2)+pow((ZB-extr[j-1][3-1]),2));
            XI1=(extr[j-1][1-1]-XB)*E11+(extr[j-1][2-1]-YB)*E12+(extr[j-1][3-1]-ZB)*E13;

            SE=XI1/R;

            CO=DD/R;

            T2=log((SE+1.0)/CO);

            if((tpproE == 1) || (tpproTE == 1)) {
                AEA[1-1][2-1]=AEA[1-1][2-1]-(1.0+(i-j)*2.0)*C1*E13*log(R+XI1);
                AEA[1-1][3-1]=AEA[1-1][3-1]+(1.0+(i-j)*2.0)*C1*E12*log(R+XI1);
                AEA[2-1][3-1]=AEA[2-1][3-1]-(1.0+(i-j)*2.0)*C1*E11*log(R+XI1);

                BEA[1-1][1-1]=BEA[1-1][1-1]+(DD*C*((3.0-4.0*nuT)*T2+pow(E11,2)*(-SE+T2)+pow(E21,2)*SE-
                    2*E11*E21*CO))*(1.0+(i-j)*2.0);;
                BEA[2-1][2-1]=BEA[2-1][2-1]+(DD*C*((3.0-4.0*nuT)*T2+pow(E12,2)*(-SE+T2)+pow(E22,2)*SE-
                    2*E12*E22*CO))*(1.0+(i-j)*2.0);;
                BEA[3-1][3-1]=BEA[3-1][3-1]+(DD*C*((3.0-4.0*nuT)*T2+pow(E13,2)*(-SE+T2)+pow(E23,2)*SE-
                    2*E13*E23*CO))*(1.0+(i-j)*2.0);;
                BEA[1-1][2-1]=BEA[1-1][2-1]+(1.0+(i-j)*2.0)*DD*C*(E11*E12*(-SE+T2)+
                    E21*E22*SE-(E11*E22+E21*E12)*CO);;
                BEA[1-1][3-1]=BEA[1-1][3-1]+(1.0+(i-j)*2.0)*DD*C*(E11*E13*(-SE+T2)+
                    E21*E23*SE-(E11*E23+E21*E13)*CO);;
                BEA[2-1][3-1]=BEA[2-1][3-1]+(1.0+(i-j)*2.0)*DD*C*(E12*E13*(-SE+T2)+
                    E22*E23*SE-(E12*E23+E22*E13)*CO);;
            }
            if((tpproT == 1) || (tpproTE == 1)) {
                BTA=BTA+(1+(i-j)*2.0)*C4*DD*T2;
            }
            if(tpproTE == 1) {
                CTEA[1-1]=CTEA[1-1]+(1+(i-j)*2.0)*pow(DD,2)*C5/2.0*(E11/CO+E21*T2);
                CTEA[2-1]=CTEA[2-1]+(1+(i-j)*2.0)*pow(DD,2)*C5/2.0*(E12/CO+E22*T2);
                CTEA[3-1]=CTEA[3-1]+(1+(i-j)*2.0)*pow(DD,2)*C5/2.0*(E13/CO+E23*T2);

                DTEA[1-1][1-1]=DTEA[1-1][1-1]+(1+(i-j)*2.0)*C5*DD*(T2-E11*E11*(T2-SE)+
                    (E11*E21+E21*E11)*CO-(E21*E21)*SE);;
                DTEA[2-1][2-1]=DTEA[2-1][2-1]+(1+(i-j)*2.0)*C5*DD*(T2-E12*E12*(T2-SE)+
                    (E12*E22+E22*E12)*CO-(E22*E22)*SE);;
                DTEA[3-1][3-1]=DTEA[3-1][3-1]+(1+(i-j)*2.0)*C5*DD*(T2-E13*E13*(T2-SE)+
                    (E13*E23+E23*E13)*CO-(E23*E23)*SE);;
                DTEA[1-1][2-1]=DTEA[1-1][2-1]+(1+(i-j)*2.0)*C5*DD*(-E11*E12*(T2-SE)+
                    (E11*E22+E21*E12)*CO-(E21*E22)*SE);;
                DTEA[1-1][3-1]=DTEA[1-1][3-1]+(1+(i-j)*2.0)*C5*DD*(-E11*E13*(T2-SE)+
                    (E11*E23+E21*E13)*CO-(E21*E23)*SE);;
                DTEA[2-1][3-1]=DTEA[2-1][3-1]+(1+(i-j)*2.0)*C5*DD*(-E12*E13*(T2-SE)+
                    (E12*E23+E22*E13)*CO-(E22*E23)*SE);;
            }
        }
    }
    DTEA[2-1][1-1]=DTEA[1-1][2-1];
    DTEA[3-1][1-1]=DTEA[1-1][3-1];
    DTEA[3-1][2-1]=DTEA[2-1][3-1];

    if((tpproE == 1) || (tpproTE == 1)) {
        for( i=1; i<=2; i++) {
            AEA[i-1][i-1]=AEA[i-1][i-1]+0.50;
            for( j=i+1; j<=3; j++) {
                AEA[j-1][i-1]=-AEA[i-1][j-1];
                BEA[j-1][i-1]=BEA[i-1][j-1];
            }
        }
        AEA[3-1][3-1]=AEA[3-1][3-1]+0.50;
    }
    *punteroA_ATA = ATA; *punteroA_BTA = BTA; return;
}

void SUBDIVIDE(double extrs[4][3],double subextr[4][4][3]) {
    //* Declaracion de variables

    /*double subextr[4][4][3];*/;   //Coordenadas de los vertices de los cuatro subtriangulos
    /*double extrs[4][3];*/;    //Coordenadas de los vertices del triangulo a dividir
    //* Subelemento 1
    //* Vertice 1
    subextr[1-1][1-1][1-1]=extrs[1-1][1-1];
    subextr[1-1][1-1][2-1]=extrs[1-1][2-1];
    subextr[1-1][1-1][3-1]=extrs[1-1][3-1];
    //* Vertice 2
    subextr[1-1][2-1][1-1]=(extrs[1-1][1-1]+extrs[2-1][1-1])/2.0;
    subextr[1-1][2-1][2-1]=(extrs[1-1][2-1]+extrs[2-1][2-1])/2.0;
    subextr[1-1][2-1][3-1]=(extrs[1-1][3-1]+extrs[2-1][3-1])/2.0;
    //* Vertice 3
    subextr[1-1][3-1][1-1]=(extrs[1-1][1-1]+extrs[3-1][1-1])/2.0;
    subextr[1-1][3-1][2-1]=(extrs[1-1][2-1]+extrs[3-1][2-1])/2.0;
    subextr[1-1][3-1][3-1]=(extrs[1-1][3-1]+extrs[3-1][3-1])/2.0;
    //* Vertice 4 = Vertice 1
    subextr[1-1][4-1][1-1]=subextr[1-1][1-1][1-1];
    subextr[1-1][4-1][2-1]=subextr[1-1][1-1][2-1];
    subextr[1-1][4-1][3-1]=subextr[1-1][1-1][3-1];
    //* Subelemento 2
    //* Vertice 1
    subextr[2-1][1-1][1-1]=extrs[3-1][1-1];
    subextr[2-1][1-1][2-1]=extrs[3-1][2-1];
    subextr[2-1][1-1][3-1]=extrs[3-1][3-1];
    //* Vertice 2
    subextr[2-1][2-1][1-1]=(extrs[1-1][1-1]+extrs[3-1][1-1])/2.0;
    subextr[2-1][2-1][2-1]=(extrs[1-1][2-1]+extrs[3-1][2-1])/2.0;
    subextr[2-1][2-1][3-1]=(extrs[1-1][3-1]+extrs[3-1][3-1])/2.0;
    //* Vertice 3
    subextr[2-1][3-1][1-1]=(extrs[2-1][1-1]+extrs[3-1][1-1])/2.0;
    subextr[2-1][3-1][2-1]=(extrs[2-1][2-1]+extrs[3-1][2-1])/2.0;
    subextr[2-1][3-1][3-1]=(extrs[2-1][3-1]+extrs[3-1][3-1])/2.0;
    //* Vertice 4 = Vertice 1
    subextr[2-1][4-1][1-1]=subextr[2-1][1-1][1-1];
    subextr[2-1][4-1][2-1]=subextr[2-1][1-1][2-1];
    subextr[2-1][4-1][3-1]=subextr[2-1][1-1][3-1];
    //* Subelemento 3
    //* Vertice 1
    subextr[3-1][1-1][1-1]=(extrs[1-1][1-1]+extrs[2-1][1-1])/2.0;
    subextr[3-1][1-1][2-1]=(extrs[1-1][2-1]+extrs[2-1][2-1])/2.0;
    subextr[3-1][1-1][3-1]=(extrs[1-1][3-1]+extrs[2-1][3-1])/2.0;
    //* Vertice 2
    subextr[3-1][2-1][1-1]=(extrs[2-1][1-1]+extrs[3-1][1-1])/2.0;
    subextr[3-1][2-1][2-1]=(extrs[2-1][2-1]+extrs[3-1][2-1])/2.0;
    subextr[3-1][2-1][3-1]=(extrs[2-1][3-1]+extrs[3-1][3-1])/2.0;
    //* Vertice 3
    subextr[3-1][3-1][1-1]=(extrs[1-1][1-1]+extrs[3-1][1-1])/2.0;
    subextr[3-1][3-1][2-1]=(extrs[1-1][2-1]+extrs[3-1][2-1])/2.0;
    subextr[3-1][3-1][3-1]=(extrs[1-1][3-1]+extrs[3-1][3-1])/2.0;
    //* Vertice 4 = Vertice 1
    subextr[3-1][4-1][1-1]=subextr[3-1][1-1][1-1];
    subextr[3-1][4-1][2-1]=subextr[3-1][1-1][2-1];
    subextr[3-1][4-1][3-1]=subextr[3-1][1-1][3-1];
    //* Subelemento 4
    //* Vertice 1
    subextr[4-1][1-1][1-1]=extrs[2-1][1-1];
    subextr[4-1][1-1][2-1]=extrs[2-1][2-1];
    subextr[4-1][1-1][3-1]=extrs[2-1][3-1];
    //* Vertice 2
    subextr[4-1][2-1][1-1]=(extrs[2-1][1-1]+extrs[3-1][1-1])/2.0;
    subextr[4-1][2-1][2-1]=(extrs[2-1][2-1]+extrs[3-1][2-1])/2.0;
    subextr[4-1][2-1][3-1]=(extrs[2-1][3-1]+extrs[3-1][3-1])/2.0;
    //* Vertice 3
    subextr[4-1][3-1][1-1]=(extrs[1-1][1-1]+extrs[2-1][1-1])/2.0;
    subextr[4-1][3-1][2-1]=(extrs[1-1][2-1]+extrs[2-1][2-1])/2.0;
    subextr[4-1][3-1][3-1]=(extrs[1-1][3-1]+extrs[2-1][3-1])/2.0;
    //* Vertice 4 = Vertice 1
    subextr[4-1][4-1][1-1]=subextr[4-1][1-1][1-1];
    subextr[4-1][4-1][2-1]=subextr[4-1][1-1][2-1];
    subextr[4-1][4-1][3-1]=subextr[4-1][1-1][3-1];
    return;
}

void NUMERICA(double AEN[3][3],double BEN[3][3],double* punteroA_ATN,double* punteroA_BTN,double CTEN[3],double DTEN[3][3],double n[3],double* punteroA_distancia) {
    //* Declaracion de variables


    /*double AEN[3][3];*    //*double BEN[3][3];*/;      //Coeficientes de integracion elasticos
    double ATN=*punteroA_ATN;double BTN=*punteroA_BTN;; //Coeficientes de integracion termicos
    /*double CTEN[3];*  //*double DTEN[3][3];*/;      //Coeficientes de integracion termoelasticos
    double distancia=*punteroA_distancia;;  //Distancia entre el nodo y el plano del elemento

    double  punt[7][3]; //puntos de integracion de Gauss
    double  Afp;    //auxiliares
    double  Bfp;    //auxiliares
    double  w[7];   //valores de las funciones peso
    double  s;  //Auxiliar para el calculo del area
    double  Area;   //Area del elemento
    int  i,j;   //Auxiliares
    double  lado[3];    //longitud de cada lado del elemento
    double  lad[3][3];  //vector de cada lado del elemento
    /*double n[3];*/;   //vector normal al elemento
    double  XP,YP,ZP,XNUE,YNUE,ZNUE;
    double  R,C,C1,C2,C4,C5;

    //* Calcula los lados de los elementos
    for( i=1; i<=3; i++) {
        lado[i-1]=0.0;
        for( j=1; j<=3; j++) {
            lado[i-1]=lado[i-1]+pow((extr[i+1-1][j-1]-extr[i-1][j-1]),2);
        }
        lado[i-1]=sqrt(lado[i-1]);
    }
    for( i=1; i<=3; i++) {
        for( j=1; j<=3; j++) {
            lad[i-1][j-1]= (extr[i+1-1][j-1]-extr[i-1][j-1])/lado[i-1];
        }
    }
    //* Calcula el area del elemento (3 lados)
    s=(lado[1-1]+lado[2-1]+lado[3-1])/2.0;
    Area=sqrt(s*(s-lado[1-1])*(s-lado[2-1])*(s-lado[3-1]));

    //* Calcula los puntos de integracón
    GAUSS(punt);if(enExcepcion==1)return;

    //* funciones peso
    Afp=(155.0+sqrt(15.0))/2400.0;
    Bfp=31.0/240.0-Afp;

    w[1-1]=9.0/80.0;
    w[2-1]=Afp;
    w[3-1]=Afp;
    w[4-1]=Afp;
    w[5-1]=Bfp;
    w[6-1]=Bfp;
    w[7-1]=Bfp;
    //*-----------------------------------------------------------------------------------?
    //* Inicializamos coeficientes
    //*coeficientes termicos
    if((tpproT == 1) || (tpproTE == 1)) {
        ATN=0.0;
        BTN=0.0;
    }
    //*coeficientes elasticos
    if((tpproE == 1) || (tpproTE == 1)) {
        for( i=1; i<=3; i++) {
            for( j=1; j<=3; j++) {
                AEN[i-1][j-1]=0.0;
                BEN[i-1][j-1]=0.0;
            }
        }
    }
    //*coeficientes termoelasticos
    if(tpproTE == 1) {
        for( i=1; i<=3; i++) {
            CTEN[i-1]=0.0;
            for( j=1; j<=3; j++) {
                DTEN[i-1][j-1]=0.0;
            }
        }
    }
    //* ANTONIO  =========================================================================
    C=1.0/(16.0*4.0*atan(1.0)*GT*(1-nuT));
    C1=(1-2.0*nuT)/(2.0*(1-nuT)*4*4.0*atan(1.0));
    C2=3.0/(1.0-2.0*nuT);
    C4=1/(4.0*4.0*atan(1.0));
    C5=alT*(1.0+nuT)/(8.0*4.0*atan(1.0)*(1.0-nuT));
    //C=1.D0/(16.D0*3.141592654D0*GT*(1-nuT))
    //C1=(1-2.D0*nuT)/(2.D0*(1-nuT)*4*3.141592654D0)
    //C2=3.D0/(1.D0-2.D0*nuT)
    //C4=1/(4.D0*3.141592654D0)
    //C5=alT*(1.D0+nuT)/(8.D0*3.141592654D0*(1.D0-nuT))


    XP=ndCol[1-1];
    YP=ndCol[2-1];
    ZP=ndCol[3-1];
    XNUE=n[1-1];
    YNUE=n[2-1];
    ZNUE=n[3-1];

    for( i=1; i<=7; i++) {
        R=sqrt(pow((punt[i-1][1-1]-XP),2)+pow((punt[i-1][2-1]-YP),2)+pow((punt[i-1][3-1]-ZP),2));
        if((tpproE == 1) || (tpproTE == 1)) {
            BEN[1-1][1-1]=BEN[1-1][1-1]+(C*(3.0-4.0*nuT+pow(((punt[i-1][1-1]-XP)/R),2))/R)*w[i-1]*2*Area;
            BEN[2-1][2-1]=BEN[2-1][2-1]+(C*(3.0-4.0*nuT+pow(((punt[i-1][2-1]-YP)/R),2))/R)*w[i-1]*2*Area;
            BEN[3-1][3-1]=BEN[3-1][3-1]+(C*(3.0-4.0*nuT+pow(((punt[i-1][3-1]-ZP)/R),2))/R)*w[i-1]*2*Area;
            BEN[1-1][2-1]=BEN[1-1][2-1]+(C*(punt[i-1][1-1]-XP)*(punt[i-1][2-1]-YP)/pow(R,3))*w[i-1]*2*Area;
            BEN[1-1][3-1]=BEN[1-1][3-1]+(C*(punt[i-1][1-1]-XP)*(punt[i-1][3-1]-ZP)/pow(R,3))*w[i-1]*2*Area;
            BEN[2-1][3-1]=BEN[2-1][3-1]+(C*(punt[i-1][2-1]-YP)*(punt[i-1][3-1]-ZP)/pow(R,3))*w[i-1]*2*Area;
            AEN[1-1][1-1]=AEN[1-1][1-1]-(C1*distancia*(1.0+C2*pow(((punt[i-1][1-1]-XP)/R),2))/pow(R,3))*w[i-1]*2*Area;
            AEN[2-1][2-1]=AEN[2-1][2-1]-(C1*distancia*(1.0+C2*pow(((punt[i-1][2-1]-YP)/R),2))/pow(R,3))*w[i-1]*2*Area;
            AEN[3-1][3-1]=AEN[3-1][3-1]-(C1*distancia*(1.0+C2*pow(((punt[i-1][3-1]-ZP)/R),2))/pow(R,3))*w[i-1]*2*Area;
            AEN[1-1][2-1]=AEN[1-1][2-1]-(C1/pow(R,3)*(distancia*C2*(punt[i-1][1-1]-XP)*(punt[i-1][2-1]-YP)/pow(R,2)-
                YNUE*(punt[i-1][1-1]-XP)+XNUE*(punt[i-1][2-1]-YP)))*w[i-1]*2*Area;;
            AEN[1-1][3-1]=AEN[1-1][3-1]-(C1/pow(R,3)*(distancia*C2*(punt[i-1][1-1]-XP)*(punt[i-1][3-1]-ZP)/pow(R,2)-
                ZNUE*(punt[i-1][1-1]-XP)+XNUE*(punt[i-1][3-1]-ZP)))*w[i-1]*2*Area;;
            AEN[2-1][3-1]=AEN[2-1][3-1]-(C1/pow(R,3)*(distancia*C2*(punt[i-1][2-1]-YP)*(punt[i-1][3-1]-ZP)/pow(R,2)-
                ZNUE*(punt[i-1][2-1]-YP)+YNUE*(punt[i-1][3-1]-ZP)))*w[i-1]*2*Area;;
            AEN[2-1][1-1]=AEN[2-1][1-1]-(C1/pow(R,3)*(distancia*C2*(punt[i-1][1-1]-XP)*(punt[i-1][2-1]-YP)/pow(R,2)-
                XNUE*(punt[i-1][2-1]-YP)+YNUE*(punt[i-1][1-1]-XP)))*w[i-1]*2*Area;;
            AEN[3-1][1-1]=AEN[3-1][1-1]-(C1/pow(R,3)*(distancia*C2*(punt[i-1][1-1]-XP)*(punt[i-1][3-1]-ZP)/pow(R,2)-
                XNUE*(punt[i-1][3-1]-ZP)+ZNUE*(punt[i-1][1-1]-XP)))*w[i-1]*2*Area;;
            AEN[3-1][2-1]=AEN[3-1][2-1]-(C1/pow(R,3)*(distancia*C2*(punt[i-1][2-1]-YP)*(punt[i-1][3-1]-ZP)/pow(R,2)-
                YNUE*(punt[i-1][3-1]-ZP)+ZNUE*(punt[i-1][2-1]-YP)))*w[i-1]*2*Area;;

        }
        if((tpproT == 1) || (tpproTE == 1)) {
            ATN=ATN-C4*distancia*(1/pow(R,3))*w[i-1]*2.0*Area;
            BTN=BTN+C4*(1/R)*w[i-1]*2.0*Area;
        }
        if(tpproTE == 1) {
            CTEN[1-1]=CTEN[1-1]+C5*((punt[i-1][1-1]-XP)/R)*w[i-1]*2*Area;
            CTEN[2-1]=CTEN[2-1]+C5*((punt[i-1][2-1]-YP)/R)*w[i-1]*2*Area;
            CTEN[3-1]=CTEN[3-1]+C5*((punt[i-1][3-1]-ZP)/R)*w[i-1]*2*Area;

            DTEN[1-1][1-1]=DTEN[1-1][1-1]+C5*((1-pow(((punt[i-1][1-1]-XP)/R),2))/R)*w[i-1]*2*Area;
            DTEN[2-1][2-1]=DTEN[2-1][2-1]+C5*((1-pow(((punt[i-1][2-1]-YP)/R),2))/R)*w[i-1]*2*Area;
            DTEN[3-1][3-1]=DTEN[3-1][3-1]+C5*((1-pow(((punt[i-1][3-1]-ZP)/R),2))/R)*w[i-1]*2*Area;
            DTEN[1-1][2-1]=DTEN[1-1][2-1]+C5*(-(punt[i-1][1-1]-XP)*(punt[i-1][2-1]-YP)/pow(R,3))*w[i-1]*2*Area;
            DTEN[1-1][3-1]=DTEN[1-1][3-1]+C5*(-(punt[i-1][1-1]-XP)*(punt[i-1][3-1]-ZP)/pow(R,3))*w[i-1]*2*Area;
            DTEN[2-1][3-1]=DTEN[2-1][3-1]+C5*(-(punt[i-1][2-1]-YP)*(punt[i-1][3-1]-ZP)/pow(R,3))*w[i-1]*2*Area;

        }
    }
    BEN[2-1][1-1]=BEN[1-1][2-1];
    BEN[3-1][1-1]=BEN[1-1][3-1];
    BEN[3-1][2-1]=BEN[2-1][3-1];
    DTEN[2-1][1-1]=DTEN[1-1][2-1];
    DTEN[3-1][1-1]=DTEN[1-1][3-1];
    DTEN[3-1][2-1]=DTEN[2-1][3-1];

    *punteroA_ATN = ATN; *punteroA_BTN = BTN; *punteroA_distancia = distancia; return;
}

void INTEGRA(int* punteroA_intenum,double AET[3][3],double BET[3][3],double* punteroA_ATT,double* punteroA_BTT,double CT[3],double DT[3][3]) {
    //* Declaracion de variables

    int intenum=*punteroA_intenum;; //tipo de integracion
    /*double AET[3][3];*    //*double BET[3][3];*/;      //Coeficientes de integracion elasticos
    double ATT=*punteroA_ATT;double BTT=*punteroA_BTT;; //Coeficientes de integracion termicos
    /*double CT[3];*    //*double DT[3][3];*/;      //Coeficientes de integracion termoelasticos
    double  AEP[3][3],BEP[3][3];    //Coeficientes de integracion elasticos parciales
    double  ATP,BTP;    //Coeficientes de integracion termicos parciales
    double  CP[3],DP[3][3]; //Coeficientes de integracion termoelasticos parciales
    double  dist;   //Distancia entre el punto de colocacion y el baricentro del elemento
    double  subextr[MAX_SUB][4][3]; //Coordenadas de los vertices de los cuatro subtriangulos
    double  extrs[4][3];    //Coordenadas de los vertices del triangulo a dividir
    double  subex[4][4][3]; //Auxiliares
    int  i,j,k,m,p;
    double  bar[3]; //baricentro del elemento
    double  lado[3];    //longitud de cada lado del elemento
    double  lad[3][3];  //vector de cada lado del elemento
    double  n[3];
    double  distancia,dis[3];

    //* Asigna cuarto extremo y calcula baricentro del elemento
    for( i=1; i<=3; i++) {
        extr[4-1][i-1]=extr[1-1][i-1];
        bar[i-1]=(extr[1-1][i-1]+extr[2-1][i-1]+extr[3-1][i-1])/3.0;
    }
    //* Calcula los lados de los elementos
    for( i=1; i<=3; i++) {
        lado[i-1]=0.0;
        for( j=1; j<=3; j++) {
            lado[i-1]=lado[i-1]+pow((extr[i+1-1][j-1]-extr[i-1][j-1]),2);
        }
        lado[i-1]=sqrt(lado[i-1]);
    }
    for( i=1; i<=3; i++) {
        for( j=1; j<=3; j++) {
            lad[i-1][j-1]= (extr[i+1-1][j-1]-extr[i-1][j-1])/lado[i-1];
        }
    }
    //* Calcula el vector normal
    n[1-1]=-lad[1-1][2-1]*lad[3-1][3-1]+lad[1-1][3-1]*lad[3-1][2-1];
    n[2-1]=-lad[1-1][3-1]*lad[3-1][1-1]+lad[1-1][1-1]*lad[3-1][3-1];
    n[3-1]=-lad[1-1][1-1]*lad[3-1][2-1]+lad[1-1][2-1]*lad[3-1][1-1];

    //* Decide el tipo de integracion
    if(intenum == 0) {
        ANALITICA(AET,BET,&ATT,&BTT,CT,DT,n,bar);if(enExcepcion==1)return;
    }
    else      
    {
    //*Calcula el vector que va del baricentro al nodo desde el que se integra
        dist=0.0;
        distancia=0.0;
        for( i=1; i<=3; i++) {
            dis[i-1]=(bar[i-1]-ndCol[i-1]);
            dist=dist+pow(dis[i-1],2);
            distancia=distancia+dis[i-1]*n[i-1];
        }
        dist=sqrt(dist);
    //*Calcula la distancia del nodo desde el que se integra al plano del elemento
        distancia=fabs(distancia);

    //* Decide si hay que subdividir el elemento
        if((lado[1-1] > dist) || (lado[2-1] > dist) || (lado[3-1] > dist)) {
    //* Hay que subdividir el elemento
            for( i=1; i<=4; i++) {
                for( j=1; j<=3; j++) {
                    extrs[i-1][j-1]=extr[i-1][j-1];
                }
            }
            SUBDIVIDE(extrs, subex);if(enExcepcion==1)return;
            for( i=1; i<=4; i++) {
                for( j=1; j<=4; j++) {
                    for( k=1; k<=3; k++) {
                        subextr[i-1][j-1][k-1]=subex[i-1][j-1][k-1];
                    }
                }
            }
            i=1;
            k=4;
            while(i <= k) {
    //* Nuevas constantes de subdivision
                dist=0.0;
                for( j=1; j<=3; j++) {
                    bar[j-1]=(subextr[i-1][1-1][j-1]+subextr[i-1][2-1][j-1]+subextr[i-1][3-1][j-1])/3.0;
                    dist=dist+pow((ndCol[j-1]-bar[j-1]),2);
                    lado[j-1]=0.0;
                    for( m=1; m<=3; m++) {
                        lado[j-1]=lado[j-1]+pow((subextr[i-1][j+1-1][m-1]-subextr[i-1][j-1][m-1]),2);
                    }
                    lado[j-1]=sqrt(lado[j-1]);
                }
                dist=sqrt(dist);

                if((lado[1-1] > dist) || (lado[2-1] > dist) || (lado[3-1] > dist)) {
    //* Hay que volver a subdividir
                    for( j=1; j<=4; j++) {
                        for( m=1; m<=3; m++) {
                            extrs[j-1][m-1]=subextr[i-1][j-1][m-1];
                        }
                    }
                    SUBDIVIDE(extrs, subex);if(enExcepcion==1)return;

                    for( j=1; j<=4; j++) {
                        for( p=1; p<=3; p++) {
                            subextr[i-1][j-1][p-1]=subex[1-1][j-1][p-1];
                        }
                    }
                    for( m=2; m<=4; m++) {
                        for( j=1; j<=4; j++) {
                            for( p=1; p<=3; p++) {
                                subextr[k+m-1-1][j-1][p-1]=subex[m-1][j-1][p-1];
                            }
                        }
                    }
                    k=k+3;
                    if(k > 997) {
                        //printf("Superado el numero maximo de subdivisiones\n"); 
                        enExcepcion=1;return;
                    }
                }
                else            
                {
                    i=i+1;
                }
            }
            for( i=1; i<=3; i++) {
                CT[i-1]=0.0;
                for( j=1; j<=3; j++) {
                    AET[i-1][j-1]=0.0;
                    BET[i-1][j-1]=0.0;
                    DT[i-1][j-1]=0.0;
                }
            }
            ATT=0.0;
            BTT=0.0;

            for( i=1; i<=k; i++) {
                for( j=1; j<=4; j++) {
                    for( m=1; m<=3; m++) {
                        extr[j-1][m-1]=subextr[i-1][j-1][m-1];
                    }
                }
                NUMERICA(AEP,BEP,&ATP,&BTP,CP,DP,n,&distancia);if(enExcepcion==1)return;

                if((tpproE == 1) || (tpproTE == 1)) {
                    for( j=1; j<=3; j++) {
                        for( m=1; m<=3      ; m++) {
                            AET[j-1][m-1]=AET[j-1][m-1]+AEP[j-1][m-1];
                            BET[j-1][m-1]=BET[j-1][m-1]+BEP[j-1][m-1];
                        }
                    }
                }
                if((tpproT == 1) || (tpproTE == 1)) {
                    ATT=ATT+ATP;
                    BTT=BTT+BTP;
                }
                if(tpproTE == 1) {
                    for( j=1; j<=3; j++) {
                        CT[j-1]=CT[j-1]+CP[j-1];
                        for( m=1; m<=3; m++) {
                            DT[j-1][m-1]=DT[j-1][m-1]+DP[j-1][m-1];
                        }
                    }
                }
            }
        }
        else        
        {
    //* No hay que subdividir
    //write(*,*) '  no subdivision'
    //write(*,*) '   NUMÉRICA'
            NUMERICA(AET,BET,&ATT,&BTT,CT,DT,n,&distancia);if(enExcepcion==1)return;
        }
    }
    *punteroA_intenum = intenum; *punteroA_ATT = ATT; *punteroA_BTT = BTT; return;
}

void TRANSFORMA(double AETR[3][3],double BETR[3][3],int* punteroA_el) {
    //* Declaracion de variables

    int el=*punteroA_el;;   //Elemento sobre el que se ha integrado
    int  i,j;   //Auxiliares
    double ccc[3][3];/*double AETR[3][3];*  //*double BETR[3][3];*/;
    //real*8  cct(3),CTR(3),DTR(3,3)
    //* Realiza transformacion

    //* Coeficientes elásticos
    for( i=1; i<=3; i++) {
        ccc[i-1][1-1]=AETR[i-1][1-1]*locT[el-1][1-1]+AETR[i-1][2-1]*locT[el-1][2-1]+AETR[i-1][3-1]*locT[el-1][3-1];
        ccc[i-1][2-1]=AETR[i-1][1-1]*locT[el-1][4-1]+AETR[i-1][2-1]*locT[el-1][5-1]+AETR[i-1][3-1]*locT[el-1][6-1];
        ccc[i-1][3-1]=AETR[i-1][1-1]*locT[el-1][7-1]+AETR[i-1][2-1]*locT[el-1][8-1]+AETR[i-1][3-1]*locT[el-1][9-1];

    }
    for( i=1; i<=3; i++) {
        for( j=1; j<=3; j++) {
            AETR[i-1][j-1]=ccc[i-1][j-1];
        }
    }
    for( i=1; i<=3; i++) {
        ccc[i-1][1-1]=BETR[i-1][1-1]*locT[el-1][1-1]+BETR[i-1][2-1]*locT[el-1][2-1]+BETR[i-1][3-1]*locT[el-1][3-1];
        ccc[i-1][2-1]=BETR[i-1][1-1]*locT[el-1][4-1]+BETR[i-1][2-1]*locT[el-1][5-1]+BETR[i-1][3-1]*locT[el-1][6-1];
        ccc[i-1][3-1]=BETR[i-1][1-1]*locT[el-1][7-1]+BETR[i-1][2-1]*locT[el-1][8-1]+BETR[i-1][3-1]*locT[el-1][9-1];
    }
    for( i=1; i<=3; i++) {
        for( j=1; j<=3; j++) {
            BETR[i-1][j-1]=ccc[i-1][j-1];
        }
    }
    //* Coeficientes termoelásticos
    //do i=1,3
    //ccc(i,1)=DTR(i,1)*locT(el,1)+DTR(i,2)*locT(el,2)+DTR(i,3)*locT(el,3)
    //ccc(i,2)=DTR(i,1)*locT(el,4)+DTR(i,2)*locT(el,5)+DTR(i,3)*locT(el,6)
    //ccc(i,3)=DTR(i,1)*locT(el,7)+DTR(i,2)*locT(el,8)+DTR(i,3)*locT(el,9)
    //enddo
    //do i=1,3
    //do j=1,3
    //DTR(i,j)=ccc(i,j)
    //enddo
    //enddo

    //cct(1)=CTR(1)*locT(el,1)+CTR(2)*locT(el,2)+CTR(3)*locT(el,3)
    //cct(2)=CTR(1)*locT(el,4)+CTR(2)*locT(el,5)+CTR(3)*locT(el,6)
    //cct(3)=CTR(1)*locT(el,7)+CTR(2)*locT(el,8)+CTR(3)*locT(el,9)
    //do i=1,3
    //CTR(i)=cct(i)
    //enddo
    *punteroA_el = el; return;
}

void ALMACENA(int* punteroA_el,int* punteroA_nd,double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T) {
    //* Declaracion de variables

    int el=*punteroA_el;int nd=*punteroA_nd;;   //Elemento sobre el que se ha integrado y nodo
    int  i,j;   //Auxiliares

    /*double AE_T[3*nelT][3*nelT];* //*double BE_T[3*nelT][3*nelT];*/;
    /*double AT_T[nelT][nelT];* //*double BT_T[nelT][nelT];*/;
    /*double CTE_T[3*nelT][nelT];*  //*double DTE_T[3*nelT][nelT];*/;

    //* Coeficientes elasticos
    if((tpproE == 1) || (tpproTE == 1)) {
    //* SI SE DESEA PUEDEN ESCALARSE LOS COEFICIENTES Bij ANTES DEL ALMACENAJE

        int arTemp_0;int arTemp_1;int arTemp_2;int arTemp_3;arTemp_2 = 1-1; for(arTemp_0=el*3 - 2;arTemp_0<=el*3;arTemp_0+=1) {
            arTemp_2 += 1;arTemp_3 = 1-1; for(arTemp_1=nd*3 -2;arTemp_1<=nd*3;arTemp_1+=1) {
                arTemp_3 += 1;AE_T[arTemp_0-1][arTemp_1-1] = AE[arTemp_2-1][arTemp_3-1];
            }
        }
        ;
        int arTemp_4;int arTemp_5;int arTemp_6;int arTemp_7;arTemp_6 = 1-1; for(arTemp_4=el*3 - 2;arTemp_4<=el*3;arTemp_4+=1) {
            arTemp_6 += 1;arTemp_7 = 1-1; for(arTemp_5=nd*3 -2;arTemp_5<=nd*3;arTemp_5+=1) {
                arTemp_7 += 1;BE_T[arTemp_4-1][arTemp_5-1] = BE[arTemp_6-1][arTemp_7-1];
            }
        }
        ;
    //write(unit=out1,rec=reg)AE
    //write(unit=out2,rec=reg)BE

    //write(out11,*)AE(1,1),AE(1,2),AE(1,3),AE(2,1),AE(2,2),AE(2,3),AE(3,1),AE(3,2),AE(3,3)
    //write(out12,*)BE(1,1),BE(1,2),BE(1,3),BE(2,1),BE(2,2),BE(2,3),BE(3,1),BE(3,2),BE(3,3)
    }
    //* Coeficientes termicos
    if((tpproT == 1) || (tpproTE == 1)) {
        AT_T[el-1][nd-1]=AT;
        BT_T[el-1][nd-1]=BT;
    //write(unit=out3,rec=reg)AT
    //write(out13,*)AT
    //write(unit=out4,rec=reg)BT
    //write(out14,*)BT
    }
    //* Coeficientes termoelasticos
    if(tpproTE == 1) {
    //inicializa coeficiente termoelástico
        for( i=1; i<=3; i++) {
            DTTE[i-1]=0.0;
        }
    //* Calcula el coeficiente termoelástico Di=Dij*nj
        for( i=1; i<=3; i++) {
            for( j=1; j<=3; j++) {
                DTTE[i-1]=DTTE[i-1]+DTE[i-1][j-1]*locT[el-1][j-1];
            }
        }
        int arTemp_8;int arTemp_9;int arTemp_10;arTemp_10 = 1-1; for(arTemp_8=el*3 - 2;arTemp_8<=el*3;arTemp_8+=1) {
            arTemp_10 += 1;for(arTemp_9=nd;arTemp_9<=nd;arTemp_9+=1) {
                CTE_T[arTemp_8-1][arTemp_9-1] = CTE[arTemp_10-1];
            }
        }
        ;
        int arTemp_11;int arTemp_12;int arTemp_13;arTemp_13 = 1-1; for(arTemp_11=el*3 - 2;arTemp_11<=el*3;arTemp_11+=1) {
            arTemp_13 += 1;for(arTemp_12=nd;arTemp_12<=nd;arTemp_12+=1) {
                DTE_T[arTemp_11-1][arTemp_12-1] = DTTE[arTemp_13-1];
            }
        }
        ;
    //write(unit=out5,rec=reg)CTE
    //write(unit=out6,rec=reg)DTTE
    //write(out15,*)CTE
    //write(out16,*)DTTE
    }
    *punteroA_el = el; *punteroA_nd = nd; return;
}


void kernelImplicit(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T) {
    int  nd,el; //Auxiliares
    int  i,j,intenum;


    //* Para todos los elementos COMIENZA EL PRIMER ELEMENTO (SOBRE EL QUE SE INTEGRA)
    for (el=1; el<=nelT; el++) {
    //* Para todos los nodos COMIENZA EL PRIMER NODO (DESDE EL QUE SE INTEGRA)
        for (nd=1; nd<=nelT; nd++) {
    //* Asigna punto de colocacion
            for (i=1; i<=3; i++) {
                ndCol[i-1]=ndT[nd-1][i-1];
            }
    //* Asigna tipo de integral
            if(el == nd) {
                intenum = 0;
            }
            else                    
            {
                intenum = 1;
            }
    //* Integra NORMAL

    //* Asigna extremos
            for (i=1; i<=3; i++) {
                for (j=1; j<=3; j++) {
                    extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                }
            }
            INTEGRA(&intenum,AE,BE,&AT,&BT,CTE,DTE);

            if((tpproE == 1) || (tpproTE == 1)) {
    //* Cambia de coordenadas los coeficientes elasticos
                TRANSFORMA(AE,BE,&el);
            }
    //* Almacena coeficientes
            reg=(el-1)*nelT+nd;
    //if((tpproE.eq.1).or.(tpproTE.eq.1))then
    //write(out11,*)'Elemento:',nd,el
    //write(out12,*)'Elemento:',nd,el
    //endif
    //if((tpproT.eq.1).or.(tpproTE.eq.1))then
    //write(out13,*)'Elemento:',nd,el
    //write(out14,*)'Elemento:',nd,el
    //endif
    //if(tpproTE.eq.1)then
    //write(out15,*)'Elemento:',nd,el
    //write(out16,*)'Elemento:',nd,el
    //endif

            ALMACENA(&el,&nd,AE_T,BE_T,AT_T,BT_T,CTE_T,DTE_T);
        }
    }
}

void kernelOXY(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T) {
    int  nd,el; //Auxiliares
    int  i,j,intenum;
    double  extraux;
    double  AE1[3][3],AE2[3][3],AE3[3][3],AE4[3][3],AE5[3][3],AE6[3][3],AE7[3][3];
    double  BE1[3][3],BE2[3][3],BE3[3][3],BE4[3][3],BE5[3][3],BE6[3][3],BE7[3][3];
    double  AT1,AT2,AT3,AT4,AT5,AT6,AT7;
    double  BT1,BT2,BT3,BT4,BT5,BT6,BT7;
    double  C1[3],C2[3],C3[3],C4[3],C5[3],C6[3],C7[3];
    double  D1[3][3],D2[3][3],D3[3][3],D4[3][3],D5[3][3],D6[3][3],D7[3][3];

    //* Para todos los elementos COMIENZA EL PRIMER ELEMENTO (SOBRE EL QUE SE INTEGRA)
    for( el=1; el<=nelT; el++) {
    //* Para todos los nodos COMIENZA EL PRIMER NODO (DESDE EL QUE SE INTEGRA)
        for( nd=1; nd<=nelT; nd++) {
    //* Asigna punto de colocacion
            for( i=1; i<=3; i++) {
                ndCol[i-1]=ndT[nd-1][i-1];
            }
    //* Asigna tipo de integral
            if(el == nd) {
                intenum = 0;
            }
            else                    
            {
                intenum = 1;
            }
    //* Integra NORMAL

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                }
            }
            INTEGRA(&intenum,AE,BE,&AT,&BT,CTE,DTE);if(enExcepcion==1)return;

    //* Integra SIMÉTRICO OXY

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 3) {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                                
                    {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
    //* Intercambia extremos 2 y 3
            for( i=1; i<=3; i++) {
                extraux=extr[2-1][i-1];
                extr[2-1][i-1]=extr[3-1][i-1];
                extr[3-1][i-1]=extraux;
            }
    //* Asigna tipo de integral
            intenum=1;
            INTEGRA(&intenum,AE1,BE1,&AT1,&BT1,C1,D1);if(enExcepcion==1)return;

    //* Combina coeficientes
    //* Elásticos
            if((tpproE == 1) || (tpproTE == 1)) {
                for( i=1; i<=3; i++) {
                    for( j=1; j<=3; j++) {
                        if(j == 3) {
                            AE[i-1][j-1]=AE[i-1][j-1]-AE1[i-1][j-1];
                            BE[i-1][j-1]=BE[i-1][j-1]-BE1[i-1][j-1];
                        }
                        else                                 
                        {
                            AE[i-1][j-1]=AE[i-1][j-1]+AE1[i-1][j-1];
                            BE[i-1][j-1]=BE[i-1][j-1]+BE1[i-1][j-1];
                        }
                    }
                }
    //* Térmoelásticos
                if(tpproTE == 1) {
                    for( i=1; i<=3; i++) {
                        CTE[i-1]=CTE[i-1]+C1[i-1];
                        for( j=1; j<=3; j++) {
                            if(j == 3) {
                                DTE[i-1][j-1]=DTE[i-1][j-1]-D1[i-1][j-1];
                            }
                            else                                     
                            {
                                DTE[i-1][j-1]=DTE[i-1][j-1]+D1[i-1][j-1];
                            }
                        }
                    }
                }
    //* Cambia de coordenadas los coeficientes elasticos y termoelásticos
                TRANSFORMA(AE,BE,&el);if(enExcepcion==1)return;
            }
    //* Térmicos
            if((tpproT == 1) || (tpproTE == 1)) {
                AT=AT+AT1;
                BT=BT+BT1;
            }
    //* Almacena coeficientes
            reg=(el-1)*nelT+nd;
    //if((tpproE.eq.1).or.(tpproTE.eq.1))then
    //write(out11,*)'Elemento:',nd,el
    //write(out12,*)'Elemento:',nd,el
    //endif
    //if((tpproT.eq.1).or.(tpproTE.eq.1))then
    //write(out13,*)'Elemento:',nd,el
    //write(out14,*)'Elemento:',nd,el
    //endif
    //if(tpproTE.eq.1)then
    //write(out15,*)'Elemento:',nd,el
    //write(out16,*)'Elemento:',nd,el
    //endif

            ALMACENA(&el,&nd,AE_T,BE_T,AT_T,BT_T,CTE_T,DTE_T);if(enExcepcion==1)return;
        }
    }
}

void kernelOXZ(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T) {
    int  nd,el; //Auxiliares
    int  i,j,intenum;
    double  extraux;
    double  AE1[3][3],AE2[3][3],AE3[3][3],AE4[3][3],AE5[3][3],AE6[3][3],AE7[3][3];
    double  BE1[3][3],BE2[3][3],BE3[3][3],BE4[3][3],BE5[3][3],BE6[3][3],BE7[3][3];
    double  AT1,AT2,AT3,AT4,AT5,AT6,AT7;
    double  BT1,BT2,BT3,BT4,BT5,BT6,BT7;
    double  C1[3],C2[3],C3[3],C4[3],C5[3],C6[3],C7[3];
    double  D1[3][3],D2[3][3],D3[3][3],D4[3][3],D5[3][3],D6[3][3],D7[3][3];

    //* Para todos los elementos COMIENZA EL PRIMER ELEMENTO (SOBRE EL QUE SE INTEGRA)
    for( el=1; el<=nelT; el++) {
    //* Para todos los nodos COMIENZA EL PRIMER NODO (DESDE EL QUE SE INTEGRA)
        for( nd=1; nd<=nelT; nd++) {
    //* Asigna punto de colocacion
            for( i=1; i<=3; i++) {
                ndCol[i-1]=ndT[nd-1][i-1];
            }
    //* Asigna tipo de integral
            if(el == nd) {
                intenum = 0;
            }
            else                    
            {
                intenum = 1;
            }
    //* Integra NORMAL

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                }
            }
            INTEGRA(&intenum,AE,BE,&AT,&BT,CTE,DTE);if(enExcepcion==1)return;

    //* Integra simetrico Oxz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 2) {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
    //* Intercambia extremos 2 y 3
            for( i=1; i<=3; i++) {
                extraux=extr[2-1][i-1];
                extr[2-1][i-1]=extr[3-1][i-1];
                extr[3-1][i-1]=extraux;
            }
    //* Asigna tipo de integral
            intenum=1;
            INTEGRA(&intenum,AE1,BE1,&AT1,&BT1,C1,D1);if(enExcepcion==1)return;

    //* Elásticos
    //* Combina coeficientes
            if((tpproE == 1) || (tpproTE == 1)) {
                for( i=1; i<=3; i++) {
                    for( j=1; j<=3; j++) {
                        if(j == 2) {
                            AE[i-1][j-1]=AE[i-1][j-1]-AE1[i-1][j-1];
                            BE[i-1][j-1]=BE[i-1][j-1]-BE1[i-1][j-1];
                        }
                        else                                 
                        {
                            AE[i-1][j-1]=AE[i-1][j-1]+AE1[i-1][j-1];
                            BE[i-1][j-1]=BE[i-1][j-1]+BE1[i-1][j-1];
                        }
                    }
                }
    //* Térmoelásticos
                if(tpproTE == 1) {
                    for( i=1; i<=3; i++) {
                        CTE[i-1]=CTE[i-1]+C1[i-1];
                        for( j=1; j<=3; j++) {
                            if(j == 2) {
                                DTE[i-1][j-1]=DTE[i-1][j-1]-D1[i-1][j-1];
                            }
                            else                                    
                            {
                                DTE[i-1][j-1]=DTE[i-1][j-1]+D1[i-1][j-1];
                            }
                        }
                    }
                }
    //* Cambia de coordenadas los coeficientes elasticos
                TRANSFORMA(AE,BE,&el);if(enExcepcion==1)return;
            }
    //* Térmicos
            if((tpproT == 1) || (tpproTE == 1)) {
                AT=AT+AT1;
                BT=BT+BT1;
            }
    //* Almacena coeficientes
            reg=(el-1)*nelT+nd;
    //if((tpproE.eq.1).or.(tpproTE.eq.1))then
    //write(out11,*)'Elemento:',nd,el
    //write(out12,*)'Elemento:',nd,el
    //endif
    //if((tpproT.eq.1).or.(tpproTE.eq.1))then
    //write(out13,*)'Elemento:',nd,el
    //write(out14,*)'Elemento:',nd,el
    //endif
    //if(tpproTE.eq.1)then
    //write(out15,*)'Elemento:',nd,el
    //write(out16,*)'Elemento:',nd,el
    //endif

            ALMACENA(&el,&nd,AE_T,BE_T,AT_T,BT_T,CTE_T,DTE_T);if(enExcepcion==1)return;
        }
    }
}

void kernelOYZ(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T) {
    int  nd,el; //Auxiliares
    int  i,j,intenum;
    double  extraux;
    double  AE1[3][3],AE2[3][3],AE3[3][3],AE4[3][3],AE5[3][3],AE6[3][3],AE7[3][3];
    double  BE1[3][3],BE2[3][3],BE3[3][3],BE4[3][3],BE5[3][3],BE6[3][3],BE7[3][3];
    double  AT1,AT2,AT3,AT4,AT5,AT6,AT7;
    double  BT1,BT2,BT3,BT4,BT5,BT6,BT7;
    double  C1[3],C2[3],C3[3],C4[3],C5[3],C6[3],C7[3];
    double  D1[3][3],D2[3][3],D3[3][3],D4[3][3],D5[3][3],D6[3][3],D7[3][3];

    //* Para todos los elementos COMIENZA EL PRIMER ELEMENTO (SOBRE EL QUE SE INTEGRA)
    for( el=1; el<=nelT; el++) {
    //* Para todos los nodos COMIENZA EL PRIMER NODO (DESDE EL QUE SE INTEGRA)
        for( nd=1; nd<=nelT; nd++) {
    //* Asigna punto de colocacion
            for( i=1; i<=3; i++) {
                ndCol[i-1]=ndT[nd-1][i-1];
            }
    //* Asigna tipo de integral
            if(el == nd) {
                intenum = 0;
            }
            else                    
            {
                intenum = 1;
            }
    //* Integra NORMAL

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                }
            }
            INTEGRA(&intenum,AE,BE,&AT,&BT,CTE,DTE);if(enExcepcion==1)return;

    //* Integra simetrico Oyz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 1) {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
    //* Intercambia extremos 2 y 3
            for( i=1; i<=3; i++) {
                extraux=extr[2-1][i-1];
                extr[2-1][i-1]=extr[3-1][i-1];
                extr[3-1][i-1]=extraux;
            }
            intenum=1;
            INTEGRA(&intenum,AE1,BE1,&AT1,&BT1,C1,D1);if(enExcepcion==1)return;

    //* Elasticos
    //* Combina coeficientes
            if((tpproE == 1) || (tpproTE == 1)) {
                for( i=1; i<=3; i++) {
                    for( j=1; j<=3; j++) {
                        if(j == 1) {
                            AE[i-1][j-1]=AE[i-1][j-1]-AE1[i-1][j-1];
                            BE[i-1][j-1]=BE[i-1][j-1]-BE1[i-1][j-1];
                        }
                        else                                 
                        {
                            AE[i-1][j-1]=AE[i-1][j-1]+AE1[i-1][j-1];
                            BE[i-1][j-1]=BE[i-1][j-1]+BE1[i-1][j-1];
                        }
                    }
                }
            }
    //* Térmoelásticos
            if(tpproTE == 1) {
                for( i=1; i<=3; i++) {
                    CTE[i-1]=CTE[i-1]+C1[i-1];
                    for( j=1; j<=3; j++) {
                        if(j == 1) {
                            DTE[i-1][j-1]=DTE[i-1][j-1]-D1[i-1][j-1];
                        }
                        else                                 
                        {
                            DTE[i-1][j-1]=DTE[i-1][j-1]+D1[i-1][j-1];
                        }
                    }
                }
            }
    //* Cambia de coordenadas los coeficientes elasticos
            TRANSFORMA(AE,BE,&el);if(enExcepcion==1)return;

    //* Térmicos
            if((tpproT == 1) || (tpproTE == 1)) {
                AT=AT+AT1;
                BT=BT+BT1;
            }
    //* Almacena coeficientes
            reg=(el-1)*nelT+nd;
    //if((tpproE.eq.1).or.(tpproTE.eq.1))then
    //write(out11,*)'Elemento:',nd,el
    //write(out12,*)'Elemento:',nd,el
    //endif
    //if((tpproT.eq.1).or.(tpproTE.eq.1))then
    //write(out13,*)'Elemento:',nd,el
    //write(out14,*)'Elemento:',nd,el
    //endif
    //if(tpproTE.eq.1)then
    //write(out15,*)'Elemento:',nd,el
    //write(out16,*)'Elemento:',nd,el
    //endif

            ALMACENA(&el,&nd,AE_T,BE_T,AT_T,BT_T,CTE_T,DTE_T);if(enExcepcion==1)return;
        }
    }
}

void kernelOXYOXZ(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T) {
    int  nd,el; //Auxiliares
    int  i,j,intenum;
    double  extraux;
    double  AE1[3][3],AE2[3][3],AE3[3][3],AE4[3][3],AE5[3][3],AE6[3][3],AE7[3][3];
    double  BE1[3][3],BE2[3][3],BE3[3][3],BE4[3][3],BE5[3][3],BE6[3][3],BE7[3][3];
    double  AT1,AT2,AT3,AT4,AT5,AT6,AT7;
    double  BT1,BT2,BT3,BT4,BT5,BT6,BT7;
    double  C1[3],C2[3],C3[3],C4[3],C5[3],C6[3],C7[3];
    double  D1[3][3],D2[3][3],D3[3][3],D4[3][3],D5[3][3],D6[3][3],D7[3][3];


    //* Para todos los elementos COMIENZA EL PRIMER ELEMENTO (SOBRE EL QUE SE INTEGRA)
    for( el=1; el<=nelT; el++) {
    //* Para todos los nodos COMIENZA EL PRIMER NODO (DESDE EL QUE SE INTEGRA)
        for( nd=1; nd<=nelT; nd++) {
    //* Asigna punto de colocacion
            for( i=1; i<=3; i++) {
                ndCol[i-1]=ndT[nd-1][i-1];
            }
    //* Asigna tipo de integral
            if(el == nd) {
                intenum = 0;
            }
            else                    
            {
                intenum = 1;
            }
    //* Integra NORMAL

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                }
            }
            INTEGRA(&intenum,AE,BE,&AT,&BT,CTE,DTE);if(enExcepcion==1)return;

    //* Integra simetrico Oxy

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 3) {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
    //* Intercambia extremos 2 y 3
            for( i=1; i<=3; i++) {
                extraux=extr[2-1][i-1];
                extr[2-1][i-1]=extr[3-1][i-1];
                extr[3-1][i-1]=extraux;
            }
    //* Asigna tipo de integral
            intenum=1;
            INTEGRA(&intenum,AE1,BE1,&AT1,&BT1,C1,D1);if(enExcepcion==1)return;

    //* Integra simetrico Oxy y Oxz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 1) {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
            INTEGRA(&intenum,AE2,BE2,&AT2,&BT2,C2,D2);if(enExcepcion==1)return;

    //* Integra simetrico Oxz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 2) {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
            INTEGRA(&intenum,AE3,BE3,&AT3,&BT3,C3,D3);if(enExcepcion==1)return;

    //* Elasticos
    //* Combina coeficientes
            if((tpproE == 1) || (tpproTE == 1)) {
                for( i=1; i<=3; i++) {
                    AE[i-1][1-1]=AE[i-1][1-1]+AE1[i-1][1-1]+AE2[i-1][1-1]+AE3[i-1][1-1];
                    AE[i-1][2-1]=AE[i-1][2-1]+AE1[i-1][2-1]-AE2[i-1][2-1]-AE3[i-1][2-1];
                    AE[i-1][3-1]=AE[i-1][3-1]-AE1[i-1][3-1]-AE2[i-1][3-1]+AE3[i-1][3-1];
                    BE[i-1][1-1]=BE[i-1][1-1]+BE1[i-1][1-1]+BE2[i-1][1-1]+BE3[i-1][1-1];
                    BE[i-1][2-1]=BE[i-1][2-1]+BE1[i-1][2-1]-BE2[i-1][2-1]-BE3[i-1][2-1];
                    BE[i-1][3-1]=BE[i-1][3-1]-BE1[i-1][3-1]-BE2[i-1][3-1]+BE3[i-1][3-1];
                }
            }
    //* Térmoelásticos
            if(tpproTE == 1) {
                for( i=1; i<=3; i++) {
                    DTE[i-1][1-1]=DTE[i-1][1-1]+D1[i-1][1-1]+D2[i-1][1-1]+D3[i-1][1-1];
                    DTE[i-1][2-1]=DTE[i-1][2-1]+D1[i-1][2-1]-D2[i-1][2-1]-D3[i-1][2-1];
                    DTE[i-1][3-1]=DTE[i-1][3-1]-D1[i-1][3-1]-D2[i-1][3-1]+D3[i-1][3-1];
                }
            }
    //* Cambia de coordenadas los coeficientes elasticos
            TRANSFORMA(AE,BE,&el);if(enExcepcion==1)return;
    //* Térmicos
            if((tpproT == 1) || (tpproTE == 1)) {
                AT=AT+AT1+AT2+AT3;
                BT=BT+BT1+BT2+BT3;
            }
    //* Almacena coeficientes
            reg=(el-1)*nelT+nd;
    //if((tpproE.eq.1).or.(tpproTE.eq.1))then
    //write(out11,*)'Elemento:',nd,el
    //write(out12,*)'Elemento:',nd,el
    //endif
    //if((tpproT.eq.1).or.(tpproTE.eq.1))then
    //write(out13,*)'Elemento:',nd,el
    //write(out14,*)'Elemento:',nd,el
    //endif
    //if(tpproTE.eq.1)then
    //write(out15,*)'Elemento:',nd,el
    //write(out16,*)'Elemento:',nd,el
    //endif

            ALMACENA(&el,&nd,AE_T,BE_T,AT_T,BT_T,CTE_T,DTE_T);if(enExcepcion==1)return;
        }
    }
}

void kernelOXYOYZ(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T) {
    int  nd,el; //Auxiliares
    int  i,j,intenum;
    double  extraux;
    double  AE1[3][3],AE2[3][3],AE3[3][3],AE4[3][3],AE5[3][3],AE6[3][3],AE7[3][3];
    double  BE1[3][3],BE2[3][3],BE3[3][3],BE4[3][3],BE5[3][3],BE6[3][3],BE7[3][3];
    double  AT1,AT2,AT3,AT4,AT5,AT6,AT7;
    double  BT1,BT2,BT3,BT4,BT5,BT6,BT7;
    double  C1[3],C2[3],C3[3],C4[3],C5[3],C6[3],C7[3];
    double  D1[3][3],D2[3][3],D3[3][3],D4[3][3],D5[3][3],D6[3][3],D7[3][3];

    //* Para todos los elementos COMIENZA EL PRIMER ELEMENTO (SOBRE EL QUE SE INTEGRA)
    for( el=1; el<=nelT; el++) {
    //* Para todos los nodos COMIENZA EL PRIMER NODO (DESDE EL QUE SE INTEGRA)
        for( nd=1; nd<=nelT; nd++) {
    //* Asigna punto de colocacion
            for( i=1; i<=3; i++) {
                ndCol[i-1]=ndT[nd-1][i-1];
            }
    //* Asigna tipo de integral
            if(el == nd) {
                intenum = 0;
            }
            else                    
            {
                intenum = 1;
            }
    //* Integra NORMAL

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                }
            }
            INTEGRA(&intenum,AE,BE,&AT,&BT,CTE,DTE);if(enExcepcion==1)return;

    //* Integra simetrico Oxy

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 3) {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
    //* Intercambia extremos 2 y 3
            for( i=1; i<=3; i++) {
                extraux=extr[2-1][i-1];
                extr[2-1][i-1]=extr[3-1][i-1];
                extr[3-1][i-1]=extraux;
            }
    //* Asigna tipo de integral
            intenum=1;
            INTEGRA(&intenum,AE1,BE1,&AT1,&BT1,C1,D1);if(enExcepcion==1)return;

    //* Integra simetrico Oxy y Oyz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 2) {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
            INTEGRA(&intenum,AE2,BE2,&AT2,&BT2,C2,D2);if(enExcepcion==1)return;

    //* Integra simetrico Oyz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 1) {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
    //* Intercambia extremos 2 y 3
            for( i=1; i<=3; i++) {
                extraux=extr[2-1][i-1];
                extr[2-1][i-1]=extr[3-1][i-1];
                extr[3-1][i-1]=extraux;
            }
            INTEGRA(&intenum,AE3,BE3,&AT3,&BT3,C3,D3);if(enExcepcion==1)return;

    //* Elasticos
    //* Combina coeficientes
            if((tpproE == 1) || (tpproTE == 1)) {
                for( i=1; i<=3; i++) {
                    AE[i-1][1-1]=AE[i-1][1-1]+AE1[i-1][1-1]-AE2[i-1][1-1]-AE3[i-1][1-1];
                    AE[i-1][2-1]=AE[i-1][2-1]+AE1[i-1][2-1]+AE2[i-1][2-1]+AE3[i-1][2-1];
                    AE[i-1][3-1]=AE[i-1][3-1]-AE1[i-1][3-1]-AE2[i-1][3-1]+AE3[i-1][3-1];
                    BE[i-1][1-1]=BE[i-1][1-1]+BE1[i-1][1-1]-BE2[i-1][1-1]-BE3[i-1][1-1];
                    BE[i-1][2-1]=BE[i-1][2-1]+BE1[i-1][2-1]+BE2[i-1][2-1]+BE3[i-1][2-1];
                    BE[i-1][3-1]=BE[i-1][3-1]-BE1[i-1][3-1]-BE2[i-1][3-1]+BE3[i-1][3-1];
                }
            }
    //* Térmoelásticos
            if(tpproTE == 1) {
                CTE[1-1]=CTE[1-1]+C1[1-1]+C2[1-1]+C3[1-1];
                CTE[2-1]=CTE[2-1]+C1[2-1]+C2[2-1]+C3[2-1];
                CTE[3-1]=CTE[3-1]+C1[3-1]+C2[3-1]+C3[3-1];
                for( i=1; i<=3; i++) {
                    DTE[i-1][1-1]=DTE[i-1][1-1]+D1[i-1][1-1]-D2[i-1][1-1]-D3[i-1][1-1];
                    DTE[i-1][2-1]=DTE[i-1][2-1]+D1[i-1][2-1]+D2[i-1][2-1]+D3[i-1][2-1];
                    DTE[i-1][3-1]=DTE[i-1][3-1]-D1[i-1][3-1]-D2[i-1][3-1]+D3[i-1][3-1];
                }
            }
    //* Cambia de coordenadas los coeficientes elasticos
            TRANSFORMA(AE,BE,&el);if(enExcepcion==1)return;
    //* Térmicos
            if((tpproT == 1) || (tpproTE == 1)) {
                AT=AT+AT1+AT2+AT3;
                BT=BT+BT1+BT2+BT3;
            }
    //* Almacena coeficientes
            reg=(el-1)*nelT+nd;
    //if((tpproE.eq.1).or.(tpproTE.eq.1))then
    //write(out11,*)'Elemento:',nd,el
    //write(out12,*)'Elemento:',nd,el
    //endif
    //if((tpproT.eq.1).or.(tpproTE.eq.1))then
    //write(out13,*)'Elemento:',nd,el
    //write(out14,*)'Elemento:',nd,el
    //endif
    //if(tpproTE.eq.1)then
    //write(out15,*)'Elemento:',nd,el
    //write(out16,*)'Elemento:',nd,el
    //endif

            ALMACENA(&el,&nd,AE_T,BE_T,AT_T,BT_T,CTE_T,DTE_T);if(enExcepcion==1)return;
        }
    }
}

void kernelOXZOYZ(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T) {
    int  nd,el; //Auxiliares
    int  i,j,intenum;
    double  extraux;
    double  AE1[3][3],AE2[3][3],AE3[3][3],AE4[3][3],AE5[3][3],AE6[3][3],AE7[3][3];
    double  BE1[3][3],BE2[3][3],BE3[3][3],BE4[3][3],BE5[3][3],BE6[3][3],BE7[3][3];
    double  AT1,AT2,AT3,AT4,AT5,AT6,AT7;
    double  BT1,BT2,BT3,BT4,BT5,BT6,BT7;
    double  C1[3],C2[3],C3[3],C4[3],C5[3],C6[3],C7[3];
    double  D1[3][3],D2[3][3],D3[3][3],D4[3][3],D5[3][3],D6[3][3],D7[3][3];

    //* Para todos los elementos COMIENZA EL PRIMER ELEMENTO (SOBRE EL QUE SE INTEGRA)
    for( el=1; el<=nelT; el++) {
    //* Para todos los nodos COMIENZA EL PRIMER NODO (DESDE EL QUE SE INTEGRA)
        for( nd=1; nd<=nelT; nd++) {
    //* Asigna punto de colocacion
            for( i=1; i<=3; i++) {
                ndCol[i-1]=ndT[nd-1][i-1];
            }
    //* Asigna tipo de integral
            if(el == nd) {
                intenum = 0;
            }
            else                    
            {
                intenum = 1;
            }
    //* Integra NORMAL

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                }
            }
            INTEGRA(&intenum,AE,BE,&AT,&BT,CTE,DTE);if(enExcepcion==1)return;
    //if ((nd.eq.1).and.(el.eq.129))then
    //write(out11,*) 'AE',AE(1,1),AE(1,2),AE(1,3),AE(2,1),AE(2,2),AE(2,3),AE(3,1),AE(3,2),AE(3,3)
    //endif


    //* Integra simetrico Oyz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 1) {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
    //* Intercambia extremos 2 y 3
            for( i=1; i<=3; i++) {
                extraux=extr[2-1][i-1];
                extr[2-1][i-1]=extr[3-1][i-1];
                extr[3-1][i-1]=extraux;
            }
    //* Asigna tipo de integral
            intenum=1;
            INTEGRA(&intenum,AE1,BE1,&AT1,&BT1,C1,D1);if(enExcepcion==1)return;
    //if ((nd.eq.1).and.(el.eq.129))then
    //write(out11,*) 'AE1',AE1(1,1),AE1(1,2),AE1(1,3),AE1(2,1),AE1(2,2),AE1(2,3),AE1(3,1),AE1(3,2),AE1(3,3)
    //endif


    //* Integra simetrico Oxz y Oyz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 3) {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
            INTEGRA(&intenum,AE2,BE2,&AT2,&BT2,C2,D2);if(enExcepcion==1)return;
    //if ((nd.eq.1).and.(el.eq.129))then
    //write(out11,*) 'AE2',AE2(1,1),AE2(1,2),AE2(1,3),AE2(2,1),AE2(2,2),AE2(2,3),AE2(3,1),AE2(3,2),AE2(3,3)
    //endif


    //* Integra simetrico Oxz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 2) {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
    //* Intercambia extremos 2 y 3
            for( i=1; i<=3; i++) {
                extraux=extr[2-1][i-1];
                extr[2-1][i-1]=extr[3-1][i-1];
                extr[3-1][i-1]=extraux;
            }
            INTEGRA(&intenum,AE3,BE3,&AT3,&BT3,C3,D3);if(enExcepcion==1)return;
    //if ((nd.eq.1).and.(el.eq.129))then
    //write(out11,*) 'AE3',AE3(1,1),AE3(1,2),AE3(1,3),AE3(2,1),AE3(2,2),AE3(2,3),AE3(3,1),AE3(3,2),AE3(3,3)
    //endif


    //* Elasticos
    //* Combina coeficientes
            if((tpproE == 1) || (tpproTE == 1)) {
                for( i=1; i<=3; i++) {
                    AE[i-1][1-1]=AE[i-1][1-1]-AE1[i-1][1-1]-AE2[i-1][1-1]+AE3[i-1][1-1];
                    AE[i-1][2-1]=AE[i-1][2-1]+AE1[i-1][2-1]-AE2[i-1][2-1]-AE3[i-1][2-1];
                    AE[i-1][3-1]=AE[i-1][3-1]+AE1[i-1][3-1]+AE2[i-1][3-1]+AE3[i-1][3-1];
                    BE[i-1][1-1]=BE[i-1][1-1]-BE1[i-1][1-1]-BE2[i-1][1-1]+BE3[i-1][1-1];
                    BE[i-1][2-1]=BE[i-1][2-1]+BE1[i-1][2-1]-BE2[i-1][2-1]-BE3[i-1][2-1];
                    BE[i-1][3-1]=BE[i-1][3-1]+BE1[i-1][3-1]+BE2[i-1][3-1]+BE3[i-1][3-1];
                }
            }
    //* Térmoelásticos
            if(tpproTE == 1) {
                CTE[1-1]=CTE[1-1]+C1[1-1]+C2[1-1]+C3[1-1];
                CTE[2-1]=CTE[2-1]+C1[2-1]+C2[2-1]+C3[2-1];
                CTE[3-1]=CTE[3-1]+C1[3-1]+C2[3-1]+C3[3-1];
                for( i=1; i<=3; i++) {
                    DTE[i-1][1-1]=DTE[i-1][1-1]-D1[i-1][1-1]-D2[i-1][1-1]+D3[i-1][1-1];
                    DTE[i-1][2-1]=DTE[i-1][2-1]+D1[i-1][2-1]-D2[i-1][2-1]-D3[i-1][2-1];
                    DTE[i-1][3-1]=DTE[i-1][3-1]+D1[i-1][3-1]+D2[i-1][3-1]+D3[i-1][3-1];
                }
            }
    //if ((nd.eq.1).and.(el.eq.129))then
    //write(out11,*) 'combina',AE(1,1),AE(1,2),AE(1,3),AE(2,1),AE(2,2),AE(2,3),AE(3,1),AE(3,2),AE(3,3)
    //endif
    //* Cambia de coordenadas los coeficientes elasticos
            TRANSFORMA(AE,BE,&el);if(enExcepcion==1)return;

    //if ((nd.eq.1).and.(el.eq.129))then
    //write(out11,*) 'transforma',AE(1,1),AE(1,2),AE(1,3),AE(2,1),AE(2,2),AE(2,3),AE(3,1),AE(3,2),AE(3,3)
    //endif
    //* Termicos
            if((tpproT == 1) || (tpproTE == 1)) {
                AT=AT+AT1+AT2+AT3;
                BT=BT+BT1+BT2+BT3;
            }
    //* Almacena coeficientes
            reg=(el-1)*nelT+nd;
            if(nd == 1) {
                //printf("Elemento:%7d%7d\n",nd,el);
            }
    //if((tpproE.eq.1).or.(tpproTE.eq.1))then
    //write(out11,*)'Elemento:',nd,el
    //write(out12,*)'Elemento:',nd,el
    //endif

    //if((tpproT.eq.1).or.(tpproTE.eq.1))then
    //write(out13,*)'Elemento:',nd,el
    //write(out14,*)'Elemento:',nd,el
    //endif
    //if(tpproTE.eq.1)then
    //write(out15,*)'Elemento:',nd,el
    //write(out16,*)'Elemento:',nd,el
    //endif

            ALMACENA(&el,&nd,AE_T,BE_T,AT_T,BT_T,CTE_T,DTE_T);if(enExcepcion==1)return;
        }
    }
}

void kernelAll(double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T) {
    int  nd,el; //Auxiliares
    int  i,j,intenum;
    double  extraux;
    double  AE1[3][3],AE2[3][3],AE3[3][3],AE4[3][3],AE5[3][3],AE6[3][3],AE7[3][3];
    double  BE1[3][3],BE2[3][3],BE3[3][3],BE4[3][3],BE5[3][3],BE6[3][3],BE7[3][3];
    double  AT1,AT2,AT3,AT4,AT5,AT6,AT7;
    double  BT1,BT2,BT3,BT4,BT5,BT6,BT7;
    double  C1[3],C2[3],C3[3],C4[3],C5[3],C6[3],C7[3];
    double  D1[3][3],D2[3][3],D3[3][3],D4[3][3],D5[3][3],D6[3][3],D7[3][3];

    //* Para todos los elementos COMIENZA EL PRIMER ELEMENTO (SOBRE EL QUE SE INTEGRA)
    for( el=1; el<=nelT; el++) {
    //* Para todos los nodos COMIENZA EL PRIMER NODO (DESDE EL QUE SE INTEGRA)
        for( nd=1; nd<=nelT; nd++) {
    //* Asigna punto de colocacion
            for( i=1; i<=3; i++) {
                ndCol[i-1]=ndT[nd-1][i-1];
            }
    //* Asigna tipo de integral
            if(el == nd) {
                intenum = 0;
            }
            else                    
            {
                intenum = 1;
            }
    //* Integra NORMAL

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                }
            }
            INTEGRA(&intenum,AE,BE,&AT,&BT,CTE,DTE);if(enExcepcion==1)return;

    //* Integra simetrico Oxy

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 3) {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
    //* Intercambia extremos 2 y 3
            for( i=1; i<=3; i++) {
                extraux=extr[2-1][i-1];
                extr[2-1][i-1]=extr[3-1][i-1];
                extr[3-1][i-1]=extraux;
            }
    //* Asigna tipo de integral
            intenum = 1;
            INTEGRA(&intenum,AE1,BE1,&AT1,&BT1,C1,D1);if(enExcepcion==1)return;

    //* Integra simetrico Oxy y Oxz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 1) {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
            INTEGRA(&intenum,AE2,BE2,&AT2,&BT2,C2,D2);if(enExcepcion==1)return;

    //* Integra simetrico Oxy,Oxz y Oyz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                }
            }
    //* Intercambia extremos 2 y 3
            for( i=1; i<=3; i++) {
                extraux=extr[2-1][i-1];
                extr[2-1][i-1]=extr[3-1][i-1];
                extr[3-1][i-1]=extraux;
            }
            INTEGRA(&intenum,AE3,BE3,&AT3,&BT3,C3,D3);if(enExcepcion==1)return;

    //* Integra simetrico Oxz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 2) {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
    //* Intercambia extremos 2 y 3
            for( i=1; i<=3; i++) {
                extraux=extr[2-1][i-1];
                extr[2-1][i-1]=extr[3-1][i-1];
                extr[3-1][i-1]=extraux;
            }
            INTEGRA(&intenum,AE4,BE4,&AT4,&BT4,C4,D4);if(enExcepcion==1)return;

    //* Integra simetrico Oxz y Oyz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 3) {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
            INTEGRA(&intenum,AE5,BE5,&AT5,&BT5,C5,D5);if(enExcepcion==1)return;

    //* Integra simetrico Oyz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 1) {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
    //* Intercambia extremos 2 y 3
            for( i=1; i<=3; i++) {
                extraux=extr[2-1][i-1];
                extr[2-1][i-1]=extr[3-1][i-1];
                extr[3-1][i-1]=extraux;
            }
            INTEGRA(&intenum,AE6,BE6,&AT6,&BT6,C6,D6);if(enExcepcion==1)return;

    //* Integra simetrico Oxy y Oyz

    //* Asigna extremos
            for( i=1; i<=3; i++) {
                for( j=1; j<=3; j++) {
                    if(j == 2) {
                        extr[i-1][j-1]=exT[conT[el-1][i-1]-1][j-1];
                    }
                    else                              
                    {
                        extr[i-1][j-1]=-exT[conT[el-1][i-1]-1][j-1];
                    }
                }
            }
            INTEGRA(&intenum,AE7,BE7,&AT7,&BT7,C7,D7);if(enExcepcion==1)return;

    //* Elasticos
    //* Combina coeficientes
            if((tpproE == 1) || (tpproTE == 1)) {
                for( i=1; i<=3; i++) {
                    AE[i-1][1-1]=AE[i-1][1-1]+AE1[i-1][1-1]+AE2[i-1][1-1]-AE3[i-1][1-1]+AE4[i-1][1-1]-AE5[i-1][1-1]-AE6[i-1][1-1]-AE7[i-1][1-1];
                    AE[i-1][2-1]=AE[i-1][2-1]+AE1[i-1][2-1]-AE2[i-1][2-1]-AE3[i-1][2-1]-AE4[i-1][2-1]-AE5[i-1][2-1]+AE6[i-1][2-1]+AE7[i-1][2-1];
                    AE[i-1][3-1]=AE[i-1][3-1]-AE1[i-1][3-1]-AE2[i-1][3-1]-AE3[i-1][3-1]+AE4[i-1][3-1]+AE5[i-1][3-1]+AE6[i-1][3-1]-AE7[i-1][3-1];
                    BE[i-1][1-1]=BE[i-1][1-1]+BE1[i-1][1-1]+BE2[i-1][1-1]-BE3[i-1][1-1]+BE4[i-1][1-1]-BE5[i-1][1-1]-BE6[i-1][1-1]-BE7[i-1][1-1];
                    BE[i-1][2-1]=BE[i-1][2-1]+BE1[i-1][2-1]-BE2[i-1][2-1]-BE3[i-1][2-1]-BE4[i-1][2-1]-BE5[i-1][2-1]+BE6[i-1][2-1]+BE7[i-1][2-1];
                    BE[i-1][3-1]=BE[i-1][3-1]-BE1[i-1][3-1]-BE2[i-1][3-1]-BE3[i-1][3-1]+BE4[i-1][3-1]+BE5[i-1][3-1]+BE6[i-1][3-1]-BE7[i-1][3-1];
                }
            }
    //* Térmoelásticos
            if(tpproTE == 1) {
                CTE[1-1]=CTE[1-1]+C1[1-1]+C2[1-1]+C3[1-1]+C4[1-1]+C5[1-1]+C6[1-1]+C7[1-1];
                CTE[2-1]=CTE[2-1]+C1[2-1]+C2[2-1]+C3[2-1]+C4[2-1]+C5[2-1]+C6[2-1]+C7[2-1];
                CTE[3-1]=CTE[3-1]+C1[3-1]+C2[3-1]+C3[3-1]+C4[3-1]+C5[3-1]+C6[3-1]+C7[3-1];
                for( i=1; i<=3; i++) {
                    DTE[i-1][1-1]=DTE[i-1][1-1]+D1[i-1][1-1]+D2[i-1][1-1]-D3[i-1][1-1]+D4[i-1][1-1]-D5[i-1][1-1]-D6[i-1][1-1]-D7[i-1][1-1];
                    DTE[i-1][2-1]=DTE[i-1][2-1]+D1[i-1][2-1]-D2[i-1][2-1]-D3[i-1][2-1]-D4[i-1][1-1]-D5[i-1][1-1]+D6[i-1][1-1]+D7[i-1][1-1];
                    DTE[i-1][3-1]=DTE[i-1][3-1]-D1[i-1][3-1]-D2[i-1][3-1]-D3[i-1][3-1]+D4[i-1][1-1]+D5[i-1][1-1]+D6[i-1][1-1]-D7[i-1][1-1];
                }
            }
    //* Cambia de coordenadas los coeficientes elasticos
            TRANSFORMA(AE,BE,&el);if(enExcepcion==1)return;
    //* Termicos
            if((tpproT == 1) || (tpproTE == 1)) {
                AT=AT+AT1+AT2+AT3+AT4+AT5+AT6+AT7;
                BT=BT+BT1+BT2+BT3+BT4+BT5+BT6+BT7;
            }
    //* Almacena coeficientes
            reg=(el-1)*nelT+nd;
    //if((tpproE.eq.1).or.(tpproTE.eq.1))then
    //write(out11,*)'Elemento:',nd,el
    //write(out12,*)'Elemento:',nd,el
    //endif
    //if((tpproT.eq.1).or.(tpproTE.eq.1))then
    //write(out13,*)'Elemento:',nd,el
    //write(out14,*)'Elemento:',nd,el
    //endif
    //if(tpproTE.eq.1)then
    //write(out15,*)'Elemento:',nd,el
    //write(out16,*)'Elemento:',nd,el
    //endif

            ALMACENA(&el,&nd,AE_T,BE_T,AT_T,BT_T,CTE_T,DTE_T);if(enExcepcion==1)return;
        }
    }
}
