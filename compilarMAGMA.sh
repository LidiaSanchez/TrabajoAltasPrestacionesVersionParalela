#!/bin/bash
LD_LIBRARY_PATH=/usr/local/lib:/opt/cuda/lib64:/opt/magma/lib:/usr/lib
export LD_LIBRARY_PATH
gcc -Wall -DADD_ -I/opt/magma/include -I/opt/cuda/include -c src/variables.c src/solidos.c src/funcionesPersonales.c src/funcionesD.c src/funcionesA.c src/funcionesMain.c src/funcionesB.c src/funcionesC.c
gcc -Wall -o solidos variables.o solidos.o funcionesPersonales.o funcionesD.o funcionesA.o funcionesMain.o funcionesB.o funcionesC.o -L/opt/magma/lib -L/opt/cuda/lib64 -L/usr/lib -lmagma -lcublas -lcudart -lcblas -lm
rm -f *.o
#gcc -o solidos src/variables.c src/solidos.c src/funcionesPersonales.c src/funcionesD.c src/funcionesA.c src/funcionesMain.c src/funcionesB.c src/funcionesC.c -lm -lmagma -lcublas -lcudart -lopenblas
