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
int translate(char c){
    return c - ' ';
}

// TODO: Evaluar si se puede hacer mejor la definicion de clase. Porque usar template?

class Alphabet_Partitioning {
    // Transcripcion de S con la clase de los caracteres.
    const wt_huff_int<> K;
    // Arreglo de clases del alfabeto
    const wt_huff_int<> C;
    
    public:
    // Constructor
    void build(string S, string alf);
    Alphabet_Partitioning(){
        
    }

    // Metodos de acceso primitivos
    string access(int i);
    int rank(char c, int i);
    int select(char c, int i);
};

void Alphabet_Partitioning::build(string S, string alf){
    // Identificamos el tamano del texto y alfabeto
    int n = S.length();
    int sigma_size = alf.length();
    // Definimos e inicializamos nuestro arreglo de caracteres y freciencias
    /*map<pair<char,int>> F;
    for(int i = 0; i < sigma_size; i++){
        F.push_back(make_pair(alf[i], 0));
    }

    // TESTEO
    for(int i = 0; i < sigma_size; i++){
        std::cout << F[i].first << F[i].second << endl;
    }
    // TESTEO

    for(int i = 0; i < n; i++){
        std::cout << F[i].first << F[i].second << endl;
    }*/
}

int main(){
    string exS = "to be or not to be, that is the question";
    string alf = "abehinoqrstu ,";
    for(int i = 0; i < exS.length(); i++){
        std::cout << translate(exS[i]) << endl;
    }

    //Alphabet_Partitioning cosa;
    // cosa.build(exS, alf);
    return 0;
}