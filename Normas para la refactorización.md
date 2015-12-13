### Es obligatorio acatar las siguientes *normas* para la refactorización del código secuencial:

* **Modificar los índices de las matrices:** en el código aparecen los índices acompañados de un -1 para empezar estos índices en 1.
  Nosotros trabajaremos comenzando los índices en 0.
  Ejemplo:
  ```
  for( i= 1; i<= nelpc; i++)
  {
    RTC[i-1]=0.0;
  }
  ```
  Después de la refactorización debe quedar así:
  ```
  for( i= 1; i<= nelpc; i++)
  {
    RTC[i]=0.0;
  }
  ```

* **No sustituir las variables globales por locales.**

* **No intentar convertir asignaciones estáticas en bucles.**
  Esto se refiere a ejemplos como el siguiente:
  ```
  subextr[1-1][4-1][1-1]=subextr[1-1][1-1][1-1];
  subextr[1-1][4-1][2-1]=subextr[1-1][1-1][2-1];
  subextr[1-1][4-1][3-1]=subextr[1-1][1-1][3-1];
  ```
  
* **Dividir en funciones:** modificar el código y añadir funciones que segmenten los bucles con claridad.

* **Los ficheros .h no deberían tener código implementado:** hay que dividir el código en .c y .h, 
  * La *cabecera* sería un .h que contiene:
    1. La signatura de las funciones, incluyendo su comentario correspondiente.
    2. Los include.
    3. Las variables.
    4. Las estructuras.
  * La *implementación* de la cabecera sería un .c 

* **El nombre de las funciones no deben estar en mayúsuculas.**

* **Las funciones en .c no deben llevar su comentario.**

* **Se debe eliminar el código comentado y las líneas inservibles o no utilizadas.**
