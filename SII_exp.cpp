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
    uint64_t n = 0;
    uint64_t offset = 1;
    vector<uint64_t> *positions = new vector<uint64_t>;
    vector<vector<uint64_t>> words_pos;
    vector<uint64_t> word_pos_size;
    uint64_t last_found = 0;
    uint64_t size = 0;
    
    for(uint64_t word:phrase)
    {
        pair<vector<uint64_t>*, uint64_t> pair_found = get_all_word_ocurrences(word);
        words_pos.push_back((*pair_found.first));
        word_pos_size.push_back(pair_found.second);
        n++;
    }
    // Por cada posicion de la primera palabra
    for(uint64_t i = 0; i < word_pos_size[0] ; i++)
    {
        // Por cada palabra en la frase
        for(uint64_t w_i = 1; w_i < n ; w_i++)
        {
            // Por cada posicion de esa palabra
            for(uint64_t j = 0; j < word_pos_size[w_i] ; j++)
            {
                if(words_pos[w_i][j] <= last_found + 1) continue;
                // Si la palabra ocurre 
                if(words_pos[w_i][j] == words_pos[0][i] + offset)
                {
                    // Si termine de leer la frase
                    if(offset == n - 1){
                        // Guardo la posicion
                        (*positions).push_back(words_pos[0][i]);
                        size ++;
                        last_found = words_pos[0][i];
                        offset = n + 1;
                    } else {
                        // Si no termine la frase, sigo con la siguiente palabra
                        offset++;
                    }
                    break;
                // Si las ocurrencias ya exceden la posicion buscada, dejamos de buscar la frase para i
                } else if (words_pos[w_i][j] > words_pos[0][i] + offset){
                    // Con esto simbolizamos que la frase ya no funciono
                    offset = n + 1;
                    last_found = words_pos[0][i];
                    break;
                }
            }
            // Si ya encontre la frase o la frase ya no funciono para i, seguimos
            if(offset == n + 1) break;
        }
        offset = 1;
    }
    return make_pair(positions, size);
}

uint64_t Simple_II::get_text_size()
{
    return text_size;
}