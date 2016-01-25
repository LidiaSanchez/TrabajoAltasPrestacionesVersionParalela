#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "cublas_v2.h"
#include "magma.h"
#include "magma_lapack.h"

#define min(a, b) (a < b) ? a : b

#define A_ROWS 3
#define A_COLUMS 3

#define B_ROWS 3
#define B_COLUMS 1

#define CPU 1
#define GPU 1

void toColumWise(double *M, int rows, int colums) {
	int i, j;
	double *tmp;
	magma_dmalloc_cpu(&tmp, rows * colums);
	for(j = 0; j < colums; j++) {
		for(i = 0; i < rows; i++) {
			tmp[j * colums + i] = M[i * colums + j];
		}
	}

	for(i = 0; i < rows*colums; i++) {
		M[i] = tmp[i];
	}
}

void printMatrix(double *M, int rows, int colums) {
	int i = 0, j = 0, cnt = 0;
	for(i = 0; i < rows; i++) {
		printf("| ");
		for(j = 0; j < colums; j++) {
			printf("%lf ", M[cnt++]);
		}
		printf("|\n");
	}
	printf("\n\n");
}

void fillMatrixA(double *A) {
	A[0] = 3;
	A[1] = 4;
	A[2] = 2;
	A[3] = 1;
	A[4] = 3;
	A[5] = 2;
	A[6] = 4;
	A[7] = 7;
	A[8] = 1;

	printf("Printing Matrix A\n");
	printMatrix(A, A_ROWS, A_COLUMS);

	toColumWise(A, A_ROWS, A_COLUMS);
}

void fillMatrixB(double *B) {
	B[0] = 20;
	B[1] = 13;
	B[2] = 30;

	printf("Printing Matrix B\n");
	printMatrix(B, B_ROWS, B_COLUMS);
}

void alternativeMethod() {
	//double a[3][3] = {{3,1,4},{4,3,7},{2,2,1}};
	double a[3][4] = {{3,4,2},{1,3,2},{4,7,1}};
	double b[3] = {20,13,30};
	double x[3] = {0,0,0};
	int nT = 3;

	int i, j, np, piv, maxi, k;
	//MATRIZ AMPLIADA
    for( i=1; i<=nT; i++)        
    {
        a[i-1][nT+1-1]=b[i-1];
    }
    //inicializamos variables
    for( i=1; i<=nT; i++)        
    {
        x[i-1]=0.0;
    }

    //busqueda del pivote
    for( i=1; i<=nT-1; i++)        
    {
        piv=0.0;
        for( j=i; j<=nT; j++)            
        {
            if(fabs(a[j-1][i-1]) > piv)                
            {
                piv=fabs(a[j-1][i-1]);
                np=j;
            }
        }
        //chequeo matriz singular
        if(piv == 0.0 )            
        {
            printf("######### matriz singular #########\n");
            return;
        }
        //intercambio de filas
        for( j=i; j<=nT+1; j++)            
        {
            maxi=a[np-1][j-1];
            a[np-1][j-1]=a[i-1][j-1];
            a[i-1][j-1]=maxi;
        }
        for( j=i+1; j<=nT; j++)            
        {
            for( k=i+1; k<=nT+1; k++)                
            {
                a[j-1][k-1]=a[j-1][k-1]-a[i-1][k-1]*a[j-1][i-1]/a[i-1][i-1];
            }
        }
    }
    printf(" COMIENZA PROCESO DE SOLUCION\n");

    //resolucion hacia atras
    for( i=nT; i>=1; i+=-1)        
    {
        maxi=0.0;
        if(i != nT)            
        {
            for( j=i; j<=nT; j++)                
            {
                maxi=maxi+a[i-1][j-1]*x[j-1];
            }
        }
        x[i-1]=(a[i-1][nT+1-1]-maxi)/a[i-1][i-1];

    }
    //open(unit=out20,file='gauss.dat')
    //do i=1,nT
    //write(out20,*)i,x(i)
    //enddo
    //close(unit=out20)

    printf("Vector x: \n");
    for( i=0; i< nT; i++) {
        printf("%lf ", x[i]);
    }
    printf("\n");
    return;
}

int main() {
	if(A_ROWS != A_COLUMS || B_COLUMS > B_ROWS || A_COLUMS != B_ROWS) {
		printf("Se tiene que poder satisfacer A*X=B\n");
		exit(0);
	}

	alternativeMethod();

	printf("Inicializando MAGMA...\n");
	magma_init();

	if(CPU) {
		printf("MAGMA con CPU...\n");

		double *A, *B;
		magma_int_t *ipiv;
		magma_int_t info = 0;

		magma_dmalloc_cpu(&A, A_ROWS * A_COLUMS);
		magma_dmalloc_cpu(&B, B_ROWS * B_COLUMS);
		magma_imalloc_cpu(&ipiv, min(A_ROWS, A_COLUMS));

		fillMatrixA(A);		
		fillMatrixB(B);

		clock_t startCPU = clock();
	    magma_dgesv( A_COLUMS, B_COLUMS, A, A_ROWS, ipiv, B, B_ROWS, &info );
	    int msec = (clock() - startCPU) * 1000 / CLOCKS_PER_SEC;

	    if ( info != 0 ) {
	        fprintf( stderr, "magma_dgesv failed with info=%d\n", info );
	    } else {
			printf("Printing Matrix A\n");
			printMatrix(A, A_ROWS, A_COLUMS);

		    printf("Printing Matrix X\n");
			printMatrix(B, B_ROWS, B_COLUMS);

			printf("Time taken %d seconds %d milliseconds\n\n", msec/1000, msec%1000);
	    }

		magma_free_cpu( A );
	    magma_free_cpu( B );
	    magma_free_cpu( ipiv );
	}

	if(GPU) {
		printf("MAGMA con GPU...\n");

		double  *ACPU, *BCPU, *AGPU, *BGPU;
		magma_int_t *ipivGPU;
		magma_int_t infoGPU = 0;

		magma_dmalloc_cpu(&ACPU, A_ROWS * A_COLUMS);
		magma_dmalloc_cpu(&BCPU, B_ROWS * B_COLUMS);
		magma_dmalloc(&AGPU, A_ROWS * A_COLUMS);
		magma_dmalloc(&BGPU, B_ROWS * B_COLUMS);
		magma_imalloc_cpu(&ipivGPU, min(A_ROWS, A_COLUMS));

		fillMatrixA(ACPU);		
		fillMatrixB(BCPU);

		magma_dsetmatrix(A_ROWS, A_COLUMS, ACPU, A_ROWS, AGPU, A_ROWS);
		magma_dsetmatrix(B_ROWS, B_COLUMS, BCPU, B_ROWS, BGPU, B_ROWS);

		clock_t startGPU = clock();
	    magma_dgesv_gpu( A_COLUMS, B_COLUMS, AGPU, A_ROWS, ipivGPU, BGPU, B_ROWS, &infoGPU );
	    int msec = (clock() - startGPU) * 1000 / CLOCKS_PER_SEC;

	    if ( infoGPU != 0 ) {
	        fprintf( stderr, "magma_dgesv failed with info=%d\n", infoGPU );
	    } else {
		   	magma_dgetmatrix(A_ROWS, A_COLUMS, AGPU, A_ROWS, ACPU, A_ROWS);
		   	magma_dgetmatrix(B_ROWS, B_COLUMS, BGPU, B_ROWS, BCPU, B_ROWS);


			printf("Printing Matrix AGPU\n");
			printMatrix(ACPU, A_ROWS, A_COLUMS);

		    printf("Printing Matrix XGPU\n");
			printMatrix(BCPU, B_ROWS, B_COLUMS);

	    	printf("Time taken %d seconds %d milliseconds\n\n", msec/1000, msec%1000);
	    }

		magma_free_cpu( ACPU );
	    magma_free_cpu( BCPU );
	    magma_free_cpu( ipivGPU );
		magma_free( AGPU );
	    magma_free( BGPU );
	}

	printf("Finalizando MAGMA...\n");
    magma_finalize();
}


/*
	X=2		3*(2)+4*(3)+2*(1)=6+12+2=20
	Y=3		1*(2)+3*(3)+2*(1)=2+9+2=13
	Z=1		4*(2)+7*(3)+1*(1)=8+21+1=30
*/
