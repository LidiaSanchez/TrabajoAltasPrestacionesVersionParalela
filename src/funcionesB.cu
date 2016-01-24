#include "funcionesB.h"

//******************************************************************************
//*                                                                            *
//*  Input:       Fichero 'Entrada.dat'                                        *
//*               ndInicial = Nodo en el que se comienza la asignacion         *
//*               ndFinal = Nodo en el que se finaliza la asignacion           *
//*               tpCod = 1 para codigos elasticos y 2 para codigos termicos   *
//*  Output:                                                                   *
//*  Usage:                                                                    *
//*  Description: Lee los codigos de carga                                     *
//*                                                                            *
//******************************************************************************

void CODIGOS(int* punteroA_ndInicial,int* punteroA_ndFinal,int* punteroA_tpCod)
{
    //* Declaracion de variables


    int ndInicial=*punteroA_ndInicial,ndFinal=*punteroA_ndFinal;	//Nodos inicial y final
    int  ndUlt,ndAct,nd;	//Auxiliares
    int codigo, tpCod=*punteroA_tpCod;
    ndAct = ndInicial;
    while(ndAct <= ndFinal)
    {
        leeEntero(in1,&ndUlt);leeEntero(in1,&codigo);leeLinea(in1);
        if((tpCod == 1) && ((codigo > 11) || (codigo < 1)))        
        {
            printf(" Nodo = %d Codigo = %d\n",ndUlt,codigo);
            printf("***ERROR*** => CODIGO ELASTICO NO VALIDO\n"); enExcepcion=1;return;
        }
        else if((tpCod == 2) && ((codigo > 4) || (codigo < 1)))        
        {
            printf(" Nodo = %d Codigo = %d\n",ndUlt,codigo);
            printf("***ERROR*** => CODIGO TERMICO NO VALIDO\n"); enExcepcion=1;return;
        }
        if((ndUlt < ndAct) || (ndUlt > ndFinal))        
        {
            printf(" Nodo = %d Codigo = %d\n",ndUlt,codigo);
            printf("***ERROR*** => ERROR EN CODIGOS\n"); enExcepcion=1;return;
        }
        for( nd=ndAct; nd<=ndUlt; nd++)        
        {
            codB[nd-1][tpCod-1]=codigo;
        }
        ndAct=ndUlt+1;
    }
    *punteroA_ndInicial = ndInicial; *punteroA_ndFinal = ndFinal; *punteroA_tpCod = tpCod; return;
}
//******************************************************************************
//*                                                                            *
//*  Input:       intenum = 1 -> Integracion numerica                          *
//*                        = 0 -> Integracion analitica                        *
//*  Output:      AET,BET,ATT,BTT,CT,DT = coeficientes de integracion          *
//*  Usage:                                                                    *
//*  Description: Coordina las integraciones. LLama a las subrutina de         *
//*               integracion apropiada                                        *
//*                                                                            *
//******************************************************************************
__device__
void INTEGRA(EntradaCuerpo* entradaCuerpo, double AET[3][3],double BET[3][3],double* punteroA_ATT,double* punteroA_BTT,double * CT,double DT[3][3])
{
    //* Declaracion de variables

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

    int&  simXY = entradaCuerpo->simXY;// Flag de simetría respecto del plano xOy
    int&  simXZ = entradaCuerpo->simXZ;// Flag de simetría respecto del plano xOz
    int&  simYZ = entradaCuerpo->simYZ;// Flag de simetría respecto del plano yOz
    int&  tpproE = entradaCuerpo->tpproE;// Flag de tipo de problema elastico
    int&  tpproT = entradaCuerpo->tpproT;// Flag de tipo de problema termico
    int&  tpproTE = entradaCuerpo->tpproTE;// Flag de tipo de problema termoelastico
    int&  tpcarFP = entradaCuerpo->tpcarFP;// Flag de tipo de carga térmica. Fuentes puntuales
    int&  tpcarFL = entradaCuerpo->tpcarFL;// Flag de tipo de carga térmica. Fuentes lineales
    int&  tpcarFD = entradaCuerpo->tpcarFD;// Flag de tipo de carga térmica. Fuentes distribuidas
    int&  tpcarFC = entradaCuerpo->tpcarFC;// Flag de tipo de carga elástica. Fuerza centrífuga
    int&  tpcarPP = entradaCuerpo->tpcarPP;// Flag de tipo de carga elástica. Peso propio

    int& enExcepcion = entradaCuerpo->enExcepcion;

    /*double AET[3][3];*	//*double BET[3][3];*/;      //Coeficientes de integracion elasticos
    double ATT=*punteroA_ATT;double BTT=*punteroA_BTT;;	//Coeficientes de integracion termicos
    /*double CT[3];*	//*double DT[3][3];*/;      //Coeficientes de integracion termoelasticos
    double  AEP[3][3],BEP[3][3];	//Coeficientes de integracion elasticos parciales
    double  ATP,BTP;	//Coeficientes de integracion termicos parciales
    double  CP[3],DP[3][3];	//Coeficientes de integracion termoelasticos parciales
    double  dist;	//Distancia entre el punto de colocacion y el baricentro del elemento
    double  subextr[MAX_SUB][4][3];	//Coordenadas de los vertices de los cuatro subtriangulos
    double  extrs[4][3];	//Coordenadas de los vertices del triangulo a dividir
    double  subex[4][4][3];	//Auxiliares
    int  i,j,k,m,p;
    double  bar[3];	//baricentro del elemento
    double  lado[3];	//longitud de cada lado del elemento
    double  lad[3][3];	//vector de cada lado del elemento
    double  n[3];
    double  distancia,dis[3];

    //* Asigna cuarto extremo y calcula baricentro del elemento
    for( i=1; i<=3; i++)      
    {
        extr[4-1][i-1]=extr[1-1][i-1];
        bar[i-1]=(extr[1-1][i-1]+extr[2-1][i-1]+extr[3-1][i-1])/3.0;
    }
    //* Calcula los lados de los elementos
    for( i=1; i<=3; i++)        
    {
        lado[i-1]=0.0;
        for( j=1; j<=3; j++)          
        {
            lado[i-1]=lado[i-1]+pow((extr[i+1-1][j-1]-extr[i-1][j-1]),2);
        }
        lado[i-1]=sqrt(lado[i-1]);
    }
    for( i=1; i<=3; i++)      
    {
        for( j=1; j<=3; j++)        
        {
            lad[i-1][j-1]= (extr[i+1-1][j-1]-extr[i-1][j-1])/lado[i-1];
        }
    }
    //* Calcula el vector normal
    n[1-1]=-lad[1-1][2-1]*lad[3-1][3-1]+lad[1-1][3-1]*lad[3-1][2-1];
    n[2-1]=-lad[1-1][3-1]*lad[3-1][1-1]+lad[1-1][1-1]*lad[3-1][3-1];
    n[3-1]=-lad[1-1][1-1]*lad[3-1][2-1]+lad[1-1][2-1]*lad[3-1][1-1];

    //* Decide el tipo de integracion
    if(!intenum)
    {
#ifdef DEBUG_TIMES
        printf("[INTEGRA()] Analitica() called.\n");
#endif
        ANALITICA(entradaCuerpo, AET,BET,&ATT,&BTT,CT,DT,n,bar);if(enExcepcion==1)return;
#ifdef DEBUG_TIMES
        printf("[INTEGRA()] Analitica() returned.\n");
#endif
    }
    else      
    {
        //*Calcula el vector que va del baricentro al nodo desde el que se integra
        dist=0.0;
        distancia=0.0;
        for( i=1; i<=3; i++)          
        {
            dis[i-1]=(bar[i-1]-ndCol[i-1]);
            dist=dist+pow(dis[i-1],2);
            distancia=distancia+dis[i-1]*n[i-1];
        }
        dist=sqrt(dist);
        //*Calcula la distancia del nodo desde el que se integra al plano del elemento
        distancia=fabs(distancia);

        //* Decide si hay que subdividir el elemento
        if((lado[1-1] > dist) || (lado[2-1] > dist) || (lado[3-1] > dist))        
        {
            //* Hay que subdividir el elemento
            for( i=1; i<=4; i++)             
            {
                for( j=1; j<=3; j++)              
                {
                    extrs[i-1][j-1]=extr[i-1][j-1];
                }
            }
#ifdef DEBUG_TIMES
            printf("[INTEGRA()] Subdivide() called. (1)\n");
#endif
            SUBDIVIDE(extrs, subex);if(enExcepcion==1)return;
#ifdef DEBUG_TIMES
            printf("[INTEGRA()] Subdivide() returned. (1)\n");
#endif

            for( i=1; i<=4; i++)          
            {
                for( j=1; j<=4; j++)            
                {
                    for( k=1; k<=3; k++)              
                    {
                        subextr[i-1][j-1][k-1]=subex[i-1][j-1][k-1];
                    }
                }
            }
            i=1;
            k=4;
            while(i <= k)
            {
                //* Nuevas constantes de subdivision
                dist=0.0;
                for( j=1; j<=3; j++)            
                {
                    bar[j-1]=(subextr[i-1][1-1][j-1]+subextr[i-1][2-1][j-1]+subextr[i-1][3-1][j-1])/3.0;
                    dist=dist+pow((ndCol[j-1]-bar[j-1]),2);
                    lado[j-1]=0.0;
                    for( m=1; m<=3; m++)              
                    {
                        lado[j-1]=lado[j-1]+pow((subextr[i-1][j+1-1][m-1]-subextr[i-1][j-1][m-1]),2);
                    }
                    lado[j-1]=sqrt(lado[j-1]);
                }
                dist=sqrt(dist);

                if((lado[1-1] > dist) || (lado[2-1] > dist) || (lado[3-1] > dist))            
                {
                    //* Hay que volver a subdividir
                    for( j=1; j<=4; j++)              
                    {
                        for( m=1; m<=3; m++)                
                        {
                            extrs[j-1][m-1]=subextr[i-1][j-1][m-1];
                        }
                    }
#ifdef DEBUG_TIMES
                    printf("[INTEGRA()] Subdivide() called. (2)\n");
#endif
                    SUBDIVIDE(extrs, subex);if(enExcepcion==1)return;
#ifdef DEBUG_TIMES
                    printf("[INTEGRA()] Subdivide() returned. (2)\n");
#endif

                    for( j=1; j<=4; j++)              
                    {
                        for( p=1; p<=3; p++)                
                        {
                            subextr[i-1][j-1][p-1]=subex[1-1][j-1][p-1];
                        }
                    }
                    for( m=2; m<=4; m++)              
                    {
                        for( j=1; j<=4; j++)                
                        {
                            for( p=1; p<=3; p++)                  
                            {
                                subextr[k+m-1-1][j-1][p-1]=subex[m-1][j-1][p-1];
                            }
                        }
                    }
                    k=k+3;
                    if(k > 997)              
                    {
                        printf("Superado el numero maximo de subdivisiones\n"); enExcepcion=1;return;
                    }
                }
                else            
                {
                    i=i+1;
                }
            }
            for( i=1; i<=3; i++)        
            {
                CT[i-1]=0.0;
                for( j=1; j<=3; j++)            
                {
                    AET[i-1][j-1]=0.0;
                    BET[i-1][j-1]=0.0;
                    DT[i-1][j-1]=0.0;
                }
            }
            ATT=0.0;
            BTT=0.0;

            for( i=1; i<=k; i++)        
            {
                for( j=1; j<=4; j++)            
                {
                    for( m=1; m<=3; m++)                
                    {
                        extr[j-1][m-1]=subextr[i-1][j-1][m-1];
                    }
                }
#ifdef DEBUG_TIMES
                printf("[INTEGRA()] Numerica() called.\n");
#endif
                NUMERICA(entradaCuerpo, AEP,BEP,&ATP,&BTP,(double*)CP,DP,(double*)n,&distancia);if(enExcepcion==1)return;
#ifdef DEBUG_TIMES
                printf("[INTEGRA()] Numerica() returned.\n");
#endif
                if((tpproE == 1) || (tpproTE == 1))            
                {
                    for( j=1; j<=3; j++)                
                    {
                        for( m=1; m<=3      ; m++)                    
                        {
                            AET[j-1][m-1]=AET[j-1][m-1]+AEP[j-1][m-1];
                            BET[j-1][m-1]=BET[j-1][m-1]+BEP[j-1][m-1];
                        }
                    }
                }
                if((tpproT == 1) || (tpproTE == 1))            
                {
                    ATT=ATT+ATP;
                    BTT=BTT+BTP;
                }
                if(tpproTE == 1)            
                {
                    for( j=1; j<=3; j++)                
                    {
                        CT[j-1]=CT[j-1]+CP[j-1];
                        for( m=1; m<=3; m++)                    
                        {
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
            NUMERICA(entradaCuerpo, AET,BET,&ATT,&BTT,CT,DT,(double*)n,&distancia);if(enExcepcion==1)return;
        }
    }
    *punteroA_ATT = ATT; *punteroA_BTT = BTT; return;
}
//******************************************************************************
//*                                                                            *
//*  Input:       el = elemento sobre el que se ha integrado                   *
//*  Output:      Coeficientes elasticos transformados                         *
//*  Usage:                                                                    *
//*  Description: Cambia a coordenadas locales los coeficientes de integracion *
//*               elasticos                                                    *
//*                                                                            *
//******************************************************************************
__device__
void TRANSFORMA(EntradaCuerpo* entradaCuerpo, double AETR[3][3],double BETR[3][3], int* punteroA_el)
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


    double &AT = entradaCuerpo->AT;
    double &BT = entradaCuerpo->BT;

    double AE[3][3];
    double BE[3][3];
    double DTE[3][3];
    for (int i=0; i< 3; i++)
        for(int j=0; j<3; j++)
        {
            AE[i][j] = entradaCuerpo->AE[i][j];
            BE[i][j] = entradaCuerpo->BE[i][j];
            DTE[i][j] = entradaCuerpo->DTE[i][j];
        }

    double * CTE = entradaCuerpo->CTE;
    double * DTTE = entradaCuerpo->DTTE;


    // Reemplazamos las referencias globales de variables.h por las del cuerpo. No son punteros, pero el '&' nos permite
    // vincular variables en C++:
    int &nelT = entradaCuerpo->nelT;
    double &GT = entradaCuerpo->GT;
    double &nuT = entradaCuerpo->nuT;
    double &alT = entradaCuerpo->alT;

    int&  simXY = entradaCuerpo->simXY;// Flag de simetría respecto del plano xOy
    int&  simXZ = entradaCuerpo->simXZ;// Flag de simetría respecto del plano xOz
    int&  simYZ = entradaCuerpo->simYZ;// Flag de simetría respecto del plano yOz
    int&  tpproE = entradaCuerpo->tpproE;// Flag de tipo de problema elastico
    int&  tpproT = entradaCuerpo->tpproT;// Flag de tipo de problema termico
    int&  tpproTE = entradaCuerpo->tpproTE;// Flag de tipo de problema termoelastico
    int&  tpcarFP = entradaCuerpo->tpcarFP;// Flag de tipo de carga térmica. Fuentes puntuales
    int&  tpcarFL = entradaCuerpo->tpcarFL;// Flag de tipo de carga térmica. Fuentes lineales
    int&  tpcarFD = entradaCuerpo->tpcarFD;// Flag de tipo de carga térmica. Fuentes distribuidas
    int&  tpcarFC = entradaCuerpo->tpcarFC;// Flag de tipo de carga elástica. Fuerza centrífuga
    int&  tpcarPP = entradaCuerpo->tpcarPP;// Flag de tipo de carga elástica. Peso propio

    int& enExcepcion = entradaCuerpo->enExcepcion;

    double ** exT = entradaCuerpo->exT;
    int    ** conT = entradaCuerpo->conT;
    double ** ndT = entradaCuerpo->ndT;
    double ** locT = entradaCuerpo->locT;

    double * ndCol = entradaCuerpo->ndCol;
    double ** extr = entradaCuerpo->extr;


    int &tipoSimetria = entradaCuerpo->tipoSimetria;
    char &intenum = entradaCuerpo->intenum;

    int el=*punteroA_el;;	//Elemento sobre el que se ha integrado
    int  i,j;	//Auxiliares
    double ccc[3][3];/*double AETR[3][3];*	//*double BETR[3][3];*/;

    //* Realiza transformacion

    //* Coeficientes elásticos
    for( i=1; i<=3; i++)      
    {
        ccc[i-1][1-1]=AETR[i-1][1-1]*locT[el-1][1-1]+AETR[i-1][2-1]*locT[el-1][2-1]+AETR[i-1][3-1]*locT[el-1][3-1];
        ccc[i-1][2-1]=AETR[i-1][1-1]*locT[el-1][4-1]+AETR[i-1][2-1]*locT[el-1][5-1]+AETR[i-1][3-1]*locT[el-1][6-1];
        ccc[i-1][3-1]=AETR[i-1][1-1]*locT[el-1][7-1]+AETR[i-1][2-1]*locT[el-1][8-1]+AETR[i-1][3-1]*locT[el-1][9-1];

    }
    for( i=1; i<=3; i++)      
    {
        for( j=1; j<=3; j++)        
        {
            AETR[i-1][j-1]=ccc[i-1][j-1];
        }
    }
    for( i=1; i<=3; i++)      
    {
        ccc[i-1][1-1]=BETR[i-1][1-1]*locT[el-1][1-1]+BETR[i-1][2-1]*locT[el-1][2-1]+BETR[i-1][3-1]*locT[el-1][3-1];
        ccc[i-1][2-1]=BETR[i-1][1-1]*locT[el-1][4-1]+BETR[i-1][2-1]*locT[el-1][5-1]+BETR[i-1][3-1]*locT[el-1][6-1];
        ccc[i-1][3-1]=BETR[i-1][1-1]*locT[el-1][7-1]+BETR[i-1][2-1]*locT[el-1][8-1]+BETR[i-1][3-1]*locT[el-1][9-1];
    }
    for( i=1; i<=3; i++)      
    {
        for( j=1; j<=3; j++)        
        {
            BETR[i-1][j-1]=ccc[i-1][j-1];
        }
    }

    *punteroA_el = el; return;
}
//******************************************************************************
//*                                                                            *
//*  Input:                                                                    *
//*  Output:      Ficheros de coeficientes de integracion                      *
//*  Usage:                                                                    *
//*  Description: Almacena los coeficientes de integracion                     *
//*                                                                            *
//******************************************************************************
__device__
void ALMACENA(int* punteroA_el,int* punteroA_nd, EntradaCuerpo* entradaCuerpo, double** AE_T,double** BE_T,double** AT_T,double** BT_T,double** CTE_T,double** DTE_T)
{
    //* Declaracion de variables

    // Aprovechamos que son punteros para direccionarlos a las matrices del cuerpo.
    // Nos ahorra muchas modificaciones.
    /*double** AE_T = entradaCuerpo->AE_T;
    double** BE_T = entradaCuerpo->BE_T;
    double** AT_T = entradaCuerpo->AT_T;
    double** BT_T = entradaCuerpo->BT_T;
    double** CTE_T = entradaCuerpo->CTE_T;
    double** DTE_T = entradaCuerpo->DTE_T;*/

    int&  simXY = entradaCuerpo->simXY;// Flag de simetría respecto del plano xOy
    int&  simXZ = entradaCuerpo->simXZ;// Flag de simetría respecto del plano xOz
    int&  simYZ = entradaCuerpo->simYZ;// Flag de simetría respecto del plano yOz
    int&  tpproE = entradaCuerpo->tpproE;// Flag de tipo de problema elastico
    int&  tpproT = entradaCuerpo->tpproT;// Flag de tipo de problema termico
    int&  tpproTE = entradaCuerpo->tpproTE;// Flag de tipo de problema termoelastico
    int&  tpcarFP = entradaCuerpo->tpcarFP;// Flag de tipo de carga térmica. Fuentes puntuales
    int&  tpcarFL = entradaCuerpo->tpcarFL;// Flag de tipo de carga térmica. Fuentes lineales
    int&  tpcarFD = entradaCuerpo->tpcarFD;// Flag de tipo de carga térmica. Fuentes distribuidas
    int&  tpcarFC = entradaCuerpo->tpcarFC;// Flag de tipo de carga elástica. Fuerza centrífuga
    int&  tpcarPP = entradaCuerpo->tpcarPP;// Flag de tipo de carga elástica. Peso propio

    int& enExcepcion = entradaCuerpo->enExcepcion;

    double &AT = entradaCuerpo->AT;
    double &BT = entradaCuerpo->BT;

    double AE[3][3];
    double BE[3][3];
    double DTE[3][3];
    for (int i=0; i< 3; i++)
        for(int j=0; j<3; j++)
        {
            AE[i][j] = entradaCuerpo->AE[i][j];
            BE[i][j] = entradaCuerpo->BE[i][j];
            DTE[i][j] = entradaCuerpo->DTE[i][j];
        }

    double * CTE = entradaCuerpo->CTE;
    double * DTTE = entradaCuerpo->DTTE;


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

    int el=*punteroA_el;int nd=*punteroA_nd;;	//Elemento sobre el que se ha integrado y nodo
    int  i,j;	//Auxiliares


    //* Coeficientes elasticos
    if((tpproE == 1) || (tpproTE == 1))      
    {
        //* SI SE DESEA PUEDEN ESCALARSE LOS COEFICIENTES Bij ANTES DEL ALMACENAJE

        int arTemp_0;int arTemp_1;int arTemp_2;int arTemp_3;arTemp_2 = 1-1; for(arTemp_0=el*3 - 2;arTemp_0<=el*3;arTemp_0+=1)
        {
            arTemp_2 += 1;arTemp_3 = 1-1; for(arTemp_1=nd*3 -2;arTemp_1<=nd*3;arTemp_1+=1)
            {
            arTemp_3 += 1;AE_T[arTemp_0-1][arTemp_1-1] = AE[arTemp_2-1][arTemp_3-1];
            }
        }
        ;
        int arTemp_4;int arTemp_5;int arTemp_6;int arTemp_7;arTemp_6 = 1-1; for(arTemp_4=el*3 - 2;arTemp_4<=el*3;arTemp_4+=1)
        {
            arTemp_6 += 1;arTemp_7 = 1-1; for(arTemp_5=nd*3 -2;arTemp_5<=nd*3;arTemp_5+=1)
            {
            arTemp_7 += 1;BE_T[arTemp_4-1][arTemp_5-1] = BE[arTemp_6-1][arTemp_7-1];
            }
        }
    }
    //* Coeficientes termicos
    if((tpproT == 1) || (tpproTE == 1))     
    {
        AT_T[el-1][nd-1]=AT;
        BT_T[el-1][nd-1]=BT;

    }
    //* Coeficientes termoelasticos
    if(tpproTE == 1)     
    {
        //inicializa coeficiente termoelástico
        for( i=1; i<=3; i++)        
        {
            DTTE[i-1]=0.0;
        }
        //* Calcula el coeficiente termoelástico Di=Dij*nj
        for( i=1; i<=3; i++)        
        {
            for( j=1; j<=3; j++)            
            {
                DTTE[i-1]=DTTE[i-1]+DTE[i-1][j-1]*locT[el-1][j-1];
            }
        }
        int arTemp_8;int arTemp_9;int arTemp_10;arTemp_10 = 1-1; for(arTemp_8=el*3 - 2;arTemp_8<=el*3;arTemp_8+=1)
        {
            arTemp_10 += 1;for(arTemp_9=nd;arTemp_9<=nd;arTemp_9+=1)
            {
            CTE_T[arTemp_8-1][arTemp_9-1] = CTE[arTemp_10-1];
            }
        }
        ;
        int arTemp_11;int arTemp_12;int arTemp_13;arTemp_13 = 1-1; for(arTemp_11=el*3 - 2;arTemp_11<=el*3;arTemp_11+=1)
        {
            arTemp_13 += 1;for(arTemp_12=nd;arTemp_12<=nd;arTemp_12+=1)
            {
            DTE_T[arTemp_11-1][arTemp_12-1] = DTTE[arTemp_13-1];
            }
        }

    }
    *punteroA_el = el; *punteroA_nd = nd; return;

}

__device__
void CHIVATO2(EntradaCuerpo* entradaCuerpo, double AET[3][3],double BET[3][3],double* punteroA_ATT,double* punteroA_BTT,double * CT,double DT[3][3])
{
    printf("[Thread %d] Ha entrado en chivato2!!!\n", threadIdx.x);
    printf("[Thread %d] Volverá? a saber.\n", threadIdx.x);

    return;
}