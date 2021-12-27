#include <sdsl/wavelet_trees.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
using namespace sdsl;

int main()
{
    int text_size = 2306280;
    string text_path = "tmp/eng_4_size_2306280.txt";
    int b_size = 15619000;
    int_vector<64> token(text_size, 0);

    string word;
    int i = 0;
    ifstream text;
    text.open(text_path);
    // Por cada linea, recuperamos palabras separadas por espacio
    while(getline(text, word, '/')){
        // Guardamos la palabra en memoria
        token[i] = stoul(word);
        i++;
    }
    text.close();
    wt_gmr<> test;
    construct_im(test, token, 8);

    int gmr_size = size_in_bytes(test);

    cout << "[GWR] comprime en " << ((gmr_size/b_size)*100) << "%, estructura pesa " << gmr_size << " bytes." << endl;
    write_structure<JSON_FORMAT>(test,"wea.json");
    return 1;

}