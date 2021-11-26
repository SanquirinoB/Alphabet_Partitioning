# Alphabet Partitioning [En desarrollo]

## De qué se trata esto
Este proyecto consiste en usar alphabet partitioning para comprimir un texto en lenguaje natural de modo de soportar no sólo descompresión sino tres funciones más: 
1. Etraer un snippet del texto (palabras i a la j)
2. Entregar todas las posiciones donde ocurre una palabra.
3. Entregar todas las posiciones donde ocurre una frase. 

Las listas invertidas se emulan con la operación select de la representación. 
Notar que deben parsear el texto en palabras, formando así un vocabulario donde a cada palabra se le asigne un número único. 
Este vocabulario se usa después para convertir palabras en números (para las consultas 2 y 3) y números en palabras (para la consulta 1). 
Se puede ordenar las palabras lexicográficamente para poder convertir palabras en números usando búsqueda binaria.

Se debe programar una pequeña interfaz (puede ser de comandos) 
que permita comprimir un texto (borrando el original, como un compresor) 
y dar la tasa de compresión obtenida (deben ser archivos de varios megas, al menos, para que la compresión se note).
Luego, dado un archivo comprimido, poder ejecutar secuencias de operaciones 1, 2 y 3. Además, medir y graficar los tiempos y espacios obtenidos para las distintas operaciones sobre algunos archivos de textos. Idealmente, comparar con la performance de algún índice invertido de dominio público.

El proyecto se sustenta en el uso de la librería [SDSL](https://github.com/simongog/sdsl-lite).

## Funcionalidades conseguidas

Se puede generar una estructura del tipo Alphabet Partitioning en base al path del texto a comprimir, y el path del alfabeto a utilizar. Vale destacar que el alfabeto deben ser palabras, los simbolos no son soportados y el espacio se considera separador.

Operaciones sobre la estructura:
1. string access(uint64_t i): Retorna la palabra en la posición i-esima del texto.
2. uint64_t rank(string word, uint64_t i): Retorna la cantidad de ocurrencias de la palabra (word) hasta la posición i-esima.
3. uint64_t select(string word, uint64_t i): Retorna la posición de la i-esima ocurrencia de la palabra (word).
4. void show_structure(): Pretty print de la estructura.
5. string get_snippet(uint64_t start, uint64_t end): Retorna un fragmento del texto entre las posiciones [start, end].
6. vector<uint64_t>* get_all_word_ocurrences(string word): Retorna un vector de las posiciones donde ocurre la palabra (word).
7. vector<uint64_t>* get_all_phrase_ocurrences(string phrase):Retorna un vector de las posiciones donde ocurre la frase (phrase).

## Desarrollo

Si deseas incluir algo o meti la pata con algo, me puedes contactar a fernandasanchirico@ug.uchile.cl o bien abrir una issue :)
Toda ayuda y sugerencia es bienvenida <3

#### Cosas pendientes
1. La estructura no soporta el uso de simbolos y depende del espacio como separador.
2. TODA la estructura está en el archivo example.cpp, quiero modularizarlo.
3. EL PROYECTO AUN NO ESTA TERMINADO.

#### Como compilar

Ejecutar el comando:
```sh
g++ -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib example.cpp -o example -lsdsl -ldivsufsort -ldivsufsort64
```
De tal forma que el ejecutable será:
```sh
./example
```

## License

GNU General Public License v3.0
