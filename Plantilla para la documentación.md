### PLANTILLA PARA LA DOCUMENTACIÓN

* Nombre de la persona:

Roberto de Castro Rodríguez (@roberdcr)

* Archivos modificados:

  - funcionesA.c
  - funcionesA.h
  - funcionesMain.c
  - variables.c
  - variables.h
  - solidos.c


* Líneas, o rango de líneas, con modificaciones:

  - 1ª Solución:

      Se ha modificado los siguientes archivos:
        - funcionesA.c la función COEFICIENTES. Líneas 573, 578, 631, 636, 761, 766, 891, 896, 1021, 1026, 1179, 1184, 1346, 1351, 1519 y 1524.
        - funcionesA.h las líneas [82-96]

  - 2ª Solución:

      Se ha modificado los siguientes archivos:
        - variables.c las líneas [199-204]
        - variables.h las líneas [214-219]
        - funcionesMain.c las líneas [379-403], [415-426], [504-527], [533-544]
        - funcionesA.c las líneas [544-559]
        - funcionesA.h las líneas [82-96]
        - solidos.c las líneas [32-40], 165, [173-174], 307 y 316

  - 3ª Solución:

      Se ha modificado los siguientes archivos:
        - variables.c las líneas [199-204]
        - variables.h las líneas [214-219]
        - funcionesMain.c las líneas [294-301], 429, [546-554]
        - funcionesA.c las líneas [561-564]
        - funcionesA.h las líneas [82-96]
        - solidos.c las 168

  *Nota*: Se han dejado comentadas las líneas añadidas o modificadas para cada solución en los archivos anteriormente indicados a excepción de la 3ª Solución.

* Tiempos de ejecución analizados:

LIBRERÍA PROBADA | MÉTODO UTILIZADO | Nº PROCESOS | TIEMPO COEFICIENTES | TIEMPO ECUACIONES | TIEMPO TOTAL
---------------- | ---------------- | ----------- | ------------------- | ----------------- | ------------
Versión Inicial  | Sin modificaciones | 1         | 26 segundos         | 545 segundos      | 571 segundos
---------------- | ---------------- | ----------- | ------------------- | ----------------- | ------------
MPI              |  Pragmas         | 1           | 26 segundos         | 545 segundos      | 571 segundos
---------------- | ---------------- | ----------- | ------------------- | ----------------- | ------------
MPI              |  Pragmas         | 10          | 26 segundos         | 545 segundos      | 571 segundos
---------------- | ---------------- | ----------- | ------------------- | ----------------- | ------------
MPI              |  Pragmas         | 50          | 26 segundos         | 545 segundos      | 571 segundos
---------------- | ---------------- | ----------- | ------------------- | ----------------- | ------------
MPI              |  MPI_Bcast & MPI_Gather | 1    | 26 segundos         | 545 segundos      | 571 segundos
---------------- | ---------------- | ----------- | ------------------- | ----------------- | ------------
MPI              |  MPI_Bcast & MPI_Gather | 10   |      --             |        --         |     --
---------------- | ---------------- | ----------- | ------------------- | ----------------- | ------------
MPI              |  MPI_Bcast & MPI_Gather | 50   |      --             |        --         |     --
---------------- | ---------------- | ----------- | ------------------- | ----------------- | ------------
MPI              |  Coeficientes Paralelos | 1    | 26 segundos         | 545 segundos      | 571 segundos
---------------- | ---------------- | ----------- | ------------------- | ----------------- | ------------
MPI              |  Coeficientes Paralelos | 2    | 13 segundos         |        --         |     --
---------------- | ---------------- | ----------- | ------------------- | ----------------- | ------------
MPI              |  Coeficientes Paralelos | 10   | 13 segundos         |        --         |     --
---------------- | ---------------- | ----------- | ------------------- | ----------------- | ------------

* Problemas encontrados (indicar para cada problema, las soluciones propuestas y la solución aplicada finalmente).

El objetivo es paralelizar el cálculo de los coeficientes para cada cuerpo.
Los coeficientes de un cuerpo se almacenan en 6 matrices que almacenan valores de tipo double. Estas matrices son:

  - AE: almacena los coeficientes elásticos del cuerpo
  - BE: almacena los coeficientes elásticos del cuerpo
  - AT: almacena los coeficientes térmicos del cuerpo
  - BT: almacena los coeficientes térmicos del cuerpo
  - CTE: almacena los coeficientes termoelásticos del cuerpo
  - DTE: almacena los coeficientes termoelásticos del cuerpo

Estos coeficientes se calculan por medio de la función **COEFICIENTES**, que se encuentra implementada en el archivo funcionesA.c
El código iterativo tarda unos 13 segundos en obtener los coeficientes para un cuerpo, tras la paralelización buscamos disminuir drásticamente este tiempo repartiendo las matrices anteriormente descritas entre todos los procesos de los que disponemos.

Inicialmente se han encontrado dos posibles soluciones a este problema:

######1ª Solución propuesta *Pragmas*:

A partir de la información obtenida en la URL de "Pragmas de OpenMPI", me he encontrado con que ésta libreria utilizada para compilar el código con la API de MPI nos provee de una seria de pragmas que facilitan la paralelización de nuestro código de forma sencilla. Estos pragmas son tratados por el compilador mpicc de forma que automáticamente se paraleliza el código tratado con el pragma.

En nuestro caso, como el objetivo es paralelizar los bucles for que recorren las posiciones de las matrices que tenemos que rellenar con los coeficientes calculados, tenemos que utilizar el pragma *#pramga ompi for*. Este pragma actua sobre el siguiente bucle for que se encuentre el compilador paralelizándolo. Para esta solución aplicamos este pragma a todos los bucles *for* que se recorren en el metodo **COEFICIENTES**.

Después de comprobar el resultado de los tiempos en la versión sin paralelizar y la versión con los pragmas, se ha observado que el tiempo es el mismo, por lo que se ha llegado a la conclusión de que el compilador de mpi que nos provee Caléndula no funciona, es decir, el pragma definido no es tratado por el compilador de mpicc y los bucles for no han sido paralelizados.

######2ª Solución propuesta *MPI_Bcast & MPI_Gather*:

Tras el fracaso de la primera solución se ha seguido con el uso de las llamadas que nos provee MPI:

  - MPI_Barrier
  - MPI_Bcast
  - MPI_Gather

Se ha pretendido paralelizar la llamada a la función coeficientes, el resto del códgio lo ejecuta el proceso maestro.

El algoritmo seguido es:

  1. Sincronizar procesos antes de la llamada a **COEFICIENTES**.

      Para asegurarnos de que los datos eran consistentes antes de hacer la llamada a la funcion **COEFICIENTES** para cada cuerpo (A y B) se han sincronizado los procesos por medio de MPI_Barrier para asegurarnos de que antes de realizar la llamada todos los procesos estaban esperando en el mismo punto.

  2. Enviar todos los datos necesarios al resto de procesos.

      A continuación por medio de MPI_Bcast mandamos desde el maestro al resto de procesos todos los datos necesarios para que cada proceso pueda realizar los cálculos debidos.

  3. Llamamos a la función **COEFICIENTES**.

      En esta función calculamos los indices de inicio y fin para los bucles for solo recorran un numero determinado de posiciones repartidas equitativamente entre todos los procesos.

  4. Sincronizar todos los procesos una vez terminada la llamada a la función **COEFICIENTES**.

      Después de la llamada a la funcion **COEFICIENTES**, volvemos a realizar la sincronización por medio de MPI_Barrier.

  5. Agrupar todos los coeficientes obtenidos en las matrices del proceso maestro.

      Para terminar, por medio de MPI_Gather enviamos al proceso maestro los coeficientes calculados por cada proceso esclavo.

Este algoritmo se utiliza para la obtención de los coeficientes de cada cuerpo.

Con el algoritmo planteado solo he conseguido que me funcionase con un único hilo, en el momento que lo ejecutaba con más de dos había alguna variable que llamaba a un puntero nulo haciendo que el programa parase con un error tipo Segmentation Fault.

Esta solución sería la ideal ya que hace uso de todos los recursos proporcionados por el sistema para realizar el cálculo de coeficientes, pero no he sido capaz de implementar correctamente el algoritmo anteriormente definido por medio del API que nos provee MPI.

######3ª Solución propuesta *Coeficientes Paralelos*:

Debido a los problemas al intenar implementar el algortimo para la segunda solución, se ha optado por probar una solución más sencilla que tambien paraleliza la obtención de los coeficientes pero de forma limitada haciendo uso únicamente de dos hilos como máximo.

Para esta solución planteada ser propone realizar el cálculo de los coeficientes para ambos cuerpos a la vez. No sería la solución más eficaz en cuanto a la optimización de recursos, puesto que únicamente utilizaremos dos procesos, uno para cada cuerpo.

El algoritmo sería el siguiente:

  1. El proceso maestro calcula los coeficientes para el cuerpo A.

  2. El proceso esclavo calcula los coeficientes para el cuerpo B.

  3. Sincronizamos los procesos antes de realizar el cálculo de ecuaciones.

  4. El proceso esclavo envía las matrices con los coeficientes del cuerpo B al proceso maestro para que este calcule las ecuaciones.

Con esta solución unicamente conseguiriamos reducir como máximo el tiempo del cálculo de los coeficientes a la mitad. En lugar de 26 segundos en total a 13.


* Bibliografía utilizada:

- [Pragmas de OpenMPI](http://www.compunity.org/events/pastevents/ewomp2004/boku_sato_matsubarea_takahashi-13.pdf)
- [Documentación OpenMpi v.1.6 (versión en Caléndula)](http://www.open-mpi.org/doc/v1.6/)
- [Paralelizar bucles for en MPI](http://wiki.ccs.tulane.edu/index.php5/Parallel_Loop_MPI)
- [Ayuda MPI](http://lsi.ugr.es/jmantas/pdp/ayuda/mpi_ayuda.php)


* Otros puntos que se quieran indicar para incluir en la documentación o en la presentación:

  - La mayour dificultad que me he encontrado a la hora de realizar la paralelización del código es el uso de las variables globales.
    Como el cálculo de los coeficientes llama a varias funciones *(INTEGRA, TRANSFORMA, ALACENA)* y estas a su vez a otras *(ANALITICA, SUBDIVIDE...)* que hacen uso de las mismas variables, los fallos que he tenido al intentar paralelizar el código es posible que se hayan producido a variables que estaban corruptas o no estaban inicializadas como es debido y de ahí venir los fallos por punteros nulos que no he sido capaz de corregir.

    La solución más apropiada a la problemática de calcular los coeficientes será la 2ª ya que el algoritmo descrito aprovecha los recursos de todos los procesos de los que el programa dispone, no obstante, es la más difícil de aplicar porque tenemos que estar seguros de que en todos los procesos todas las variables de las que se hace uso para el cálculo tienen el valor correcto.
