#include "funcionesPersonales.h"
#include "variables.h"

void leeEntero(FILE* fp,int* r)
{
    fscanf(fp,"%d",&(*r));
}

void leeDouble(FILE* fp,double* r)
{
    char temp[50];
    fscanf(fp,"%s",temp);

#ifdef DEBUG
    printf("[funcionesPersonales.c] leeDouble(): %s\n", temp);
#endif
    char *endp;
    *r = strtod(temp,&endp);
    if(*endp == 'D' || *endp == 'd')
    {
        *endp = 'e';
        *r = strtod(temp,&endp);
    }
    
    double test = strtod(temp, &endp);
#ifdef DEBUG
    printf("[funcionesPersonales.c] leeDouble(): extracted double: %f; %f\n", *r, test);
#endif
}

void leeCadena(FILE* fp,char* cad)
{
    fgets(cad,80,fp);
}

void leeLinea(FILE* fp)
{
    char temp[80];
    fgets(temp,80,fp);
#ifdef DEBUG
    printf("[funcionesPersonales.c] leeLinea() value: \"%s\"\n", temp);
#endif
}
