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
    // Cantidad de palabras en el texto
    uint64_t get_text_size();
    // Byte size de la estructura
    uint64_t size();
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
    vector<uint64_t> *words_pos;
    uint64_t size = 0;

    uint64_t min_occ = text_size, min_aux = 0, offset_min = 0;
    pair<vector<uint64_t>*, uint64_t> pair_found;
    for(uint64_t word:phrase)
    {
        pair_found = get_all_word_ocurrences(word);
        
        if (pair_found.second < min_occ)
        {
            min_occ = pair_found.second;
            offset_min = n;
            words_pos = pair_found.first;
        }
        n++;
    }

    // cout << "  offset min: " << offset_min << endl;
    // cout << "  occ min: " << min_occ << endl;

    uint64_t pos_f_i = 0;

    for(uint64_t i = 0; i < min_occ ; i++)
    {   
        pos_f_i = (*words_pos)[i] - offset_min;
        // cout << "   Posicion inicial de frase: " << pos_f_i << endl;
        // cout << "   Posicion final de frase: " << pos_f_e << endl;
        // cout << "   Posicion palabra min: " << (*words_pos)[offset_min][i] << endl;
        // Por cada palabra en la frase
        for(uint64_t w_i = 0; w_i < n ; w_i++)
        {   
            if(phrase[w_i] == tokenized_text[pos_f_i + w_i])
            {
                if(w_i == n - 1)
                {
                    positions->push_back(pos_f_i);
                    size++;
                }
            } else {
                break;
            }
        }
    }

    delete words_pos;

    return make_pair(positions, size);
}

uint64_t Simple_II::get_text_size()
{
    return text_size;
}

uint64_t Simple_II::size()
{
    return size_in_bytes(tokenized_text);
}