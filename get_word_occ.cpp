#include "AP_exp.cpp"
#include "SII_exp.cpp"
#include <time.h>

int main(){
    
    string text_path, alph_path, id, size;
	cout << "Ingrese el path del archivo a comprimir: ";
    cin >> text_path;
    cout << "Ingrese la cantidad de palabras: ";
    cin >> size;
    cout << "Ingrese el path del alfabeto a usar: ";
    cin >> alph_path;
    cout << "Ingrese el id para resultados: ";
    cin >> id;

    cout << "[Alphabet_Partitioning]" << endl;
    Alphabet_Partitioning ap(text_path, stoul(size), alph_path, id, false, false);
    cout << "[Simple_II]" << endl;
    Simple_II sii(text_path);

    ofstream results;
    results.open("results_new/get_all_word_occurrences/ap_gawo_" + id + ".csv");

    string word_str;
    time_t begin;
    time_t end;
    int count = 0;
    pair<vector<uint64_t>*, uint64_t> ans;
    results << "occurences\ttime\n";

    cout << "Inicia el proceso de Alphabet Partitioning..." << endl;

    for(uint64_t word = 1; word <= 418038; word++)
    {
        begin = clock();
        ans = ap.get_all_word_ocurrences(word);
        end = clock();
        results << ans.second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    }

    results.close();

    cout << "   Terminado!!" << endl;

    results.open("results_new/get_all_word_occurrences/sii_gawo_" + id + ".csv");
    results << "occurences\ttime\n";

    cout << "Inicia el proceso de Simple_II..." << endl;

    for(uint64_t word = 1; word <= 418038; word++)
    {
        begin = clock();
        ans = sii.get_all_word_ocurrences(word);
        end = clock();
        results << ans.second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    }
    results.close();

    cout << "   Terminado!!" << endl;

    return 1;
}

