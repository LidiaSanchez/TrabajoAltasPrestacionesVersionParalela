### PLANTILLA PARA LA DOCUMENTACIÓN

* Nombre de la persona:

Iván de Paz Centeno (@ipazce00)

* Archivos modificados:

Todos.

* Líneas, o rango de líneas, con modificaciones:

Demasiadas para colocarlo aquí.

* Tiempos de ejecución analizados:
  *(Rellenar la siguiente tabla)*

LIBRERÍA PROBADA | MÉTODO UTILIZADO | Nº PROCESOS | TIEMPO COEFICIENTES | TIEMPO ECUACIONES | TIEMPO TOTAL
---------------- | ---------------- | ----------- | ------------------- | ----------------- | ------------
Versión inicial | Sin modificaciones | 1 | 17 segundos | 440 segundos | 457 segundos
NVCC | Adaptado para compilar en nvcc | 1 | 7 segundos | 240 segundos | 247 segundos
NVCC | Pasar información a una estructura y volcarlo en el dispositivo para usar bloques y threads  | -- | --


* Problemas encontrados (indicar para cada problema, las soluciones propuestas y la solución aplicada finalmente).

** Las variables globales no se pueden compartir directamente con el dispositivo. Se soluciońó mdiante el uso de una estructura que aglutine las variables necesarias (que son muchas).
** Inicializar una matriz de dos dimensiones de forma dinámica que permita copiarse con cudaMemCpy. Consultar el fichero funcionesCuda.cu para ver la solución.
** Refactorización en funciones locales para usar las variables de la estructura en lugar de las globales. Se solucinó utilizando inyecciones de variables al inicio de la función mediante el operador de paso por referencia de C++ (&).
** Recursos insuficientes en el dispositivo para trabajar con tanta información. No se encontró solución.

######Solución planteada:

 Se planteó asignar una estructura para cada cuerpo a la que el dispositivo tuviera acceso. Los campos que deberían ser privados para cada hilo estarían almacenados en un array, con una correspondencia entre los índices del array el identificador del Thread en CUDA. Se encontró el problema de que CUDA no dispone de memoria suficiente para realizar esta operación, por lo que no se completó.
 
  El planteamiento de la distribución consiste en determinar el punto de inicio y punto final del bucle principal en COEFICIENTES() y distribuirlo entre cada uno de los THREADS del bloque, de forma que cada uno ejecutará una sección del cálculo y lo almacenará en la posición correspondiente de la matriz de coeficientes.
  
  Al haber dos cuerpos, se planteó dividir cada uno en dos bloques dentro de CUDA con el mismo número de THREADS cada uno.
  
* Bibliografía utilizada:

Programación cuda en ()[http://docs.nvidia.com/cuda/cuda-c-programming-guide/#axzz3yC0DI8yo]

* Otros puntos que se quieran indicar para incluir en la documentación o en la presentación:

  La versión compilada con NVCC con un solo proceso realiza optimizaciones que reducen sustancialmente los tiempos de ejecución de la aplicación en comparación con la original sin modificaciones compilada con GCC.
