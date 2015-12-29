### PLANTILLA PARA LA DOCUMENTACIÓN

* Nombre de la persona:
Roberto de Castro Rodríguez

* Archivos modificados:

-funcionesA.c
-variables.c
-solidos.c
-variables.h

* Líneas, o rango de líneas, con modificaciones:

* Tiempos de ejecución analizados:

LIBRERÍA PROBADA | MÉTODO UTILIZADO | TIEMPO DE EJECUCIÓN
---------------- | ---------------- | -------------------
OpenMPI             Pramgas           13 segundos para A y otros 13 segundos para B = 26 segundos
-----------------|------------------|--------------------
OpenMPI             División bucles
-----------------|------------------|--------------------
Versión inicial | Con modificaciones |
26 seg Total      26 seg Total
--------------- | ------------------ |
26 seg Total      

* Problemas encontrados (indicar para cada problema, las soluciones propuestas y la solución aplicada finalmente).

El problema principal es dividir los bucles for para el número de procesos que se están utilizando en la ejecución.
Para ello se han encontrado dos formas de hacerlo por medio de MPI, obteniendo la posición inicial y final en las que se deben de calcular los coeficientes o utilizar pragmas que hacen esto automáticamente.

Después de comprobar el resultado de los tiempos en la versión sin paralelizar y la versión con los pragmas, se ha observado que el tiempo es el mismo, por lo que se ha llegado a la conclusión de que el compilador de mpi que nos provee Caléndula no funciona, por tanto se ha llevado a cabo la división de los bucles.

* Bibliografía utilizada:

-Pragmas de OpenMPI
http://www.compunity.org/events/pastevents/ewomp2004/boku_sato_matsubarea_takahashi-13.pdf
-Documentación OpenMpi v.1.6 (versión en Caléndula)
http://www.open-mpi.org/doc/v1.6/
-Paralelizar bucles for en MPI:
http://wiki.ccs.tulane.edu/index.php5/Parallel_Loop_MPI


* Otros puntos que se quieran indicar para incluir en la documentación o en la presentación:

Buscando la mejor forma de poder paralelizar con MPI los bucles for en los que se realiza el cálculo de los coeficientes,líneas 563, 566, 622, 625, 752, 755, 882 885, 1012, 1015, 1164, 1167, 1326, 1329, 1516 y 1519. He encontrado un artículo sobre la librería OpenMPI utilizada para compilar nuestro código, la cual provee una serie de pragmas que facilitan el uso de la librería paralelizando el código.

Sobre nuestro código, la librería OpenMPI ofrece el pragma: #pramga ompi for. Este pragma lo que hace es que el compilador de mpi paraleliza automáticamente la siguiente línea con el bucle for de modo que facilita mucho la paralelización del código.
