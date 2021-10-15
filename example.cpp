#include <sdsl/wavelet_trees.hpp>
#include <iostream>
#include <map>

using namespace std;
using namespace sdsl;

// TODO: Evaluar si se puede hacer mejor la definicion de clase. Porque usar template?

class Alphabet_Partitioning {

    private:
    // Transcripcion de S con la clase de los caracteres.
    wt_huff_int<rrr_vector<63>> K;
    // Arreglo de clases del alfabeto
    wt_huff_int<rrr_vector<63>> C;
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

Alphabet_Partitioning::Alphabet_Partitioning(string SRaw)
{   
    // Generamos variables de uso general
    //  Capturamos el tamanio del texto
    int n = SRaw.length();
    //   Corregimos el texto para indexar en 1
    string S = " " + SRaw;
    //  Computamos el logaritmo base 2 del tamanio del alfabeto
    int log2_sigma = floor_log2(alphabet_size);

    // Definicion de F
    //  Definimos  nuestro vector de pares [(int) frecuencia, (int) caracter]
    vector<pair<int, int>> *F = new vector<pair<int, int>>;

    // Inicialicion de F
    //  Ingresamos un par inicial dummy para la indexacion en 1
    (*F).push_back(make_pair(-1, -1));
    //  Incluimos los 95 caracteres con frecuencia 0
    for(int i = 1; i <= alphabet_size; i++) (*F).push_back(make_pair(0, i));
    //  Para cada caracter en el texto, sumamos 1 a su frecuencia respectiva
    for(int i = 1; i <= n ; i++) (*F)[to_int(S[i])].first += 1;
    //  Ordenamos el alfabeto por frecuencia en orden descendente
    //   (!) Nolotar que hora F indexa en 0 hasta 94, pues el dato dummy inicial se fue al final
    sort((*F).rbegin(), (*F).rend()); 
    
    // Definicion de N
    //  Arreglo de enteros, del tamanio del logaritmo base 2 del tamanio del alfabeto
    //   (!) Le sumamos 1 para que indexe en 1
    int N[log2_sigma + 1];

    // Inicializacion de N
    //  Se setean todos sus valores a 0
    for(int l = 1; l <= log2_sigma; l++) N[l] = 0;

    // Definicion de val_C
    //  Arreglo de caracteres, del tamanio del alfabeto.
    //   (!) Le sumamos 1 para que indexe en 1
    // char val_C[alphabet_size + 1];
    int_vector<64> val_C(alphabet_size, 0);
    //  Generamos una variable auxiliar para almacenar la clase de un caracter
    int l;
    
    // Inicializacion de val_C
    //  Por cada elemento del alfabeto 
    for(int j = 1; j <= alphabet_size; j++)
    {   
        // Computamos su clase (l)
        l = floor_log2(j);
        // Para su posicion en val_C, le asignamos su clase codificado en formato char
        val_C[(*F)[j-1].second - 1] = l;
        // Incrementamos el tamanio de la clase l en la frecuencia del caracter asociado
        N[l+1] += (*F)[j-1].first;
    }
    
    // Eliminacion de F
    delete F;

    // Inicializacion de C
    // A partir de val_C, generamos C en formato de Wavelet Tree huffman shaped
    construct_im(C, val_C, 8);
    
    // Definicion de L_class
    //  Arreglo de punteros, del tamanio del logaritmo base 2 del tamanio del alfabeto
    //  En cada elemento almacenamos un arreglo de int con la ocurrencia de caada caracter
    //  segun su clase
    int *L_class[log2_sigma+1];

    // Inicializacion de L_class
    //  Dentro del alfabeto [1, log2_sigma]
    for (int l = 1; l <= log2_sigma; l++)
    {   
        // Instanciamos un arreglo del tamanio de la clase l
        int n_l[N[l]];
        // Asociamos el arreglo al arreglo de punteros L_class
        L_class[l] = n_l;
        // Definimos el tamanio de la clase l como 0 (limpiamos)
        N[l] = 0;
    }
    
    // Deficion val_K
    //  Arreglo de char, de tamanio equivalente al texto, almacena el equivalente de S en formato de clases
    //   (!) Le sumamos 1 para que indexe en 1
    char val_K[n + 1];

    // Inicializacion de val_K
    //  Por cada caracter en el texto
    std::cout << "Alphabet_size es " << alphabet_size<< endl;
    std::cout << "C.size()="<< C.size() << endl;
    std::cout << "C.sigma ="<< C.sigma << endl;
    for (int i = 1; i <= n; i ++)
    {   
        std::cout << S[i]  << endl;
        std::cout << "      " << to_int(S[i]) << endl;
        l = C[to_int(S[i])];
        // Aqui hay segmentation
        val_K[i] = to_char(l);
        //N[l] += l;
        //*L_class[N[l]] = int(C.rank(to_int(S[i]), l));
    }   
}

int Alphabet_Partitioning::to_int(char c){
    return c - ' ' + 1;
}

char Alphabet_Partitioning::to_char(int i){
    return alphabet[i - 1];
}

int Alphabet_Partitioning::floor_log2(int n)
{   
    int ans = -1;
    while(n != 0)
    {
        n = n >> 1;
        ans += 1;
    }
    return ans;
}

int main(){
    string exS = "to be or not to be, that is the question";
    Alphabet_Partitioning cosa(exS);
    
   return 0;
}