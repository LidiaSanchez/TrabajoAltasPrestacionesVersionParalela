### PLANTILLA PARA LA DOCUMENTACIÓN

* Nombre de la persona: Cristina Blanco Fernández


* Archivos modificados: funcionesMain.c, funcionesMain.h


* Líneas, o rango de líneas, con modificaciones: 237 en funcionesMain.c, 10 en funcionesMain.h

* Tiempos de ejecución analizados:
  *(Rellenar la siguiente tabla)*

LIBRERÍA PROBADA | MÉTODO UTILIZADO | TIEMPO DE EJECUCIÓN
---------------- | ---------------- | -------------------
Versión inicial | Sin modificaciones | 581 segundos
---------------- | ---------------- | -------------------
OpenMP | parallel for | 379 segundos


* Problemas encontrados (indicar para cada problema, las soluciones propuestas y la solución aplicada finalmente). 
- El primer problema que me he encontrado ha sido la gran dificultad de paralelizar los bucles for del código. 
Casi todos los bucles se encuentran dentro de otros bucles if o sentencias similares y al intentar paralelizarlo, o el tiempo
de ejecución no mejoraba o la estructura del bucle no es válida para paralelizar con OpenMP.
Más concretamente me aparecía el error al paralelizar bucles for contenidos en bucles if con el siguiente error: "ramificación
inválida para/desde un bloque estructurado". No he encontrado solución alguna por lo que no he podido paralelizar los bucles
for contenidos en bucles if.
- El segundo problema que me he encontrado ha sido con los tiempos de ejecución. Ya sea por la estructura de Caléndula o por un 
problema mío, un mismo código ejecutado dos veces sin ninguna modificación me daba tiempos con diferencias de hasta 200 segundos.

Estos dos problemas han generado un retraso en mi trabajo bastante grande por lo que finalmente he podido probar pocas cosas de 
las cuáles solo una merecía tenerla en cuenta ya que el resto de paralelizaciones probadas mejoraban el código uno o dos segundos.

* Bibliografía utilizada:
Ninguna.

* Otros puntos que se quieran indicar para incluir en la documentación o en la presentación:
