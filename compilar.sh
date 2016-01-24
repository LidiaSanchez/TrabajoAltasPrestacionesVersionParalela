#!/bin/bash
nvcc -g -O0 -o solidos src/variables.cu src/solidos.cu src/funcionesPersonales.cu src/funcionesD.cu src/funcionesA.cu src/funcionesMain.cu src/funcionesB.cu src/funcionesC.cu src/funcionesCuda.cu src/funcionesCheck.cu -lm -rdc=true

