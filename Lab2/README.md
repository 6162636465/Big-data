## Big-data
El codigo del archivo ***GeneradorDocumentos*** es un scrip para copiar los archivos que usaremos para la tarea de Índice Invertido
estos archivos an sido extraidos de ***https://www.kaggle.com/datasets/metron/public-files-of-religious-and-spiritual-texts?resource=download***

![Cantidad elementos](Imagenes/Esquemaoriginall.png)
la cantidad de elementos que me genero fue de:


![Cantidad elementos](Imagenes/CantidadElementos.png)


y el peso final que me genero tras 697 copias es de:


![Cantidad elementos](Imagenes/PesoFinal.png)

para la clasificacion de elementos para nuestro index usaremos la similitud coseno cuya formula es  

![Cantidad elementos](Imagenes/Formula.png)

El programa encuentra el documento más cercano calculando la similitud coseno entre vectores de frecuencia. Funciona utilizando la idea de los búferes por thread, donde cada hilo procesa un documento. Dado que ahora el problema no es el peso sino la cantidad de elementos, cambiamos de metodología y obtenemos los siguientes resultados.

![Cantidad elementos](Imagenes/ResultadosF.png)
