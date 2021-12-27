#include <sdsl/wavelet_trees.hpp>
#include <iostream>
#include <map>
#include <stdio.h>

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
    string tmp_text_path = "tmp/tmp_text_";
    
    uint64_t alphabet_size;
    uint64_t text_size;

    ifstream alphabet_access;
    char delimiter = '/';

    protected:
    // Con esta funcion transformaremos los caracteres a su valor int, minimo = 1
    uint64_t to_int(string word);
    // Con esta funcion transformaremos los int a su valor equivalente al alfabeto usado
    string to_string(uint64_t i);
    // Computamos el piso del logaritmo base 2 de un int
    // Nolotar que se computa en O(log(n))
    uint64_t floor_log2(uint64_t n);

    void text_to_int(string text_path, string alph_path, string id, bool keep_lines);
    void Identify_alphabet(string alphabet_path);

    public:
    // Constructor que recibe solo el path del texto
    Alphabet_Partitioning(string text_path, uint64_t size, string index);
    // Constructor que recibe el path del texto y el path del alfabeto a utilizar.
    Alphabet_Partitioning(string text_path, uint64_t size, string custom_alphabet_path, string index, bool convertion, bool keep_lines);
    // Retorna la palabra en la posición i-esima del texto
    uint64_t access(uint64_t i);
    // Retorna la cantidad de ocurrencias de la palabra (word) hasta la posición i-esima.
    uint64_t rank(uint64_t word, uint64_t i);
    // Retorna la posición de la i-esima ocurrencia de la palabra (word)
    uint64_t select(uint64_t word, uint64_t i);
    // Retorna la posición de la palabra (word) dentro del alfabeto
    uint64_t BS_over_alphabet(string word);
    // Pretty print de la estructura
    void show_structure();
    // Retorna un fragmento del texto entre las posiciones [start, end] 
    uint64_t get_snippet(uint64_t start, uint64_t end);
    // Retorna un vector de las posiciones donde ocurre la palabra (word)
    pair<vector<uint64_t>*, uint64_t> get_all_word_ocurrences(uint64_t word);
    // Retorna un vector de las posiciones donde ocurre la palabra (word)
    pair<vector<uint64_t>*, uint64_t> get_all_phrase_ocurrences(vector<uint64_t> phrase);
    // Cantidad de palabras del texto
    uint64_t get_text_size();
    // Byte size de la estructura
    uint64_t size();
};

Alphabet_Partitioning::Alphabet_Partitioning(string text_path, uint64_t size, string index)
{
    Alphabet_Partitioning(text_path, size, "alphabets/default.txt", index, false, false);
}
Alphabet_Partitioning::Alphabet_Partitioning(string text_path, uint64_t size, string custom_alphabet_path, string index, bool convertion, bool keep_lines)
{   
    // Analiza el alfabeto entregado, actualizando alphabet_size y alphabet_buffer_size
    alphabet_path = custom_alphabet_path;
    alphabet_access.open(alphabet_path, ios::in);
    tmp_text_path = "tmp/" + index + ".txt";
    text_size = size;

    cout << "Identificando alfabeto..." << endl;
    Identify_alphabet(custom_alphabet_path);
    cout << "   Alfabeto identificado!" << endl;

    if(convertion)
    {
        cout << "Traduciendo el texto..." << endl;
        text_to_int(text_path, custom_alphabet_path, index, keep_lines);
        cout << "   Texto parseado!" << endl;
        cout << "Proceso finalizado!" << endl;
        return;
    }

    tmp_text_path = text_path;

    //  Computamos el logaritmo base 2 del tamanio del alfabeto
    uint64_t log2_sigma = floor_log2(alphabet_size);

    cout << "(!) Alfabeto de tamano: " << alphabet_size << endl;
    cout << "(!) Alfabeto de tamano log: " << log2_sigma << endl;
    

    // Definicion de F
    //  Definimos  nuestro vector de pares [(int) frecuencia, (int) caracter]
    vector<pair<uint64_t, uint64_t>> *F = new vector<pair<uint64_t, uint64_t>>;

    // Inicialicion de F
    //  Incluimos los 95 caracteres con frecuencia 0
    //   (!) Notar que hora F indexa en 0 hasta 94.
    cout << "Identificando ocurrencia de palabras..." << endl;
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
    cout << "   (1/3) Palabras recuperadas y ordenadas" << endl;

    // Definicion de N
    //  Arreglo de enteros, del tamanio del logaritmo base 2 del tamanio del alfabeto
    size_L = new uint64_t[log2_sigma + 1];

    // Inicializacion de N
    //  Se setean todos sus valores a 0
    for(uint64_t l = 0; l <= log2_sigma; l++) size_L[l] = 0;

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
        size_L[l] += (*F)[j-1].first;
    }
    cout << "   (2/3) Conversionadas a sus clases" << endl;

    // for(int i = 0; i <= log2_sigma; i++)
    // {
    //     cout << "Clase " << i << " tiene tamaño " << size_L[i] << endl;
    // }

    // Eliminacion de F
    delete F;

    // Inicializacion de C
    // A partir de val_C, generamos C en formato de Wavelet Tree huffman shaped
    //      (!) Indexa en 1
    construct_im(C, val_C, 8);
    cout << "   (3/3) Codificador construido" << endl;

    // Definicion de val_L
    //  Arreglo de punteros, del tamanio del logaritmo base 2 del tamanio del alfabeto
    //  En cada elemento almacenamos un arreglo de int con la ocurrencia de caada caracter
    //  segun su clase
    vector<int_vector<64>> val_L;

    // Inicializacion de val_L
    //  Dentro del alfabeto [0, log2_sigma] (clases posibles)
    cout << "Caracterizando texto por clases..." << endl;
    for (uint64_t l = 0; l <= log2_sigma; l++)
    {
        // Asociamos el arreglo al arreglo de punteros val_L
        //      (!) Optimizacion levemente inutil, pero asi no guardamos referencia a clases vacias
        val_L.push_back(int_vector<64>(size_L[l], 1));
        // Definimos el tamanio de la clase l como 0 (limpiamos)
        size_L[l] = 0;
    }

    // Deficion val_K
    //  AVector de int, de tamanio equivalente al texto, almacena el equivalente de S en formato de clases
    int_vector<64> val_K(text_size, 0);

    // Inicializacion de val_K
    //  Por cada caracter en el texto
    int i = 0;
    cout << "   Reconstruyendo texto codificado..." << endl;
    while(getline(text, word, delimiter))
    {
        // Del texto, para la letra text[i] recuperamos su clase
        l = C[stoul(word)];
        // Reescribimos text[i] como la clase de i
        val_K[i] = l;
        // Avanzamos en el elemento de la clase l leido
        size_L[l] += 1;
        // Para el L de la clase l, accedemos al elemento de la clase en la que vamos
        // y renumeramos text[i] en el marco del alfabeto de la clase.
        val_L[l][size_L[l] - 1] = uint64_t(C.rank(stoul(word) + 1, l));

        i++;
    }
    text.clear();
    text.seekg(0);

    // Inicializacion de K
    // A partir de val_K, generamos K en formato de Wavelet Tree huffman shaped
    //      (!) Indexa en 1
    construct_im(K, val_K, 8);
    cout << "       Construido!" << endl;

    // Procesamos cada L de cada clase a una estructura basada en permutaciones
    // Generamos una lista auxiliar con cada L_l previamente computado
    wt_gmr<>* aux_list = new wt_gmr<>[log2_sigma + 1];
    // Por cada clase
    for(l = 0; l < val_L.size(); l++)
    {
        construct_im(aux_list[l], val_L[l], 8);
        L.push_back(aux_list[l]);
    }
    cout << "   Construido codificador por clases!" << endl;
    cout << "Proceso de compresión finalizado." << endl;
}

void Alphabet_Partitioning::Identify_alphabet(string alph_path){
    alphabet_size = 0;
    string line;
    uint64_t buffer = 0;

    if(!alphabet_access.is_open()){
        cout << "(!!!) [Identify_alphabet] Error: No se encuentra el alfabeto especificado" << endl;
        exit(1);
    }

    // TODO: manejo de error en casao de no abrirse
    while(getline(alphabet_access, line))
    {
        buffer += line.size() + 1;
        alphabet_size++;
    }
    alphabet_access.clear();
    alphabet_access.seekg(0);
}

uint64_t Alphabet_Partitioning::BS_over_alphabet(string word){
    // uint64_t first = 0;
    // uint64_t last = alphabet_size - 1;
    // uint64_t middle = floor((first + last)/2);
    // string line;
    // int status;

    // if(!alphabet_access.is_open()){
    //     cout << "(!!!) [BS_over_alphabet] Error: No se encuentra el alfabeto especificado" << endl;
    //     exit(1);
    // }
    // //cout << "Busco la palabra: " << word << endl;

    // while(first <= last){
    //     alphabet_access.seekg(alphabet_word_reference[middle]);
    //     getline(alphabet_access, line);
    //     //cout << "   Comparo contra la palabra: " << line << endl;
    //     status = line.compare(word); 
    //     if(status < 0){
    //         //cout << "       Me voy a la z" << endl;
    //         first = middle + 1;
    //     } else if (status == 0){
    //         //cout << "       (!) Retorno " << (middle + 1) <<  endl;
    //         return middle + 1;
    //     } else {
    //         //cout << "       Me voy a la a" << endl;
    //         last = middle - 1;
    //     }

    //     middle = floor((first + last)/2);
    // }

    // if(first > last) {
    //     cout << "(!) [BS_over_alphabet] Error: Palabra no encontrada. Por favor verifique que el alfabeto contenga todas las palabras posibles" << endl;
    //     cout << "       (?) Palabra desconocida:" << word << endl;
    //     exit(1);
    // }
    // alphabet_access.clear();
    // alphabet_access.seekg(0);
    return 0;
}

// Funcion descartada
uint64_t Alphabet_Partitioning::to_int(string word){
    if(!alphabet_access.is_open()){
        cout << "(!!!) [to_int] Error: No se encuentra el alfabeto especificado" << endl;
        exit(1);
    }
    uint64_t code = BS_over_alphabet(word);
    return code;
}
// Funcion descartada
string Alphabet_Partitioning::to_string(uint64_t i){

    string word;
    // if(!alphabet_access.is_open()){
    //     cout << "(!!!) [to_string] Error: No se encuentra el alfabeto especificado" << endl;
    //     exit(1);
    // }
    // alphabet_access.seekg(alphabet_word_reference[i - 1]);
    // getline(alphabet_access, word);
    // alphabet_access.clear();
    // alphabet_access.seekg(0);
    return word;
}

void Alphabet_Partitioning::text_to_int(string text_path, string alph_path, string id, bool keep_lines){
    // Abrimos el texto
    ifstream raw_text;
    raw_text.open(text_path, ifstream::in);

    if(!raw_text.is_open()){
        cout << "(!!!) [text_to_int] Error: No se encuentra el texto especificado" << endl;
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
        if(keep_lines){text << "\n";}
    }
    cout << "El tamano del texto es: " << text_size << endl;
    raw_text.close();
    text.close();
    rename(tmp_text_path, "tmp/" + id + "_size_" + std::to_string(text_size) + ".txt");
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
uint64_t Alphabet_Partitioning::access(uint64_t i)
{   
    uint64_t l = K[i];
    int_vector_size_type k = K.rank(i+1, l);
    int_vector_size_type m = L[l][k];
    // return to_string(C.select(m, l));
    return C.select(m, l);
}
uint64_t Alphabet_Partitioning::rank(uint64_t word, uint64_t i)
{
    //uint64_t w_int = to_int(word);
    uint64_t l = C[word];
    // cout << " (RANK) Clase: " << l << endl;
    uint64_t m = C.rank(word + 1, l);
    // cout << " (RANK) Occ de la clase nro en C: " << m << endl;
    uint64_t k = K.rank(i + 1, l);
    // cout << " (RANK) Occ de la clase nro en K: " << k << endl;
    // cout << " (RANK) Dentro de l soy el nro: " << L[l].rank(k + 1, m) << endl;
    // cout << " (RANK) L de tamano: " << size_L[l] << endl;
    return L[l].rank(k + 1, m);
}
uint64_t Alphabet_Partitioning::select(uint64_t word, uint64_t i)
{
    // uint64_t w_int = to_int(word);
    uint64_t l = C[word];
    uint64_t m = C.rank(word + 1, l);
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

uint64_t Alphabet_Partitioning::get_snippet(uint64_t start, uint64_t end)
{
    if (start < 0 | start > text_size | end < 0 | end > text_size){
        cout << "(!!!) [get_snippet] Error: Los indices entregados están fuera de los límites permitidos." << endl;
        cout << "   start = " << start << endl;
        cout << "   end = " << end << endl;
        return 0;
    }
    if (start > end){
        cout << "(!!!) [get_snippet] Error: El limite superior es menor que el limite inferior entregado." << endl;
        cout << "   start = " << start << endl;
        cout << "   end = " << end << endl;
        return 0;
    }
    
    uint64_t snippet = access(start);
    
    for(uint64_t i = start + 1; i <= end; i++){
        //snippet += " ";
        snippet += access(i);
    }

    return snippet;
}
pair<vector<uint64_t>*, uint64_t> Alphabet_Partitioning::get_all_word_ocurrences(uint64_t word)
{
    vector<uint64_t> *positions = new vector<uint64_t>;
    uint64_t size = rank(word, text_size);
    for(uint64_t i = 1; i <= size; i++)
    {
        (*positions).push_back(select(word, i));
    }
    return make_pair(positions, size);
}

pair<vector<uint64_t>*, uint64_t> Alphabet_Partitioning::get_all_phrase_ocurrences(vector<uint64_t> phrase)
{
    uint64_t min_occ = text_size, r = 0, offset = 1, offset_min = 1, l = 0, n_occ = 0, word_min;
    vector<uint64_t> *positions = new vector<uint64_t>;
    // Identificamos el de menor ocurrencia
    for(uint64_t word:phrase)
    {
        r = rank(word,text_size);
        if(r < min_occ)
        {
            min_occ = r;
            offset_min = offset; 
            word_min = word;
        }
        if(r == 0)
        {
            cout << "[GAPO] (!!) WARNING: Palabra " << word << " sin ocurrencias." << endl; 
            return make_pair(positions, 0);
        }
        offset++;
        l++;
    }
    // cout << "  Palabra min: " << word_min << endl;
    // cout << "  offset min: " << offset_min << endl;
    // cout << "  occ min: " << offset_min << endl;

    uint64_t pos_end_phrase = 0, i_last_occ = 0, pos_word_i = 0, pos_word_min = 0, pos_begin_phrase = 0;
    for(uint64_t i_word_min = 1; i_word_min <= min_occ; i_word_min++)
    {
        pos_word_min = select(word_min, i_word_min);
        pos_end_phrase = pos_word_min + l - offset_min;
        pos_begin_phrase = pos_word_min - offset_min + 1;
        offset = 0;

        if(pos_end_phrase > text_size) break;
        // cout << "   Posicion inicial de frase: " << pos_begin_phrase << endl;
        // cout << "   Posicion final de frase: " << pos_end_phrase << endl;
        // cout << "   Posicion palabra min: " << pos_word_min << endl;

        for(uint64_t word:phrase)
        {
            // Si en la posicion deseada no esta nuestra palabra
            if(word != access(pos_begin_phrase + offset))
            {   
                break;
            } else {
                if(offset == l-1)
                {
                    (*positions).push_back(pos_begin_phrase);
                    n_occ++;
                }
            }
            offset++;
        }
        // cout << "OFFSET: " << offset << " L: " << l << endl;
    }

    return make_pair(positions, n_occ);
}

uint64_t Alphabet_Partitioning::get_text_size()
{
    return text_size;
}

uint64_t Alphabet_Partitioning::size()
{
    uint64_t s = 0;
    uint64_t aux = size_in_bytes(K);
    cout << "K gasta " << aux << " bytes" << endl;
    s+= aux;
    aux = size_in_bytes(C);
    cout << "C gasta " << aux << " bytes" << endl;
    s+= aux;
    for(wt_gmr<> l:L)
    {
        aux = size_in_bytes(l);
        cout << "l gasta " << aux << " bytes" << endl;
        s+= aux;
    }
    // size_l
    s+= (4*floor_log2(alphabet_size));
    return s;
}