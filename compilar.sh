#!/bin/bash
nvcc -o solidos src/variables.cu src/solidos.cu src/funcionesPersonales.cu src/funcionesD.cu src/funcionesA.cu src/funcionesMain.cu src/funcionesB.cu src/funcionesC.cu src/funcionesCuda.cu -lm
