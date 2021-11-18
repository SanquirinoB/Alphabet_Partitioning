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
    uint64_t* size_L;

    // Alfabeto a utilizar
    // Por ahora toleraremos solo alfabeto definido por ASCII imprimibles 
    // [32, 126] -> [1, 95]
    string alphabet_path = "alphabets/default.txt";
    string tmp_text_path = "tmp/tmp_text.txt";
    vector<uint64_t> alphabet_word_reference;
    ifstream alphabet_access;
    uint64_t alphabet_size;
    uint64_t text_size;
    char delimiter = '/';

    protected:
    // Con esta funcion transformaremos los caracteres a su valor int, minimo = 1
    uint64_t to_int(string c);
    // Con esta funcion transformaremos los int a su valor equivalente al alfabeto usado
    string to_string(uint64_t i);
    // Computamos el piso del logaritmo base 2 de un int
    // Nolotar que se computa en O(log(n))
    uint64_t floor_log2(uint64_t n);

    void text_to_int(string text_path, string alph_path);
    void Identify_alphabet(string alphabet_path);

    public:
    // Constructor
    Alphabet_Partitioning(string text_path);
    Alphabet_Partitioning(string text_path, string custom_alphabet_path);

    string access(uint64_t i);
    uint64_t rank(string c, uint64_t i);
    uint64_t select(string c, uint64_t i);

    uint64_t BS_over_alphabet(string word);
    void show_structure();

    string get_snippet(uint64_t i, uint64_t j);
};

Alphabet_Partitioning::Alphabet_Partitioning(string text_path)
{
    Alphabet_Partitioning(text_path, "alphabets/default.txt");
}
Alphabet_Partitioning::Alphabet_Partitioning(string text_path, string custom_alphabet_path)
{   
    // Analiza el alfabeto entregado, actualizando alphabet_size y alphabet_buffer_size
    alphabet_path = custom_alphabet_path;
    alphabet_access.open(alphabet_path, ios::in);
    
    Identify_alphabet(custom_alphabet_path);
    text_to_int(text_path, custom_alphabet_path);

    //  Computamos el logaritmo base 2 del tamanio del alfabeto
    uint64_t log2_sigma = floor_log2(alphabet_size);

    // Definicion de F
    //  Definimos  nuestro vector de pares [(int) frecuencia, (int) caracter]
    vector<pair<uint64_t, uint64_t>> *F = new vector<pair<uint64_t, uint64_t>>;

    // Inicialicion de F
    //  Incluimos los 95 caracteres con frecuencia 0
    //   (!) Notar que hora F indexa en 0 hasta 94.
    for(uint64_t i = 1; i <= alphabet_size; i++) (*F).push_back(make_pair(0, i));
    // Accedemos a la version temporal del texto
    ifstream text;
    string word;
    text.open(tmp_text_path, ios::in);
    //  Para cada caracter en el texto, sumamos 1 a su frecuencia respectiva
    while(getline(text, word, delimiter)) (*F)[stoul(word) - 1].first += 1;
    text.clear();
    text.seekg(0);
    //  Ordenamos el alfabeto por frecuencia en orden descendente
    sort((*F).rbegin(), (*F).rend()); 
    
    // Definicion de N
    //  Arreglo de enteros, del tamanio del logaritmo base 2 del tamanio del alfabeto
    uint64_t N[log2_sigma + 1];

    // Inicializacion de N
    //  Se setean todos sus valores a 0
    for(uint64_t l = 0; l <= log2_sigma; l++) N[l] = 0;

    // Definicion de val_C
    //  Vector de enteros, del tamanio del alfabeto.
    int_vector<64> val_C(alphabet_size, 0);
    //  Generamos una variable auxiliar para almacenar la clase de un caracter
    uint64_t l;
    
    // Inicializacion de val_C
    //  Por cada elemento del alfabeto de C [1, log(sigma)]
    for(uint64_t j = 1; j <= alphabet_size; j++)
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
    size_L = N;

    // Inicializacion de val_L
    //  Dentro del alfabeto [0, log2_sigma] (clases posibles)
    for (uint64_t l = 0; l <= log2_sigma; l++)
    {   
        // Asociamos el arreglo al arreglo de punteros val_L
        //      (!) Optimizacion levemente inutil, pero asi no guardamos referencia a clases vacias
        val_L.push_back(int_vector<64>(N[l], 1));
        // Definimos el tamanio de la clase l como 0 (limpiamos)
        N[l] = 0;
    }
    
    // Deficion val_K
    //  AVector de int, de tamanio equivalente al texto, almacena el equivalente de S en formato de clases
    int_vector<64> val_K(text_size, 0);

    // Inicializacion de val_K
    //  Por cada caracter en el texto
    int i = 0;
    while(getline(text, word, delimiter))
    {
        // Del texto, para la letra text[i] recuperamos su clase
        l = C[stoul(word)];
        // Reescribimos text[i] como la clase de i
        val_K[i] = l;
        // Avanzamos en el elemento de la clase l leido
        N[l] += 1;
        // Para el L de la clase l, accedemos al elemento de la clase en la que vamos
        // y renumeramos text[i] en el marco del alfabeto de la clase.
        val_L[l][N[l] - 1] = uint64_t(C.rank(stoul(word) + 1, l));
    
        i++;
    }  
    text.clear();
    text.seekg(0);

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

void Alphabet_Partitioning::Identify_alphabet(string alph_path){
    alphabet_size = 0;
    string line;
    uint64_t buffer = 0;

    if(!alphabet_access.is_open()){
        cout << "(!!!) Error: No se encuentra el alfabeto especificado" << endl;
        exit(1);
    }

    // TODO: manejo de error en casao de no abrirse
    while(getline(alphabet_access, line))
    {
        alphabet_word_reference.push_back(buffer);
        buffer += line.size() + 1;
        alphabet_size++;
    }
    alphabet_word_reference.push_back(buffer);
    alphabet_access.clear();
    alphabet_access.seekg(0);
}

uint64_t Alphabet_Partitioning::BS_over_alphabet(string word){
    uint64_t first = 0;
    uint64_t last = alphabet_size - 1;
    uint64_t middle = floor((first + last)/2);
    string line;

    if(!alphabet_access.is_open()){
        cout << "(!!!) Error: No se encuentra el alfabeto especificado" << endl;
        exit(1);
    }

    while(first <= last){
        alphabet_access.seekg(alphabet_word_reference[middle]);
        getline(alphabet_access, line);

        if(line < word){
            first = middle + 1;
        } else if (line == word){
            return middle + 1;
        } else {
            last = middle - 1;
        }

        middle = floor((first + last)/2);
    }

    if(first > last) {
        cout << "(!) Error: Palabra no encontrada. Por favor verifique que el alfabeto contenga todas las palabras posibles" << endl;
        cout << "       (?) Palabra desconocida: " << word << endl;
    }
    alphabet_access.clear();
    alphabet_access.seekg(0);

    return 0;
}

uint64_t Alphabet_Partitioning::to_int(string c){
    if(!alphabet_access.is_open()){
        cout << "(!!!) Error: No se encuentra el alfabeto especificado" << endl;
        exit(1);
    }
    uint64_t code = BS_over_alphabet(c);
    return code;
}
string Alphabet_Partitioning::to_string(uint64_t i){

    string word;
    if(!alphabet_access.is_open()){
        cout << "(!!!) Error: No se encuentra el alfabeto especificado" << endl;
        exit(1);
    }
    alphabet_access.seekg(alphabet_word_reference[i - 1]);
    getline(alphabet_access, word);
    alphabet_access.clear();
    alphabet_access.seekg(0);
    return word;
}

void Alphabet_Partitioning::text_to_int(string text_path, string alph_path){
    // Abrimos el texto
    ifstream raw_text;
    raw_text.open(text_path, ifstream::in);

    if(!raw_text.is_open()){
        cout << "(!!!) Error: No se encuentra el texto especificado" << endl;
        exit(1);
    }

    // Generamos un archivo temporal con el documento a generar
    ofstream{tmp_text_path};
    ofstream text;
    text.open(tmp_text_path);

    string line, word;
    text_size = 0;
    // Leo por linea
    while(getline(raw_text, line)){
        // Leo por cada palabra
        stringstream s_line(line);
        while(getline(s_line, word, ' ')){
            text << std::to_string(BS_over_alphabet(word)) << delimiter;
            text_size++;
        }
    }
    raw_text.close();
    text.close();
}

uint64_t Alphabet_Partitioning::floor_log2(uint64_t n)
{   
    uint64_t ans = -1;
    while(n != 0)
    {
        n = n >> 1;
        ans += 1;
    }
    return ans;
}
string Alphabet_Partitioning::access(uint64_t i)
{
    uint64_t l = K[i];
    int_vector_size_type k = K.rank(i+1, l);
    int_vector_size_type m = L[l][k];
    return to_string(C.select(m, l));
}
uint64_t Alphabet_Partitioning::rank(string c, uint64_t i)
{
    uint64_t l = C[to_int(c)];
    uint64_t m = C.rank(to_int(c) + 1, l);
    uint64_t k = K.rank(i + 1, l);
    return L[l].rank(k + 1, m);
}
uint64_t Alphabet_Partitioning::select(string c, uint64_t i)
{
    uint64_t l = C[to_int(c)];
    uint64_t m = C.rank(to_int(c) + 1, l);
    uint64_t k = L[l].select(i, m);
    return K.select(k, l);
}
void Alphabet_Partitioning::show_structure()
{
    std::cout << "El contenido de K es:" << endl;
    for(int i = 1; i <= text_size; i++){
        std::cout << K[i] << "|";
    }
    std::cout << endl;

    std::cout << "El tamano de C es: " << alphabet_size << endl;
    std::cout << "El contenido de C es:" << endl;
    int i = 1;
    string word;
    alphabet_access.clear();
    alphabet_access.seekg(0);
    while(getline(alphabet_access, word)){
        cout << word << '|';
        cout << C[i] << endl;
        i++;
    }
    alphabet_access.clear();
    alphabet_access.seekg(0);

    std::cout << "El cantidad de clases es: " << floor_log2(alphabet_size) << endl;
    std::cout << "El contenido de cada L es:" << endl;
    for(int i = 0; i <= floor_log2(alphabet_size); i++){
        std::cout << "  La clase: " << i << endl;
        std::cout << "      De tamanio: " << size_L[i] << endl;

        for(int j = 1; j <= size_L[i]; j++){
            std::cout << "  " << " ";
            std::cout << L[i][j] << "|";
        }
        std::cout << endl;
    }
    std::cout << endl;
}

string Alphabet_Partitioning::get_snippet(uint64_t start, uint64_t end)
{
    if (start < 0 | start > text_size | end < 0 | end > text_size){
        cout << "(!!!) [get_snippet] Error: Los indices entregados están fuera de los límites permitidos." << endl;
        cout << "   start = " << start << endl;
        cout << "   end = " << end << endl;
        return "";
    }
    if (start > end){
        cout << "(!!!) [get_snippet] Error: El limite superior es menor que el limite inferior entregado." << endl;
        cout << "   start = " << start << endl;
        cout << "   end = " << end << endl;
        return "";
    }

    string word = access(start);

    for(uint64_t i = start + 1; i <= end; i++){
        word += " ";
        word += access(i);
    }

    return word;
}

int main(){
    string alphabet_path_ex = "alphabets/word_test.txt";
    string text_path_ex = "text/example_text.txt";
    Alphabet_Partitioning cosa(text_path_ex, alphabet_path_ex);
    // cout << cosa.BS_over_alphabet("to") << endl;
    // cout << cosa.BS_over_alphabet("not") << endl;
    // cout << cosa.BS_over_alphabet("be") << endl;
    // cout << cosa.BS_over_alphabet("or") << endl;
    // cout << cosa.BS_over_alphabet("wea") << endl;
    // cosa.End_BS();
    cout << cosa.get_snippet(7, 10) << endl;
}

