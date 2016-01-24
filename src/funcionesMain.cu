#include "funcionesMain.h"

int ObtenerSimetria(EntradaCuerpo* entradaCuerpo)
{
    // Buscamos la simetria
    int simetria;

    int&  simXY = entradaCuerpo->simXY;// Flag de simetría respecto del plano xOy
    int&  simXZ = entradaCuerpo->simXZ;// Flag de simetría respecto del plano xOz
    int&  simYZ = entradaCuerpo->simYZ;// Flag de simetría respecto del plano yOz


    if ((simXY != 1) && (simXZ != 1) && (simYZ != 1))
        simetria = NO_SIMETRIA_IMPLICITA;
    else if ((simXY == 1) && (simXZ != 1) && (simYZ != 1))
        simetria = SIMETRIA_PLANO_0XY;
    else if ((simXY != 1) && (simXZ == 1) && (simYZ != 1))
        simetria = SIMETRIA_PLANO_0XZ;
    else if ((simXY != 1) && (simXZ != 1) && (simYZ == 1))
        simetria = SIMETRIA_PLANO_0YZ;
    else if ((simXY == 1) && (simXZ == 1) && (simYZ != 1))
        simetria = SIMETRIA_PLANOS_0XY_0XZ;
    else if ((simXY == 1) && (simXZ != 1) && (simYZ == 1))
        simetria = SIMETRIA_PLANOS_0XY_0YZ;
    else if ((simXY != 1) && (simXZ == 1) && (simYZ == 1))
        simetria = SIMETRIA_PLANOS_0XZ_0YZ;
    else if ((simXY == 1) && (simXZ == 1) && (simYZ == 1))
        simetria = SIMETRIA_TRES_PLANOS_COORDENADOS;
    else
        simetria = SIMETRIA_ERRONEA;

    return simetria;
}


void CODIFICADA()
{
    //* Declaracion de variables
    
    
    int  bContacto; //Indica si es superficie de contacto
    int  nele,nex,nexpc,nin;    //Número de elementos y extremos actuales. Número de extremos en la zona de contacto
    int  ifla1; //Flag para continuar o no la lectura
    int  tipoS; //Tipo de superficie
    int  i,j;   //Auxiliares
    //* Lee los datos relativos a la zona de contacto
    nele=1;
    nex=1;
    ifla1=0;
    bContacto=1;
    while(ifla1 == 0)
    {
        leeEntero(in1,&tipoS);leeEntero(in1,&ifla1);leeLinea(in1);
        //* Segun el tipo de contorno sigue un camino u otro
        if(tipoS == 0)        
        {
            //* Contacto S4-S4
            SUPERFICIE_CUATRO(&nele,&nex,&bContacto);if(enExcepcion==1)return;
        }
        else if(tipoS == 1)        
        {
            //* Contacto S3-S3
            SUPERFICIE_TRES(&nele,&nex,&bContacto);if(enExcepcion==1)return;
        }
        else        
        {
            //* Contacto entre otros tipos de superficies
            printf(" **ERROR**: SUPERFICIE DE CONTACTO NO PROGRAMADA\n");
            printf("REVISE LA ENTRADA DE DATOS\n"); enExcepcion=1;return;
        }
    }
    //* Se ha terminado la zona de contacto
    nelpc=nele-1;
    nexpc=nex-1;
    //* Lee codigos de la zona de contacto
    nin=1;
    LEE_CODIGOS(&nin,&nelpc);if(enExcepcion==1)return;
    //* Inicializa condiciones de contorno de la zona de contacto
    for( i=1; i<=nelpc; i++)      
    {
        codA[i-1][1-1]=codB[i-1][1-1];
        codA[i-1][2-1]=codB[i-1][2-1];
        for( j=1; j<=10; j++)        
        {
            ccA[i-1][j-1]=0.;
            ccB[i-1][j-1]=0.;
        }
    }
    //* Lee los datos relativos a la zona libre de "A"
    ifla1=0;
    bContacto=0;
    while(ifla1 == 0)
    {
        leeEntero(in1,&tipoS);leeEntero(in1,&ifla1);leeLinea(in1);
        //* Segun el tipo de contorno sigue un camino u otro
        if(tipoS == 0)        
        {
            //* Superficie S4
            SUPERFICIE_CUATRO(&nele,&nex,&bContacto);if(enExcepcion==1)return;
        }
        else if(tipoS == 1)        
        {
            //* Superficie S3-S3
            SUPERFICIE_TRES(&nele,&nex,&bContacto);if(enExcepcion==1)return;
        }
        else        
        {
            //* Otros tipos de superficies
            printf(" **ERROR**: SUPERFICIE NO PROGRAMADA EN SOLIDO A\n");
            printf("REVISE LA ENTRADA DE DATOS\n"); enExcepcion=1;return;
        }
    }
    //* Se ha terminado la zona libre de "A"
    nelA=nele-1;
    nexA=nex-1;
    //* Lee codigos de la zona libre de A
    nin=nelpc+1;
    LEE_CODIGOS(&nin,&nelA);if(enExcepcion==1)return;
    //* Lee condiciones de contorno de la zona libre de A
    LEE_CC(&nin,&nelA);if(enExcepcion==1)return;
    //* Asigna valores a variables relativas a "A"
    for( i=nexpc+1; i<=nexA; i++)      
    {
        for( j=1; j<=3; j++)        
        {
            exA[i-1][j-1]=exB[i-1][j-1];
        }
    }
    for( i=nelpc+1; i<=nelA; i++)      
    {
        codA[i-1][1-1]=codB[i-1][1-1];
        codA[i-1][2-1]=codB[i-1][2-1];
        for( j=1; j<=3; j++)        
        {
            conA[i-1][j-1]=conB[i-1][j-1];
        }
        for( j=1; j<=10; j++)        
        {
            ccA[i-1][j-1]=ccB[i-1][j-1];
        }

        for( j=1; j<=9; j++)        
        {
            locA[i-1][j-1]=locB[i-1][j-1];
        }
    }
    
    
#ifdef DEBUG
    printf("[funcionesMain.c] locA address: %p\n", locA);
    printf("[funcionesMain.c] locA[0][0] value: %f\n", locA[0][0]);
#endif
    
    //*    WRITE(*,*) 'SOLIDO B'
    //* Lee los datos relativos a la zona libre de "B"
    nele=nelpc+1;
    nex=nexpc+1;
    ifla1=0;
    while(ifla1 == 0)
    {
        leeEntero(in1,&tipoS);leeEntero(in1,&ifla1);leeLinea(in1);
        //* Segun el tipo de contorno sigue un camino u otro
        if(tipoS == 0)        
        {
            //* Superficie S4
            SUPERFICIE_CUATRO(&nele,&nex,&bContacto);if(enExcepcion==1)return;
        }
        else if(tipoS == 1)        
        {
            //* Superficie S3-S3
            SUPERFICIE_TRES(&nele,&nex,&bContacto);if(enExcepcion==1)return;
        }
        else        
        {
            //* Otros tipos de superficies
            printf(" **ERROR**: SUPERFICIE NO PROGRAMADA EN SOLIDO A\n");
            printf("REVISE LA ENTRADA DE DATOS\n"); enExcepcion=1;return;
        }
    }
    //* Se ha terminado la zona libre de "B"
    nelB=nele-1;
    nexB=nex-1;
    //* Lee codigos de la zona libre de B
    nin=nelpc+1;
    LEE_CODIGOS(&nin,&nelB);if(enExcepcion==1)return;
    //* Lee condiciones de contorno de la zona libre de B
    LEE_CC(&nin,&nelB);if(enExcepcion==1)return;
    //* Calcula los nodos de ambos cuerpos
    CALCNODOS();if(enExcepcion==1)return;
    //* Calcula el sistema local de coordenadas de ambos cuerpos
    SISTLOCAL();if(enExcepcion==1)return;
    return;
}



void SEMICODIFICADA()
{
    //* Declaracion de variables
    
    
    
    
    //*       INCOMPLETA
    
    
    return;
}



void NODOS_CARGADOS()
{
    //* Declaracion de variables
    
    
    int  nd;    //Auxiliares
    int  i;
    //* Para el solido A
    nelwA=0;
    for( nd=nelpc+1; nd<=nelA; nd++)      
    {
        for( i=1; i<=10; i++)       
        {
            if(ccA[nd-1][i-1] != 0.0 )         
            {
                nelwA=nelwA+1;
                break;
            }
        }
    }
    //* Para el solido B
    nelwB=0;
    for( nd=nelpc+1; nd<=nelB; nd++)      
    {
        for( i=1; i<=10; i++)       
        {
            if(ccB[nd-1][i-1] != 0.0 )         
            {
                nelwB=nelwB+1;
                break;
            }
        }
    }
    return;
}



void DIR_CONTACTO()
{
    //* Declaracion de variables
    
    
    int  nd;    //Auxiliares
    int  i;
    double  modulo;
    //* Para todos los elementos de la zona potencial de contacto
    for( nd=1; nd<=nelpc; nd++)      
    {
        modulo=0.0;
        for( i=1; i<=3; i++)        
        {
            modulo=pow(modulo+(locA[nd-1][i-1]-locB[nd-1][i-1]),2);
        }
        modulo=sqrt(modulo);
        if(modulo != 0.0 )        
        {
            locA[nd-1][1-1]=(locA[nd-1][1-1]-locB[nd-1][1-1])/modulo;
            locB[nd-1][1-1]=-locA[nd-1][1-1];
            locA[nd-1][2-1]=(locA[nd-1][2-1]-locB[nd-1][2-1])/modulo;
            locB[nd-1][2-1]=-locA[nd-1][2-1];
            locA[nd-1][3-1]=(locA[nd-1][3-1]-locB[nd-1][3-1])/modulo;
            locB[nd-1][3-1]=-locA[nd-1][3-1];
        }
    }
    return;
}



void GAP_INICIAL()
{
    //* Declaracion de variables
    
    int  nd;    //Auxiliares
    double  modulo;
    //* Para todos los elementos de la zona potencial de contacto
    for( nd=1; nd<=nelpc; nd++)      
    {
        //* Calcula el modulo para ver si es nulo
        modulo=sqrt(pow((ndA[nd-1][1-1]-ndB[nd-1][1-1]),2)+pow((ndA[nd-1][2-1]-ndB[nd-1][2-1]),2)+pow((ndA[nd-1][3-1]-ndB[nd-1][3-1]),2));
        if(modulo == 0. )        
        {
            //* Separacion nula
            gap[nd-1]=0.;
        }
        else        
        {
            //* Calcula la separacion sobre la direccion de contacto
            gap[nd-1]=(ndA[nd-1][1-1]-ndB[nd-1][1-1])*locA[nd-1][1-1]+(ndA[nd-1][2-1]-ndB[nd-1][2-1])*locA[nd-1][2-1]+(ndA[nd-1][3-1]-ndB[nd-1][3-1])*locA[nd-1][3-1];
        }
    }
    return;
}




void INTEGRAL()
{
    //* Declaracion de variables
    
    
    int  el,ex; //Auxiliares
    int  j;
    float tiniA,tiniB,tfinA,tfinB;
    
    out11=81;
    out13=83;
    out14=84;

    // Asignamos valores mínimos del cuerpo A para poder inicializarlo
    cuerpoA[HOST].simXY = simXY;// Flag de simetría respecto del plano xOy
    cuerpoA[HOST].simXZ = simXZ;// Flag de simetría respecto del plano xOz
    cuerpoA[HOST].simYZ = simYZ;// Flag de simetría respecto del plano yOz
    cuerpoA[HOST].tpproE = tpproE;// Flag de tipo de problema elastico
    cuerpoA[HOST].tpproT = tpproT;// Flag de tipo de problema termico
    cuerpoA[HOST].tpproTE = tpproTE;// Flag de tipo de problema termoelastico
    cuerpoA[HOST].tpcarFP = tpcarFP;// Flag de tipo de carga térmica. Fuentes puntuales
    cuerpoA[HOST].tpcarFL = tpcarFL;// Flag de tipo de carga térmica. Fuentes lineales
    cuerpoA[HOST].tpcarFD = tpcarFD;// Flag de tipo de carga térmica. Fuentes distribuidas
    cuerpoA[HOST].tpcarFC = tpcarFC;// Flag de tipo de carga elástica. Fuerza centrífuga
    cuerpoA[HOST].tpcarPP = tpcarPP;// Flag de tipo de carga elástica. Peso propio
    cuerpoA[HOST].tipoSimetria = ObtenerSimetria(&cuerpoA[HOST]);


    // Calcula constantes
    cte1=16.0*4.0*atan(1.0)*GT*(1.0-nuT);
    cte2=1.0-2.0*nuT;
    cte3=8.0*(1.0-nuT)*4.0*atan(1.0);
    cte4=4.0*4.0*atan(1.0);
    cte5=alT*(1.0+nuT)/(8.0*4.0*atan(1.0)*(1.0-nuT));


    cuerpoA[HOST].cte1 = cte1;// Constante en la integracion elastica
    cuerpoA[HOST].cte2 = cte2;// Constante en la integracion elastica
    cuerpoA[HOST].cte3 = cte3;// Constante en la integracion elastica
    cuerpoA[HOST].cte4 = cte4;// Constante en la integracion termica
    cuerpoA[HOST].cte5 = cte5;

    cuerpoA[HOST].reg = reg;

    cuerpoA[HOST].enExcepcion = enExcepcion;

    cuerpoA[HOST].nexT = nexA;
    cuerpoA[HOST].nelT = nelA;
    cuerpoA[HOST].ET = EA;
    cuerpoA[HOST].alT = alA;
    cuerpoA[HOST].nuT = nuA;
    cuerpoA[HOST].GT = EA/(2.0*(1.0+nuA));

    cuerpoA[HOST].AT = AT;
    cuerpoA[HOST].BT = BT;

    cuerpoA[CUDA] = cuerpoA[HOST];


    // Asignamos valores mínimos del cuerpo B para poder inicializarlo
    cuerpoB[HOST].simXY = simXY;// Flag de simetría respecto del plano xOy
    cuerpoB[HOST].simXZ = simXZ;// Flag de simetría respecto del plano xOz
    cuerpoB[HOST].simYZ = simYZ;// Flag de simetría respecto del plano yOz
    cuerpoB[HOST].tpproE = tpproE;// Flag de tipo de problema elastico
    cuerpoB[HOST].tpproT = tpproT;// Flag de tipo de problema termico
    cuerpoB[HOST].tpproTE = tpproTE;// Flag de tipo de problema termoelastico
    cuerpoB[HOST].tpcarFP = tpcarFP;// Flag de tipo de carga térmica. Fuentes puntuales
    cuerpoB[HOST].tpcarFL = tpcarFL;// Flag de tipo de carga térmica. Fuentes lineales
    cuerpoB[HOST].tpcarFD = tpcarFD;// Flag de tipo de carga térmica. Fuentes distribuidas
    cuerpoB[HOST].tpcarFC = tpcarFC;// Flag de tipo de carga elástica. Fuerza centrífuga
    cuerpoB[HOST].tpcarPP = tpcarPP;// Flag de tipo de carga elástica. Peso propio
    cuerpoB[HOST].tipoSimetria = ObtenerSimetria(&cuerpoB[HOST]);

    cuerpoB[HOST].cte1 = cte1;// Constante en la integracion elastica
    cuerpoB[HOST].cte2 = cte2;// Constante en la integracion elastica
    cuerpoB[HOST].cte3 = cte3;// Constante en la integracion elastica
    cuerpoB[HOST].cte4 = cte4;// Constante en la integracion termica
    cuerpoB[HOST].cte5 = cte5;

    cuerpoB[HOST].reg = reg;

    cuerpoB[HOST].enExcepcion = enExcepcion;
    cuerpoB[HOST].nexT = nexB;
    cuerpoB[HOST].nelT = nelB;
    cuerpoB[HOST].ET = EB;
    cuerpoB[HOST].alT = alB;
    cuerpoB[HOST].nuT = nuB;
    cuerpoB[HOST].GT = EB/(2.0*(1.0+nuB));

    cuerpoB[HOST].AT = AT;
    cuerpoB[HOST].BT = BT;

    cuerpoB[CUDA] = cuerpoB[HOST];

    // Inicializamos las estructuras de entrada en cada contexto
    printf("Reservando memoria para estructuras de entrada de cuerpo en Host y Cuda... ");
    inicializar(&cuerpoA[HOST], HOST);
    inicializar(&cuerpoB[HOST], HOST);
    inicializar(&cuerpoA[CUDA], CUDA);
    inicializar(&cuerpoB[CUDA], CUDA);
    printf("Hecho.\n");

    for (int i=0; i<3; i++)
    {
        for(int j=0;j<3; j++)
        {
            cuerpoA[HOST].AE[i][j] = AE[i][j];
            cuerpoA[HOST].BE[i][j] = BE[i][j];
            cuerpoA[HOST].DTE[i][j] = DTE[i][j];
            cuerpoB[HOST].AE[i][j] = AE[i][j];
            cuerpoB[HOST].BE[i][j] = BE[i][j];
            cuerpoB[HOST].DTE[i][j] = DTE[i][j];
        }
        cuerpoA[HOST].CTE[i] = CTE[i];
        cuerpoA[HOST].DTTE[i] = DTTE[i];
        cuerpoB[HOST].CTE[i] = CTE[i];
        cuerpoB[HOST].DTTE[i] = DTTE[i];
    }
    for( ex=1; ex<=nexA; ex++)
        for( j=1; j<=3; j++)
            cuerpoA[HOST].exT[ex-1][j-1]=exA[ex-1][j-1];
    for( ex=1; ex<=nexB; ex++)
        for( j=1; j<=3; j++)
            cuerpoB[HOST].exT[ex-1][j-1]=exB[ex-1][j-1];
    for( el=1; el<=nelA; el++)
    {
        for( j=1; j<=3; j++)
        {
            cuerpoA[HOST].conT[el-1][j-1]=conA[el-1][j-1];
            cuerpoA[HOST].ndT[el-1][j-1]=ndA[el-1][j-1];
        }
        for( j=1; j<=9; j++)
            cuerpoA[HOST].locT[el-1][j-1]=locA[el-1][j-1];
    }
    for( el=1; el<=nelB; el++)
    {
        for( j=1; j<=3; j++)
        {
            cuerpoB[HOST].conT[el-1][j-1]=conB[el-1][j-1];
            cuerpoB[HOST].ndT[el-1][j-1]=ndB[el-1][j-1];
        }
        for( j=1; j<=9; j++)
            cuerpoB[HOST].locT[el-1][j-1]=locB[el-1][j-1];
    }

    //Vinculamos variables globales con los del cuerpo del host. Por si acaso.
    AE_A = cuerpoA[HOST].AE_T;
    BE_A = cuerpoA[HOST].BE_T;
    AT_A = cuerpoA[HOST].AT_T;
    BT_A = cuerpoA[HOST].BT_T;
    CTE_A = cuerpoA[HOST].CTE_T;
    DTE_A = cuerpoA[HOST].DTE_T;

    AE_B = cuerpoB[HOST].AE_T;
    BE_B = cuerpoB[HOST].BE_T;
    AT_B = cuerpoB[HOST].AT_T;
    BT_B = cuerpoB[HOST].BT_T;
    CTE_B = cuerpoB[HOST].CTE_T;
    DTE_B = cuerpoB[HOST].DTE_T;

    printf("==================== Entrando en INTEGRAL ====================\n");
    //* Lee los datos del problema
    

    //* Abre ficheros
    
    //* Calcula coeficientes del cuerpo "A"
    printf("++++++++ Calcula coeficientes del cuerpo \"A\" ++++++++\n");
    tiniA = clock()/CLOCKS_PER_SEC;
    printf("[%.3f] Inicio calculo coeficientes.\n",tiniA);


    copiarEstructura(&cuerpoA[CUDA], &cuerpoA[HOST], cudaMemcpyHostToDevice);

    EntradaCuerpo* entradaCuerpoEnDispositivo = pasarADispositivo(&cuerpoA[CUDA]);

    host_printCheck_conT(cuerpoA[HOST].conT);
    host_printCheck_exT(cuerpoA[HOST].exT);
    //chivato<<<1,1>>>(entradaCuerpoEnDispositivo);
    COEFICIENTES<<<1,1>>>(entradaCuerpoEnDispositivo);
    cudaDeviceSynchronize();
    printf("Error: %s\n",cudaGetErrorString(cudaGetLastError()));
    //COEFICIENTES<<<1, 1>>>(entradaCuerpoEnDispositivo);
    //cudaDeviceSynchronize();
    cuerpoA[CUDA] = obtenerDesdeDispositivo(entradaCuerpoEnDispositivo);

    copiarEstructura(&cuerpoA[HOST], &cuerpoA[CUDA], cudaMemcpyDeviceToHost);

    printf("Valor de cte5: %.2f", cuerpoA[HOST].cte5);
    if(cuerpoA[HOST].enExcepcion==1)return;
    tfinA = clock()/CLOCKS_PER_SEC;
    printf("[%.3f] Final calculo coeficientes.\n",tfinA);
    printf("\tTiempo Total= %f segundos\n",tfinA-tiniA);
    
    //* Cierra ficheros
    
    //* Asigna datos de entrada del cuerpo "B"

    //* Abre ficheros
    
    //* Calcula coeficientes del cuerpo "B"
    printf("+++++++ Calcula coeficientes del cuerpo \"B\" +++++++\n");
    tiniB = clock()/CLOCKS_PER_SEC;
    printf("[%.3f] Inicio calculo coeficientes.\n",tiniB);


    copiarEstructura(&cuerpoB[CUDA], &cuerpoB[HOST], cudaMemcpyHostToDevice);
    entradaCuerpoEnDispositivo = pasarADispositivo(&cuerpoB[CUDA]);
    COEFICIENTES<<<1, 1>>>(entradaCuerpoEnDispositivo);if(enExcepcion==1)return;
    cuerpoB[CUDA] = obtenerDesdeDispositivo(entradaCuerpoEnDispositivo);

    copiarEstructura(&cuerpoB[HOST], &cuerpoB[CUDA], cudaMemcpyDeviceToHost);

    if(cuerpoB[HOST].enExcepcion==1)return;
    //* Cierra ficheros
    
    //* Final del programa
    
    tfinB = clock()/CLOCKS_PER_SEC;
    printf("[%.3f] Final calculo coeficientes.\n",tfinB);
    printf("\tTiempo Total= %f segundos\n",tfinB-tiniB);
    
    printf("==================== Saliendo de INTEGRAL ===================\n");
    printf("TIEMPOOOOOOOOO: %f\n",(tfinA-tiniA)+(tfinB-tiniB));

    // Volcamos estructuras en variables globales para que pueda continuar el proceso.
    for (int i=0; i<3; i++)
    {
        for(int j=0;j<3; j++)
        {
            AE[i][j] = cuerpoA[HOST].AE[i][j];
            BE[i][j] = cuerpoA[HOST].BE[i][j];
            DTE[i][j] = cuerpoA[HOST].DTE[i][j];
            AE[i][j] = cuerpoB[HOST].AE[i][j];
            BE[i][j] = cuerpoB[HOST].BE[i][j];
            DTE[i][j] = cuerpoB[HOST].DTE[i][j];
        }
        CTE[i] = cuerpoA[HOST].CTE[i];
        DTTE[i] = cuerpoA[HOST].DTTE[i];
        CTE[i] = cuerpoB[HOST].CTE[i];
        DTTE[i] = cuerpoB[HOST].DTTE[i];
    }
    AT = cuerpoB->AT;
    BT = cuerpoB->BT;
    nexT = cuerpoB->nexT;
    nelT = cuerpoB->nelT;
    ET = cuerpoB->ET;
    alT = cuerpoB->alT;
    nuT = cuerpoB->nuT;
    GT = cuerpoB->GT;

    for( ex=1; ex<=nexB; ex++)
        for( j=1; j<=3; j++)
            exT[ex-1][j-1]=cuerpoB[HOST].exT[ex-1][j-1];

    for( el=1; el<=nelB; el++)
    {
        for( j=1; j<=3; j++)
        {
            conT[el-1][j-1]=cuerpoB[HOST].conT[el-1][j-1];
            ndT[el-1][j-1]=cuerpoB[HOST].ndT[el-1][j-1];
        }
        for( j=1; j<=9; j++)
            locT[el-1][j-1]=cuerpoB[HOST].locT[el-1][j-1];
    }
}


void TERMICO ()
{
    //* Declaracion de variables    
    //* Inicializa variables
    
    
    //* Comienzo del programa
    printf(" ==================== Entrando en TERMICO ====================\n");
    
    //* Se llama a la subrutina que trae los coeficientes   y monta el sistema
    MONTAJETER();if(enExcepcion==1)return;
    
    //* resuelve el sistema por el metodo de GAUSS
    GAUSS_SOLU();if(enExcepcion==1)return;
    //* Interpretacion de resultados (condiciones de contorno y contacto)
    CONTORNOTER();if(enExcepcion==1)return;
    //* Salida de resultados a ficheros
    SALIDATER();if(enExcepcion==1)return;
    //* Final del programa
    printf(" ==================== Saliendo de TERMICO ====================\n");
    return;
}



void MONTAJE()
{
    //* Declaracion de variables
    
    int i,j,nd,el,m;
    //**** NOTA : La matriz se va construyendo por filas.
    
    //* Inicializa el vector de cargas y la matriz de coeficientes
    for( i=1; i<=ngl; i++)      
    {
        b[i-1]=0.0;
        for( j=1; j<=ngl; j++)            
        {
            a[i-1][j-1]=0.0;
        }
    }
    //* Abre ficheros de coeficientes elásticos del cuerpo A
    
    //* Monta la parte correspondiente al cuerpo A del problema elastico
    printf("  Monta la parte correspondiente al cuerpo A del problema elastico\n");
    for( nd=1; nd<=nelA; nd++)      
    {
        for( el=1; el<=nelA; el++)            
        {
            //* Lee los coeficientes
            
            int arTemp_0;int arTemp_1;int arTemp_2;int arTemp_3;arTemp_2 = el*3 -2-1; for(arTemp_0=1;arTemp_0<=3;arTemp_0+=1)
            {
                arTemp_2 += 1;arTemp_3 = nd*3 -2-1; for(arTemp_1=1;arTemp_1<=3;arTemp_1+=1)
                {
                    arTemp_3 += 1;AE[arTemp_0-1][arTemp_1-1] =  AE_A[arTemp_2-1][arTemp_3-1];
                }
            }
            ;
            int arTemp_4;int arTemp_5;int arTemp_6;int arTemp_7;arTemp_6 = el*3 -2-1; for(arTemp_4=1;arTemp_4<=3;arTemp_4+=1)
            {
                arTemp_6 += 1;arTemp_7 = nd*3 -2-1; for(arTemp_5=1;arTemp_5<=3;arTemp_5+=1)
                {
                    arTemp_7 += 1;BE[arTemp_4-1][arTemp_5-1] =  BE_A[arTemp_6-1][arTemp_7-1];
                }
            }
            ;
            
            //* Mira si el nodo el pertenace a la zona actual de contacto
            if(codA[el-1][1-1] < 9)                  
            {
                //* No pertenece
                cod=codA[el-1][1-1];
                nel=0.0;
                for( m=1; m<=6; m++)                        
                {
                    cc[m-1]=ccA[el-1][m-1];
                }
                NOCONTA(&el,&nd);if(enExcepcion==1)return;
                
                //* Si pertenece
            }
            else if(codA[el-1][1-1] == 9)                  
            {
                //* Deslizamiento
                for( i=1; i<=3; i++)                        
                {
                    a[3*(nd-1)+i-1][3*(nelA+el)-2-1]=-BE[i-1][1-1]*Emedia;
                    for( j=1; j<=3; j++)                              
                    {
                        a[3*(nd-1)+i-1][3*(el-1)+j-1]=AE[i-1][j-1];
                    }
                }
            }
        }
    }
    
    //* Suma al termino independiente los coeficientes del problema termoelastico
    if(tpproTE == 1)      
    {
        for( nd=1; nd<=nelA; nd++)            
        {
            for( el=1; el<=nelA; el++)                  
            {
                int arTemp_8;int arTemp_9;int arTemp_10;arTemp_8 = 1-1; for(arTemp_9=el*3 -2;arTemp_9<=el*3;arTemp_9+=1)
                {
                    arTemp_8 += 1;for(arTemp_10=nd;arTemp_10<=nd;arTemp_10+=1)
                    {
                        CTE[arTemp_8-1] =  CTE_A[arTemp_9-1][arTemp_10-1];
                    }
                }
                ;
                int arTemp_11;int arTemp_12;int arTemp_13;arTemp_11 = 1-1; for(arTemp_12=el*3 -2;arTemp_12<=el*3;arTemp_12+=1)
                {
                    arTemp_11 += 1;for(arTemp_13=nd;arTemp_13<=nd;arTemp_13+=1)
                    {
                        DTTE[arTemp_11-1] =  DTE_A[arTemp_12-1][arTemp_13-1];
                    }
                }
                ;
                
                for( i=1; i<=3; i++)                        
                {
                    b[3*(nd-1)+i-1]=b[3*(nd-1)+i-1]+DTTE[i-1]*(tempA[el-1]-tref)-
                    CTE[i-1]*flujA[el-1];;
                }
            }
        }
    }
    //* Abre ficheros de coeficientes elasticos del cuerpo B
    //* Monta la parte correspondiente al cuerpo B del problema elastico
    printf("  Monta la parte correspondiente al cuerpo B del problema elastico\n");
    for( nd=1; nd<=nelB; nd++)      
    {
        for( el=1; el<=nelB; el++)            
        {
            //* Lee los coeficientes
            int arTemp_14;int arTemp_15;int arTemp_16;int arTemp_17;arTemp_16 = el*3 -2-1; for(arTemp_14=1;arTemp_14<=3;arTemp_14+=1)
            {
                arTemp_16 += 1;arTemp_17 = nd*3 -2-1; for(arTemp_15=1;arTemp_15<=3;arTemp_15+=1)
                {
                    arTemp_17 += 1;AE[arTemp_14-1][arTemp_15-1] =  AE_B[arTemp_16-1][arTemp_17-1];
                }
            }
            ;
            int arTemp_18;int arTemp_19;int arTemp_20;int arTemp_21;arTemp_20 = el*3 -2-1; for(arTemp_18=1;arTemp_18<=3;arTemp_18+=1)
            {
                arTemp_20 += 1;arTemp_21 = nd*3 -2-1; for(arTemp_19=1;arTemp_19<=3;arTemp_19+=1)
                {
                    arTemp_21 += 1;BE[arTemp_18-1][arTemp_19-1] =  BE_B[arTemp_20-1][arTemp_21-1];
                }
            }
            ;
            
            //* Mira si el nodo el pertenace a la zona actual de contacto
            if(codB[el-1][1-1] < 9)                  
            {
                //* No pertenece
                nel=nelA;
                cod=codB[el-1][1-1];
                for( m=1; m<=6; m++)                        
                {
                    cc[m-1]=ccB[el-1][m-1];
                }
                NOCONTA(&el,&nd);if(enExcepcion==1)return;
                //* Si pertenece
            }
            else if(codB[el-1][1-1] == 9)                  
            {
                //* Deslizamiento
                for( i=1; i<=3; i++)                        
                {
                    for( j=2; j<=3; j++)                              
                    {
                        a[3*(nelA+nd-1)+i-1][3*(nelA+el-1)+j-1]=AE[i-1][j-1];
                    }
                    a[3*(nelA+nd-1)+i-1][3*(nelA+el)-2-1]=-BE[i-1][1-1]*Emedia;
                    a[3*(nelA+nd-1)+i-1][3*el-2-1]=-AE[i-1][1-1];
                    b[3*(nelA+nd-1)+i-1]=b[3*(nelA+nd-1)+i-1]-gap[el-1]*AE[i-1][1-1];
                }
            }
        }
    }
    
    //* Suma al termino independiente los coeficientes del problema termoelastico
    if(tpproTE == 1)      
    {
        for( nd=1; nd<=nelB; nd++)            
        {
            for( el=1; el<=nelB; el++)                  
            {
                int arTemp_22;int arTemp_23;int arTemp_24;arTemp_22 = 1-1; for(arTemp_23=el*3 -2;arTemp_23<=el*3;arTemp_23+=1)
                {
                    arTemp_22 += 1;for(arTemp_24=nd;arTemp_24<=nd;arTemp_24+=1)
                    {
                        CTE[arTemp_22-1] =  CTE_B[arTemp_23-1][arTemp_24-1];
                    }
                }
                ;
                int arTemp_25;int arTemp_26;int arTemp_27;arTemp_25 = 1-1; for(arTemp_26=el*3 -2;arTemp_26<=el*3;arTemp_26+=1)
                {
                    arTemp_25 += 1;for(arTemp_27=nd;arTemp_27<=nd;arTemp_27+=1)
                    {
                        DTTE[arTemp_25-1] =  DTE_B[arTemp_26-1][arTemp_27-1];
                    }
                }
                ;
                
                for( i=1; i<=3; i++)                        
                {
                    b[3*(nelA+nd-1)+i-1]=b[3*(nelA+nd-1)+i-1]+DTTE[i-1]*(tempB[el-1]-tref)-
                    CTE[i-1]*flujB[el-1];;
                }
            }
        }
    }
    printf("fin montaje elas\n");
    
    return;
}


void INTERPR()
{
    //* Declaracion de variables
    
    int j,k;    //auxiliar
    int ix; //parametro para identificar la numeración de las variables
    //* Interpreta resultados del cuerpo A
    for( j=1; j<=nelA; j++)      
    {
        //* Mira si el nodo j perteneca a la zona de contacto actual
        if(codA[j-1][1-1] < 9)            
        {
            //* No pertenece
            ix=0.0;
            INTLIBR(&j,&ix);if(enExcepcion==1)return;
            //* Si pertenece
        }
        else if(codA[j-1][1-1] == 9)            
        {
            //* Deslizamiento
            for( k=1; k<=3; k++)                  
            {
                ccA[j-1][k-1]=x[3*(j-1)+k-1];
            }
            ccA[j-1][4-1]=x[3*(j+nelA)-2-1]*Emedia;
            ccA[j-1][5-1]=0.;
            ccA[j-1][6-1]=0.;
        }
        else            
        {
            //* Adhesion
            for( k=1; k<=3; k++)                  
            {
                ccA[j-1][k-1]=x[3*(j-1)+k-1];
                ccA[j-1][k+3-1]=x[3*(j+nelA-1)+k-1]*Emedia;
            }
        }
    }
    //* Interpreta resultados del cuerpo B
    for( j=1; j<=nelB; j++)      
    {
        //* Mira si el nodo j perteneca a la zona de contacto actual
        if(codB[j-1][1-1] < 9)            
        {
            //* No pertenece
            ix=3*nelA;
            INTLIBR(&j,&ix);if(enExcepcion==1)return;
            //* Si pertenece
        }
        else if(codB[j-1][1-1] == 9)            
        {
            //* Deslizamiento
            ccB[j-1][1-1]=gap[j-1]-ccA[j-1][1-1];
            ccB[j-1][4-1]=ccA[j-1][4-1];
            ccB[j-1][5-1]=-ccA[j-1][5-1];
            ccB[j-1][6-1]=ccA[j-1][6-1];
            for( k=2; k<=3; k++)                  
            {
                ccB[j-1][k-1]=x[3*(nelA+j-1)+k-1];
            }
        }
        else            
        {
            //* Adhesion
            ccB[j-1][1-1]=gap[j-1]-ccA[j-1][1-1];
            ccB[j-1][2-1]=ccA[j-1][2-1];
            ccB[j-1][3-1]=-ccA[j-1][3-1];
            ccB[j-1][4-1]=ccA[j-1][4-1];
            ccB[j-1][5-1]=-ccA[j-1][5-1];
            ccB[j-1][6-1]=ccA[j-1][6-1];
        }
    }
    return;
}


void DESLIZA()
{
    //* Declaracion de variables
    double tang;
    int i;  //auxiliar
    for( i=1; i<=nelpc; i++)      
    {
        if(codA[i-1][1-1] > 8)            
        {
            ve2[i-1]=ccA[i-1][2-1]-ccB[i-1][2-1];
            ve3[i-1]=ccA[i-1][3-1]+ccB[i-1][3-1];
            ve[i-1]=sqrt(pow(ve2[i-1],2)+pow(ve3[i-1],2));
            if(ve2[i-1] != 0. )                  
            {
                tang=ve3[i-1]/ve2[i-1];
                anguv[i-1]=atan(tang);
                if(ve2[i-1] < 0. )anguv[i-1]=anguv[i-1]+4.0*atan(1.0);              //3.141592654
            }
            else                  
            {
                anguv[i-1]=4.0*atan(1.0)/2.;
                if(ve3[i-1] < 0. )anguv[i-1]=anguv[i-1]+4.0*atan(1.0);              //3.141592654
            }
        }
        else            
        {
            ve[i-1]=0.;
            ve2[i-1]=0.;
            ve3[i-1]=0.;
            anguv[i-1]=0.;
        }
        anguv[i-1]=anguv[i-1]*180./4.0*atan(1.0);       //3.141592654
    }
    return;
}



void CHEQUEO()
{
    //* Declaracion de variables
    //* Inicializa los chivatos
    ifla=0;
    //* Chequeo de tracciones
    CHETRAC();if(enExcepcion==1)return;
    printf("numero de elementos que salen de la zona de contacto:%d\n",ifla);
    //* Si es negativo, retorna
    if(ifla != 0)return;
    //* Chequeo de interpenetraciones
    CHEINTE();if(enExcepcion==1)return;
    printf("numero de elementos que entran en la zona de contacto:%d\n",ifla);
    return;
}



void COMPRESIS()
{
    //* Declaracion de variables
    
    
    int nd; //auxiliar
    double mc,sigc; //Parametros de rugosidades
    double Hmicro;  //Parametros de microdurezas
    double kc,hc,hj,hb,hg;  //conductividades y conductancias contacto
    double Y,Mgas;  //interfase
    double tempg;   //Temperatura media del gas
    //* Ponemos a cero el chivato chires
    chires=0;
    //* Calculo de la resistencia térmica
    
    for( nd=1; nd<= nelpc     ; nd++)        
    {
        if(codA[nd-1][2-1] == 3)            
        {
            //*calcula parámetros de contacto DATOS: k en W/mmºC, sig en mm, Hmicro en MPa
            kc=2*lamA*lamB/(lamA+lamB);
            Hmicro=2*HAmicro*HBmicro/(HAmicro+HBmicro);
            mc=pow((pow(mA,2)+pow(mB,2)),0.50);
            sigc=pow((pow(sigA,2)+pow(sigB,2)),0.50);
            //* Calcula resistencia térmica de contacto
            
            //Ley de Yovanovich
            hc=1.25*kc*mc/sigc*pow((fabs(ccA[nd-1][4-1])/Hmicro),0.950);
            //Ley de Pilar
            //_____________________________________________________
            //hc=1.45*kc*mc/sigc*(abs(ccA(nd,4))/Hmicro)**0.985D0
            //_____________________________________________________
            
            if(TIM == 1)                
            {
                hg=0.00;
            }
            else                
            {
                Y=1.53*sigc*pow((fabs(ccA[nd-1][4-1])/Hmicro),(-0.0970));
                if(TIM == 2)                    
                {
                    tempg=(tempA[nd-1]+tempB[nd-1])/2;
                    Mgas=M0*(tempg/50)*(0.1013000/(fabs(ccA[nd-1][4-1])));
                }
                else                    
                {
                    Mgas=0.0;
                }
                hg=kg/(Y+Mgas);
            }
            hj=hc+hg;
            if(TIM == 4)                
            {
                hb=kg/(tTIM*(1-fabs(ccA[nd-1][4-1])/ETIM));
                //hb=kg/tTIM
                RTCC[nd-1]=1/hj+1/hb;
            }
            else                
            {
                RTCC[nd-1]=1/hj;
            }
            //* Resistencia ejemplo Kishimoto
            //RTCC(nd)=0.002*exp(-0.01*(abs(ccA(nd,4))))*1e6
        }
    }
    //* Compara resistencias termicas nodos en contacto
    
    for( nd=1; nd<= nelpc ; nd++)            
    {
        if(codA[nd-1][2-1] == 3)                
        {
            if(fabs((RTC[nd-1]-RTCC[nd-1])*100/RTCC[nd-1]) > (10.0) )                  
            {
                chires=1;
                goto l3434;
            }
        }
    }
    //* Asigna valores de resistencia termica
    l3434:      for( nd=1; nd<= nelpc; nd++)    
    {
        if(codA[nd-1][2-1] == 3)            
        {
            RTC[nd-1]=RTCC[nd-1];
        }
    }
    return;
}



void SALIDA_SOLU()
{
    //* Declaracion de variables
    int nd; //auxiliar
    
    //* abre ficheros de salida
    out3s = fopen(nomArchSalElas, "w");
    
    fprintf(out3s," %s",titulo);    // Formato: 5
    fprintf(out3s," Resultados problema ELÁSTICO de contacto\n");   // Formato: 10
    fprintf(out3s," \n");   // Formato: 20
    fprintf(out3s," DESPLAZAMIENTOS Y TENSIONES\n");    // Formato: 30
    fprintf(out3s," \n");   // Formato: 20
    
    //* CUERPO A
    
    fprintf(out3s," ==================== CUERPO A ====================\n"); // Formato: 35
    fprintf(out3s,"  - Zona de contacto -\n");  // Formato: 40
    
    //*  RESULTADOS EN: Movimientos [µm], Presiones [MPa], Resistencias en [mm2 ºC/W]
    //*  Zona de contacto de A
    
    for( nd=1; nd<=nelpc; nd++)    
    {
        fprintf(out3s," %4d %12.5f %12.4f %12.4f %12.4f %12.4f %12.4f %13.6f\n",nd,ccA[nd-1][1-1]*1e3,ccA[nd-1][2-1],ccA[nd-1][3-1],-ccA[nd-1][4-1],ccA[nd-1][5-1],ccA[nd-1][6-1],RTC[nd-1]);       // Formato: 100
    }
    //*  Zona libre de A
    fprintf(out3s,"  - Zona libre -\n");    // Formato: 60
    
    for( nd=nelpc+1; nd<=nelA; nd++)    
    {
        fprintf(out3s," %4d %12.5f %12.4f %12.4f %12.4f %12.4f %12.4f \n",nd,ccA[nd-1][1-1]*1e3,ccA[nd-1][2-1],ccA[nd-1][3-1],-ccA[nd-1][4-1],ccA[nd-1][5-1],ccA[nd-1][6-1]);       // Formato: 110
    }
    fprintf(out3s," \n");   // Formato: 20
    
    //* CUERPO B
    fprintf(out3s," ==================== CUERPO B ====================\n"); // Formato: 55
    fprintf(out3s,"  - Zona de contacto -\n");  // Formato: 40
    
    
    //*  Zona de contacto de B
    
    for( nd=1; nd<=nelpc; nd++)        
    {
        fprintf(out3s," %4d %12.5f %12.4f %12.4f %12.4f %12.4f %12.4f %13.6f\n",nd,ccB[nd-1][1-1]*1e3,ccB[nd-1][2-1],ccB[nd-1][3-1],-ccB[nd-1][4-1],ccB[nd-1][5-1],ccB[nd-1][6-1],RTC[nd-1]);       // Formato: 100
    }
    //*  Zona libre de B
    fprintf(out3s,"  - Zona libre -\n");    // Formato: 60
    //write(out3s,56)
    for( nd=nelpc+1; nd<=nelB; nd++)        
    {
        fprintf(out3s," %4d %12.5f %12.4f %12.4f %12.4f %12.4f %12.4f \n",nd,ccB[nd-1][1-1]*1e3,ccB[nd-1][2-1],ccB[nd-1][3-1],-ccB[nd-1][4-1],ccB[nd-1][5-1],ccB[nd-1][6-1]);       // Formato: 110
    }
    //*  Cierra fichero
    
    fclose(out3s);
    
    //* Formatos
    return;
}