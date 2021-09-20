// Secuencia basada en permutacion
#include <sdsl/wt_gmr.hpp>
// Wavelet Tree
#include <sdsl/wavelet_trees.hpp>
#include <iostream>
#include <map>

using namespace std;
using namespace sdsl;

// Por ahora toleraremos solo alfabeto definido por ASCII imprimibles 
// [32, 126] -> [1, 95]
const string alphabet = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
/*
Llevamos un int a su version char
*/
int to_int(char c){
    return c - ' ' + 1;
}
/*
Llevamos un char a su version int
*/
char to_char(int i){
    return alphabet[i - 1];
}
/*
Computamos el piso del logaritmo base 2 de un int
notar que se computa en O(log(n))
*/
int floorLog2(int n)
{   
    int ans = -1;
    while(n != 0)
    {
        n = n >> 1;
        ans += 1;
    }
    return ans;
}

// TODO: Evaluar si se puede hacer mejor la definicion de clase. Porque usar template?

class Alphabet_Partitioning {
    private:
    // Transcripcion de S con la clase de los caracteres.
    wt_huff_int<> K;
    // Arreglo de clases del alfabeto
    wt_huff_int<> C;
    public:
    // Constructor
    Alphabet_Partitioning(string S);
    // Metodos de acceso primitivos
    string access(int i);
    int rank(char c, int i);
    int select(char c, int i);
};

Alphabet_Partitioning::Alphabet_Partitioning(string SRaw)
{
    // Identificamos el tamano del texto y alfabeto
    string S = " " + SRaw;
    int n = S.length() + 1;
    int sigma_size = 95;
    int log2_sigma = floorLog2(sigma_size);

    // Definimos e inicializamos nuestro arreglo de caracteres y freciencias
    vector<pair<int, int>> *F = new vector<pair<int, int>>;
    (*F).push_back(make_pair(-1, NULL));
    // Incluimos los 95 caracteres con frecuencia 0, len(F) = 96, indexa en 1
    for(int i = 1; i <= sigma_size; i++) (*F).push_back(make_pair(0, i));

    // Para cada caracter en el alfabeto, computamos su frecuencia dentro de S
    for(int i = 1; i <= n ; i++) (*F)[to_int(S[i])].first += 1;
    // Ordenamos el alfabeto por frecuencia en orden descendente
    sort((*F).rbegin(), (*F).rend());
    
    // Alocamos el arreglo con el id de las clases
    char valC[sigma_size + 1];
    // Inicializamos el tamanio de cada clase
    int N[log2_sigma + 1];
    for(int l = 1; l <= log2_sigma; l++) N[l] = 0;

    int l;

    for(int j = 1; j <= sigma_size; j++)
    {   
        l = floor(log(j));
        valC[to_int((*F)[j].second)] = to_char(l);
        N[l] += (*F)[j].first;
    }
    // Liberamos memoria
    delete F;
    // Instanciamos C en formato de Wavelet Tree huffman shaped
    construct(C, valC);
    
    char valK[n];
    int *LClass[log2_sigma];

    for (int l = 0; l < log2_sigma; l++)
    {   
        int NL[N[l]];
        LClass[l] = NL;
        N[l] = 0;
    }
    //Aqui hay un segmentation fault :c
    // Resulta que S deja de existir aqui, este es el segmentation fault
    std::cout << S << endl;
    
    for (int i = 1; i <= n; i ++)
    {   
        std::cout << S << endl;
        //l = C[to_int(S[i])];
        //valK[i] = to_char(l);
        //N[l] += l;
        //LClass[N[l]] = &(C.rank(S[i]));
    }





    //wavelet
    
    
    
}



int main(){
    string exS = "to be or not to be, that is the question";
    string alf = "abehinoqrstu ,";
    const string alphabet = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

    Alphabet_Partitioning cosa(exS);
   return 0;
}