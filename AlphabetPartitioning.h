// Secuencia basada en permutacion
#include <sdsl/wt_gmr.hpp>
// Wavelet Tree
#include <sdsl/wavelet_trees.hpp>
#include <iostream>
#include <map>

using namespace std;
using namespace sdsl;

// TODO: Evaluar si se puede hacer mejor la definicion de clase. Porque usar template?

class Alphabet_Partitioning {

    private:
    // Transcripcion de S con la clase de los caracteres.
    wt_huff_int<> K;
    // Arreglo de clases del alfabeto
    wt_huff_int<> C;
    // Alfabeto a utilizar
    // Por ahora toleraremos solo alfabeto definido por ASCII imprimibles 
    // [32, 126] -> [1, 95]
    const string alphabet = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    const int alphabet_size = alphabet.length();

    protected:
    // Con esta funcion transformaremos los caracteres a su valor int, minimo = 1
    int to_int(char c);
    // Con esta funcion transformaremos los int a su valor equivalente al alfabeto usado
    char to_char(int i);
    // Computamos el piso del logaritmo base 2 de un int
    // Nolotar que se computa en O(log(n))
    int floor_log2(int n);

    public:
    // Constructor
    Alphabet_Partitioning(string S);
    // TODO: Hacer buenas definiciones
    string access(int i);
    int rank(char c, int i);
    int select(char c, int i);
};