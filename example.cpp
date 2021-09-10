// Secuencia basada en permutacion
#include <sdsl/wt_gmr.hpp>
// Wavelet Tree
#include <sdsl/wavelet_trees.hpp>
#include <iostream>
#include <map>

using namespace std;
using namespace sdsl;

// Por ahora toleraremos solo alfabeto definido por ASCII imprimibles 
// [32, 126] -> [0, 94]
const string alphabet = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
int to_int(char c){
    return c - ' ';
}

char to_char(int i){
    return alphabet[i];
}

// TODO: Evaluar si se puede hacer mejor la definicion de clase. Porque usar template?

class Alphabet_Partitioning {
    // Transcripcion de S con la clase de los caracteres.
    const wt_huff_int<> K;
    // Arreglo de clases del alfabeto
    const wt_huff_int<> C;
    public:
    // Constructor
    Alphabet_Partitioning(string S);

    // Metodos de acceso primitivos
    string access(int i);
    int rank(char c, int i);
    int select(char c, int i);
};

Alphabet_Partitioning::Alphabet_Partitioning(string S)
{
    // Identificamos el tamano del texto y alfabeto
    int n = S.length();
    int sigma_size = 95;
    // Definimos e inicializamos nuestro arreglo de caracteres y freciencias
    vector<pair<int, int>> *F = new vector<pair<int, int>>;
    for(int i = 0; i < sigma_size; i++) (*F).push_back(make_pair(0, i));
    // Para cada caracter en el alfabeto, computamos su frecuencia dentro de S
    for(int i = 0; i < n; i++) (*F)[to_int(S[i])].first += 1;
    // Ordenamos el alfabeto por frecuencia en orden descendente
    sort((*F).rbegin(), (*F).rend());
    // Alocamos el arreglo con el id de las clases
    char C[sigma_size];
    // Almacenamos el tamanio de cada clase
    int N[int(log(sigma_size))];
    int ln_sigma = floor(log(sigma_size));

    for(int l = 0; l < ln_sigma; l++) N[l] = 0;
    int l;

    // TODO: Porque la clase 0 me queda con dos integrantes?
    for(int j = 1; j < sigma_size; j++)
    {
        l = floor(log(j));
        C[to_int((*F)[j].second)] = to_char(l);
        std::cout << "A la clase " << l << endl;
        std::cout << "le agregamos la frecuencia " << (*F)[j].first << endl;
        std::cout << "del caracter " << to_char((*F)[j].second) << endl;
        N[l] += (*F)[j].first;
    }

    std::cout << sigma_size << ln_sigma << endl;
    for(int i = 0; i < ln_sigma; i++){
        std::cout << N[i] << i << endl;
    }
    //wavelet
    // Liberamos memoria
    delete[]F;
    
    
}

int main(){
    string exS = "to be or not to be, that is the question";
    string alf = "abehinoqrstu ,";
    const string alphabet = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

    Alphabet_Partitioning cosa(exS);
    return 0;
}