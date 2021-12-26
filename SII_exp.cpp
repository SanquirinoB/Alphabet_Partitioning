#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Simple_II {

private:
    string text_path;
    ifstream text;
    vector<uint64_t> tokenized_text;
    uint64_t text_size = 0;

public:
    Simple_II(string txt_path);   
    // Tokenizamos el texto completo y guardamos en memoria
    void process_text();
    // Retorna un fragmento del texto entre las posiciones [start, end] 
    uint64_t get_snippet(uint64_t start, uint64_t end);
    // Retorna un vector de las posiciones donde ocurre la palabra (word) y la cantidad de ocurrencias
    pair<vector<uint64_t>*, uint64_t> get_all_word_ocurrences(uint64_t word);
    // Retorna un vector de las posiciones donde ocurre la palabra (word)
    pair<vector<uint64_t>*, uint64_t> get_all_phrase_ocurrences(vector<uint64_t> phrase);

    uint64_t get_text_size();
};

Simple_II::Simple_II(string txt_path){
    text_path = txt_path;
    cout << "Abrimos el texto" << endl;
    text.open(txt_path, ios::in);

    cout << "Procesando el texto..." << endl;
    process_text();
    cout << "   Texto procesado!" << endl;
    text.close();
}

void Simple_II::process_text(){
    string word;
    // Por cada linea, recuperamos palabras separadas por espacio
    while(getline(text, word, '/')){
        // Guardamos la palabra en memoria
        tokenized_text.push_back(stoul(word));
        text_size++;
    }
}

uint64_t Simple_II::get_snippet(uint64_t start, uint64_t end){
    uint64_t snippet = tokenized_text[start];
    for(uint64_t i = start + 1; i <= end; i++){
        snippet += tokenized_text[i];
    }

    return snippet;
}

pair<vector<uint64_t>*, uint64_t> Simple_II::get_all_word_ocurrences(uint64_t word){
    vector<uint64_t> *positions = new vector<uint64_t>;
    uint64_t size = 0;
    for (uint64_t i = 0; i < text_size; i++){
        if (tokenized_text[i] == word){
            (*positions).push_back(i);
            size ++;
        }
    }
    return make_pair(positions, size);
}

pair<vector<uint64_t>*, uint64_t> Simple_II::get_all_phrase_ocurrences(vector<uint64_t> phrase){
    uint64_t n = 0, word_min;
    vector<uint64_t> *positions = new vector<uint64_t>;
    vector<vector<uint64_t>> *words_pos = new vector<vector<uint64_t>>;
    vector<uint64_t> *word_pos_size = new vector<uint64_t>;
    uint64_t size = 0;

    uint64_t min_occ = text_size, min_aux = 0, offset_min = 0;
    pair<vector<uint64_t>*, uint64_t> pair_found;
    for(uint64_t word:phrase)
    {
        pair_found = get_all_word_ocurrences(word);
        words_pos->push_back(*pair_found.first);
        word_pos_size->push_back(pair_found.second);
        if (pair_found.second < min_occ)
        {
            min_occ = pair_found.second;
            offset_min = n;
            word_min = word;
        }
        n++;
    }

    // cout << "  Palabra min: " << word_min << endl;
    // cout << "  offset min: " << offset_min << endl;
    // cout << "  occ min: " << min_occ << endl;

    uint64_t pos_f_i = 0, pos_f_e = 0;
    bool has_failed = false;
    // Por cada posicion de la primera palabra
    for(uint64_t i = 0; i < min_occ ; i++)
    {   
        pos_f_i = (*words_pos)[offset_min][i] - offset_min;
        pos_f_e = (*words_pos)[offset_min][i] + n - offset_min - 1;
        // cout << "   Posicion inicial de frase: " << pos_f_i << endl;
        // cout << "   Posicion final de frase: " << pos_f_e << endl;
        // cout << "   Posicion palabra min: " << (*words_pos)[offset_min][i] << endl;
        // Por cada palabra en la frase
        for(uint64_t w_i = 0; w_i < n ; w_i++)
        {   
            // Por cada posicion de esa palabra
            for(uint64_t j = 0; j < (*word_pos_size)[w_i] ; j++)
            {
                // Posicion ocurre antes del inicio de la frase, saltamos
                if((*words_pos)[w_i][j] < pos_f_i)
                {
                    continue;
                 // Si la posicion esta despues del final, se acaba
                }
                // cout << "       Posicion de palabra: " << (*words_pos)[w_i][j] << endl;
                // cout << "       Offset de palabra: " << w_i << endl;
                // cout << "       Dd espero: " << (pos_f_i + w_i) << endl;
                // sleep(1);
                
                if((*words_pos)[w_i][j] == pos_f_i + w_i) {
                    if(w_i == n - 1){
                        positions->push_back(pos_f_i);
                        size++;
                    }
                    break;
                }
                if(pos_f_e < (*words_pos)[w_i][j])
                {
                    // cout << "   (!) La palabra fallo" << endl;
                    has_failed = true;
                    break;
                // Pero si esta donde queremos, sgte palabra
                } 
            }
            // Si falla una sola palabra para la posicion, pasamos al sgte i
            if(has_failed) break;            
        }
        has_failed = false;
    }
    return make_pair(positions, size);
}

uint64_t Simple_II::get_text_size()
{
    return text_size;
}