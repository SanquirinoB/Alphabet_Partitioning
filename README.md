# Alphabet_Partitioning

Este proyecto consiste en usar alphabet partitioning para comprimir un texto en lenguaje natural de 
modo de soportar no sólo descompresión sino tres funciones más: 
(1) extraer un snippet del texto (palabras i a la j)
(2) entregar todas las posiciones donde ocurre una palabra
(3) entregar todas las posiciones donde ocurre una frase mediante intersectar las listas invertidas de la palabra. 

Las listas invertidas se emulan con la operación select de la representación. 
Notar que deben parsear el texto en palabras, formando así un vocabulario donde a 
cada palabra se le asigne un número único. 
Este vocabulario se usa después para convertir palabras en números (para las consultas 2 y 3) 
y números en palabras (para la consulta 1). 
Se puede ordenar las palabras lexicográficamente para poder convertir palabras en números 
usando búsqueda binaria.

Se debe programar una pequeña interfaz (puede ser de comandos) 
que permita comprimir un texto (borrando el original, como un compresor) 
y dar la tasa de compresión obtenida (deben ser archivos de varios megas, al menos, para que la compresión se note).
 Luego, dado un archivo comprimido, poder ejecutar secuencias de operaciones 1, 2 y 3. Además, medir 
y graficar los tiempos y espacios obtenidos para las distintas operaciones sobre algunos archivos 
de textos. Idealmente, comparar con la performance de algún índice invertido de dominio público.
