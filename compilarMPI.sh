#!/bin/bash
mpicc -o solidos src/variables.c src/solidos.c src/funcionesPersonales.c src/funcionesD.c src/funcionesA.c src/funcionesMain.c src/funcionesB.c src/funcionesC.c -lm
