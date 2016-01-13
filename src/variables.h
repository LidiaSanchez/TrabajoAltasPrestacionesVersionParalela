#include <stdio.h>


#ifndef VARIABLES_H_
#define VARIABLES_H_

// CUDA API error checking macro
#define cudaCheck(error) \
  if (error != cudaSuccess) { \
    printf("Fatal error: %s at %s:%d\n", \
      cudaGetErrorString(error), \
      __FILE__, __LINE__); \
    exit(1); \
  }

#define MAX_EX 5000     // Máximo número de extremos (son 4000 todos)
#define MAX_ND 5000     // Máximo número de nodos
#define MAX_EL 5000     // Máximo número de elementos
#define MAX_SUB 5000    // Máximo número de subelementos

#define MEMCPY_HOST_TO_HOST 0
#define BLOCKS_PER_PROCESS 2
#define THREADS_PER_BLOCK 4

#define FICHERO_ENTRADA_COMPLETA "datos/EntradaCompleta.dat"
#define FICHERO_TERM "datos/term.dat"
#define FICHERO_ELAST "datos/elast.dat"


//#define DEBUG 0
//#define DEBUG_TIMES

enum CONTEXTO
{
    HOST = 0,
    CUDA = 1
};

enum SIMETRIAS
{
    SIMETRIA_ERRONEA                    =  -1,
    NO_SIMETRIA_IMPLICITA               =   0,
    SIMETRIA_PLANO_0XY                  =   1,
    SIMETRIA_PLANO_0XZ                  =   2,
    SIMETRIA_PLANO_0YZ                  =   3,
    SIMETRIA_PLANOS_0XY_0XZ             =   4,
    SIMETRIA_PLANOS_0XY_0YZ             =   5,
    SIMETRIA_PLANOS_0XZ_0YZ             =   6,
    SIMETRIA_TRES_PLANOS_COORDENADOS    =   7
};


typedef struct EntradaCuerpo
{
    CONTEXTO contexto;

    int nexT;
    int nelT;
    double ET;
    double alT;
    double nuT;
    double GT;
    double ** exT;
    int    ** conT;
    double ** ndT;
    double ** locT;

    // Coeficientes
    double ** AE_T;
    double ** BE_T;
    double ** AT_T;
    double ** BT_T;
    double ** CTE_T;
    double ** DTE_T;

    double ** AE;// Coeficientes A elasticos
    double ** BE;// Coeficientes B elasticos
    double  AT;// Coeficiente A termico
    double  BT;// Coeficiente B termico
    double * CTE;// Coeficientes C termoelasticos
    double * DTTE;// Coeficientes D termoelasticos
    double ** DTE;// Coeficientes D termoelasticos

    double * ndCol;
    char intenum;
    int tipoSimetria;
    double ** extr;
} EntradaCuerpo;


extern EntradaCuerpo cuerpoA[2];
extern EntradaCuerpo cuerpoB[2];

extern int enExcepcion;

extern char titulo[80];// Título del problema
extern char nomArchSalTer[80];// Nombre del archivo de salida [térmico]
extern char nomArchSalElas[80];// Nombre del archivo de salida [elástico

extern int  nelA;// Número total de elementos. Sólido A
extern int  nelB;// Número total de elementos. Sólido B
extern int  nelT;// Número total de elementos.
extern int  nelpA;// Número de elementos pasivos. Sólido A
extern int  nelpB;// Número de elementos pasivos. Sólido B
extern int  nelwA;// Número de elementos cargados. Sólido A
extern int  nelwB;// Número de elementos cargados. Sólido B
extern int  nelpc;// Número de elementos en zona potencial de contacto
extern int  nexA;// Número total de extremos. Sólido A
extern int  nexB;// Número total de extremos. Sólido B
extern int  nexT;// Número total de extremos.

extern int  nel;// Auxiliar montaje cuerpo A-cuerpoB
extern int  ngl;// Número total de grados de libertad del problema

extern int  conA[MAX_EL][3];// Tabla de conectividades. Sólido A
extern int  conB[MAX_EL][3];// Tabla de conectividades. Sólido B
extern int  conT[MAX_EL][3];// Tabla de conectividades.

extern int  codA[MAX_ND][2];// Códigos de carga. Sólido A
extern int  codB[MAX_ND][2];// Códigos de carga. Sólido B
extern int  codT[MAX_ND];// Codigos genericos
extern int  cod;// Codigos genericos para el montaje sistema elastico

extern double  ccA[MAX_EL][10];// Condiciones de contorno cuerpo A
extern double  ccB[MAX_EL][10];// Condiciones de contorno cuerpo B
extern double  cc[MAX_EL];// Condiciones de contorno generales para el montaje del sistema

extern double  exA[MAX_EX][3];// Coordenadas de los extremos. Sólido A
extern double  exB[MAX_EX][3];// Coordenadas de los extremos. Sólido B
extern double  exT[MAX_EX][3];// Coordenadas de los extremos.

extern double  ndA[MAX_ND][3];// Coordenadas de los nodos. Sólido A
extern double  ndB[MAX_ND][3];// Coordenadas de los nodos. Sólido B
extern double  ndT[MAX_ND][3];// Coordenadas de los nodos.

extern double  locA[MAX_ND][9];// Componentes del sistema local en los nodos. Sólido A
extern double  locB[MAX_ND][9];// Componentes del sistema local en los nodos. Sólido B
extern double  locT[MAX_ND][9];// Componentes del sistema local en los nodos.


extern double  gap[MAX_ND];// Gap en los pares de nodos en contacto

extern double  EA;// Módulo de elasticidad. Sólido A
extern double  EB;// Módulo de elasticidad. Sólido B
extern double  ET;// Módulo de elasticidad.
extern double  Emedia;// Factor de escalado = Módulo de elasticidad medio.

extern double  nuA;// Módulo de Poisson. Sólido A
extern double  nuB;// Módulo de Poisson. Sólido B
extern double  nuT;// Módulo de Poisson.
extern double  GT;// Módulo de elasticidad transversal.

extern double  tref;// Temperatura de referencia
extern double  alA;// Coeficiente de dilatación lineal. Sólido A
extern double  alB;// Coeficiente de dilatación lineal. Sólido B
extern double  alT;// Coeficiente de dilatación lineal.
extern double  lamA;// Conductividad térmica del sólido A
extern double  lamB;// Conductividad térmica del sólido B
extern double  diA;// Distorsionabilidad del sólido A
extern double  diB;// Distorsionabilidad del sólido B

extern int  TIM;// Tipo de contacto
extern double  HAmicro;// Microdureza. Sólido A
extern double  HBmicro;// Microdureza. Sólido B
extern double  sigA;// Rugosidad. Sólido A
extern double  sigB;// Rugosidad. Sólido B
extern double  mA;// Acabado superficial. Sólido A
extern double  mB;// Acabado superficial. Sólido B

extern double  M0;// Parámetro del gas
extern double  kg;// conductividad material en la interfase
extern double  tTIM;// Espesor de la interfase
extern double  ETIM;// Módulo de elasticidad interfase

extern int  tip;// material en despegue
extern double  hflu;// convección del gas en despegue
extern double  tflu;// temperatura del gas despegue convección forzada
extern double  cf;// Coeficiente de fricción

extern int  simXY;// Flag de simetría respecto del plano xOy
extern int  simXZ;// Flag de simetría respecto del plano xOz
extern int  simYZ;// Flag de simetría respecto del plano yOz
extern int  tpproE;// Flag de tipo de problema elastico
extern int  tpproT;// Flag de tipo de problema termico
extern int  tpproTE;// Flag de tipo de problema termoelastico
extern int  tpcarFP;// Flag de tipo de carga térmica. Fuentes puntuales
extern int  tpcarFL;// Flag de tipo de carga térmica. Fuentes lineales
extern int  tpcarFD;// Flag de tipo de carga térmica. Fuentes distribuidas
extern int  tpcarFC;// Flag de tipo de carga elástica. Fuerza centrífuga
extern int  tpcarPP;// Flag de tipo de carga elástica. Peso propio




extern double  AE[3][3];// Coeficientes A elasticos
extern double  BE[3][3];// Coeficientes B elasticos
extern double  AT;// Coeficiente A termico
extern double  BT;// Coeficiente B termico
extern double  CTE[3];// Coeficientes C termoelasticos
extern double  DTTE[3];// Coeficientes D termoelasticos
extern double  DTE[3][3];// Coeficientes D termoelasticos


extern double** AE_A;extern double** AE_B;
extern double** BE_A;extern double** BE_B;
extern double** AT_A;extern double** AT_B;
extern double** BT_A;extern double** BT_B;
extern double** CTE_A;extern double** CTE_B;
extern double** DTE_A;extern double** DTE_B;


extern double  a[3*MAX_EL][3*MAX_EL];// Matriz del sistema de ecuaciones
extern double  b[3*MAX_EL];// Términos independientes del sistema de ecuaciones
extern double  x[3*MAX_EL];// Incognitas del sistema de ecuaciones

extern int  nT;// Número total de grados de libertad para resolución del sistema
extern int  nTA;// Número total de elementos de A para resolución del sistema
extern int  nTpc;// Número total de elementos zona contacto para resolución del sistema

extern double  tempA[MAX_EL];// Temperatura en el contorno del cuerpo A
extern double  tempB[MAX_EL];// Temperatura en el contorno del cuerpo B
extern double  tempT;// Temperatura en los nodos
extern double  temp[MAX_EL];// Temperatura en el contorno
extern double  flujA[MAX_EL];// Flujo de calor en el contorno del cuerpo A
extern double  flujB[MAX_EL];// Flujo de calor en el contorno del cuerpo B
extern double  fluj[MAX_EL];// Flujo de calor en el contorno
extern double  flujT;// Flujo en los nodos
extern double  tf[MAX_EL];// Temperatura fluido convectivo
extern double  hf[MAX_EL];// Coeficiente de transmisión térmica por convección

extern double  RTC[MAX_EL];// Vector resistencia térmica de cálculo
extern double  RTCC[MAX_EL];// Vector resistencia térmica de comparación
extern int  chires;// chivato de convergencia resistencia termica

extern double  ve[MAX_EL];// Deslizamiento total relativo
extern double  anguv[MAX_EL];// Ángulo de deslizamiento
extern double  ve2[MAX_EL];// Componente 2 del deslizamiento relativo
extern double  ve3[MAX_EL];// Componente 3 del deslizamiento relativo

//extern double  ndCol[3];// Coordenadas del nodo de colocacion
extern double  nelInte;// Numero del elemento sobre el que se integra
//extern double  extr[4][3];// Extremos del elemento sobre el que se integra

extern long int  reg;// Registro del fichero de coeficientes

extern int  nmiter;// Número máximo de iteraciones
extern int  iter;// Número total de iteraciones
extern int  ifla;// Chivato para iteraciones
extern double  rsq;

extern double  cte1;// Constante en la integracion elastica
extern double  cte2;// Constante en la integracion elastica
extern double  cte3;// Constante en la integracion elastica
extern double  cte4;// Constante en la integracion termica
extern double  cte5;// Constante en la integracion termoelastica

extern FILE*   in1;// Fichero de entrada

extern int  out1;// Fichero de salida
extern int  out2;// Fichero de salida
extern int  out3;// Fichero de salida
extern int  out4;// Fichero de salida


extern int  in1s;// Fichero de entrada
extern int  in2s;// Fichero de entrada
extern int  in3s;// Fichero de entrada
extern int  in4s;// Fichero de entrada
extern int  in5s;// Fichero de entrada
extern int  in6s;// Fichero de entrada
extern int  in7s;// Fichero de entrada
extern int  in8s;// Fichero de entrada
extern int  in9s;// Fichero de entrada
extern int  in10s;// Fichero de entrada
extern int  out1s;// Fichero de salida
extern FILE*  out2s;// Fichero de salida
extern FILE*  out3s;// Fichero de salida

extern int  out11;// Fichero de salida
extern int  out12;// Fichero de salida

extern int  out13;// Fichero de salida
extern int  out14;// Fichero de salida

extern int  out15;// Fichero de salida
extern int  out16;// Fichero de salida

extern int out20;


#endif /* VARIABLES_H_ */

