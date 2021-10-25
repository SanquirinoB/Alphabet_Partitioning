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
    // Conjunto de L_l
    vector<wt_gmr<>> L ;

    // Alfabeto a utilizar
    // Por ahora toleraremos solo alfabeto definido por ASCII imprimibles 
    // [32, 126] -> [1, 95]
    const string alphabet = "# !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    const int alphabet_size = alphabet.length() - 1;

    int n;

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
    char access(int i);
    int rank(char c, int i);
    int select(char c, int i);
    wt_huff_int<rrr_vector<63>> get_C();

    void show_structure();
};

Alphabet_Partitioning::Alphabet_Partitioning(string S)
{   
    // Generamos variables de uso general
    //  Capturamos el tamanio del texto
    n = S.length();
    //  Computamos el logaritmo base 2 del tamanio del alfabeto
    int log2_sigma = floor_log2(alphabet_size);

    // Definicion de F
    //  Definimos  nuestro vector de pares [(int) frecuencia, (int) caracter]
    vector<pair<int, int>> *F = new vector<pair<int, int>>;

    // Inicialicion de F
    //  Incluimos los 95 caracteres con frecuencia 0
    //   (!) Notar que hora F indexa en 0 hasta 94.
    for(int i = 1; i <= alphabet_size; i++) (*F).push_back(make_pair(0, i));
    //  Para cada caracter en el texto, sumamos 1 a su frecuencia respectiva
    for(int i = 0; i < n ; i++) (*F)[to_int(S[i]) - 1].first += 1;
    //  Ordenamos el alfabeto por frecuencia en orden descendente
    sort((*F).rbegin(), (*F).rend()); 
    
    // Definicion de N
    //  Arreglo de enteros, del tamanio del logaritmo base 2 del tamanio del alfabeto
    int N[log2_sigma + 1];

    // Inicializacion de N
    //  Se setean todos sus valores a 0
    for(int l = 0; l <= log2_sigma; l++) N[l] = 0;

    // Definicion de val_C
    //  Vector de enteros, del tamanio del alfabeto.
    int_vector<64> val_C(alphabet_size, 0);
    //  Generamos una variable auxiliar para almacenar la clase de un caracter
    int l;
    
    // Inicializacion de val_C
    //  Por cada elemento del alfabeto de C [1, log(sigma)]
    for(int j = 1; j <= alphabet_size; j++)
    {   
        // Computamos su clase (l)
        l = floor_log2(j);
        // Para su posicion en val_C, le asignamos su clase (puede haber error aqui)
        val_C[(*F)[j-1].second - 1] = l;
        // Incrementamos el tamanio de la clase l en la frecuencia del caracter asociado
        N[l] += (*F)[j-1].first;
    }
    
    // Eliminacion de F
    delete F;

    // Inicializacion de C
    // A partir de val_C, generamos C en formato de Wavelet Tree huffman shaped
    //      (!) Indexa en 1
    construct_im(C, val_C, 8);
    
    // Definicion de val_L
    //  Arreglo de punteros, del tamanio del logaritmo base 2 del tamanio del alfabeto
    //  En cada elemento almacenamos un arreglo de int con la ocurrencia de caada caracter
    //  segun su clase
    vector<int_vector<64>> val_L;

    // Inicializacion de val_L
    //  Dentro del alfabeto [0, log2_sigma] (clases posibles)
    for (int l = 0; l <= log2_sigma; l++)
    {   
        // Asociamos el arreglo al arreglo de punteros val_L
        //      (!) Optimizacion levemente inutil, pero asi no guardamos referencia a clases vacias
        val_L.push_back(int_vector<64>(N[l], 0));
        // Definimos el tamanio de la clase l como 0 (limpiamos)
        N[l] = 0;
    }
    
    // Deficion val_K
    //  AVector de int, de tamanio equivalente al texto, almacena el equivalente de S en formato de clases
    int_vector<64> val_K(n, 0);

    // Inicializacion de val_K
    //  Por cada caracter en el texto
    for (int i = 0; i < n; i ++)
    {   
        // Del texto, para la letra S[i] recuperamos su clase
        l = C[to_int(S[i])];
        // Reescribimos S[i] como la clase de i
        val_K[i] = l;
        // Avanzamos en el elemento de la clase l leido
        N[l] += 1;
        // Para el L de la clase l, accedemos al elemento de la clase en la que vamos
        // y renumeramos S[i] en el marco del alfabeto de la clase.
        val_L[l][N[l]] = int(C.rank(to_int(S[i]), l));
    }   

    // Inicializacion de K
    // A partir de val_K, generamos K en formato de Wavelet Tree huffman shaped
    //      (!) Indexa en 1
    construct_im(K, val_K, 8);

    // Procesamos cada L de cada clase a una estructura basada en permutaciones
    // Generamos una lista auxiliar con cada L_l previamente computado
    wt_gmr<>* aux_list = new wt_gmr<>[log2_sigma + 1];
    // Por cada clase       
    for(l = 0; l < val_L.size(); l++)
    {    
        construct_im(aux_list[l], val_L[l], 8);
        L.push_back(aux_list[l]);
    }
}

int Alphabet_Partitioning::to_int(char c){
    return c - ' ' + 1;
}

char Alphabet_Partitioning::to_char(int i){
    return alphabet[i];
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

wt_huff_int<rrr_vector<63>> Alphabet_Partitioning::get_C()
{
    return C;
}

char Alphabet_Partitioning::access(int i)
{
    uint64_t l = K[i];
    int_vector_size_type k = K.rank(i, l);
    int_vector_size_type m = L[l][k];
    return to_char(C.select(m + 1, l));
}
int Alphabet_Partitioning::rank(char c, int i)
{
    return 1;
}
int Alphabet_Partitioning::select(char c, int i)
{
    return 1;
}

void Alphabet_Partitioning::show_structure()
{
    std::cout << "El contenido de K es:" << endl;
    for(int i = 1; i <= n; i++){
        std::cout << K[i] << "|";
    }
    std::cout << endl;

    std::cout << "El contenido de C es:" << endl;
    for(int i = 1; i <= alphabet_size; i++){
        std::cout << alphabet[i] << "|";
    }
    std::cout << endl;
    for(int i = 1; i <= alphabet_size; i++){
        std::cout << C[i] << "|";
    }
    std::cout << endl;
    std::cout << "El contenido de cada L es:" << endl;
    for(int i = 0; i <= L.size(); i++){
        std::cout << "  La clase: " << i << endl;

        for(int j = 1; j <= 5; j++){
            std::cout << "  " << " ";
            std::cout << L[i][j] << "|";
        }
    }
    std::cout << endl;
}

int main(){
    string exS = "tobeornottobethatisthequestion";
    Alphabet_Partitioning cosa(exS);
    //cosa.show_structure();
    for(int i = 1; i <= exS.length(); i++){
        std::cout << cosa.access(i);
    }
    // // write only one object to std::cout
    // write_structure<HTML_FORMAT>(cosa.get_C(), cout);
    // // write one object into a file
    // write_structure<HTML_FORMAT>(cosa.get_C(), "csa_structure.html");
    
   return 0;
}