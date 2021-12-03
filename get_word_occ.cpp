#include "Alphabet_Partitioning.cpp"
#include "Simple_II.cpp"
#include <time.h>

int main(){
    
    string text_path, alph_path, id;
	cout << "Ingrese el path del archivo a comprimir: ";
    cin >> text_path;
    cout << "Ingrese el path del alfabeto a usar: ";
    cin >> alph_path;
    cout << "Ingrese el id para resultados: ";
    cin >> id;

    cout << "[Alphabet_Partitioning]" << endl;
    Alphabet_Partitioning ap(text_path, alph_path, id);
    cout << "[Simple_II]" << endl;
    Simple_II sii(text_path);

    ifstream test;
    test.open(alph_path);

    ofstream results;
    results.open("results/ap_gawo_p4.csv");

    string word;
    time_t begin;
    time_t end;
    int count = 0;
    pair<vector<uint64_t>*, uint64_t> ans;
    results << "word\toccurences\ttime\n";

    cout << "Inicia el proceso de Alphabet Partitioning..." << endl;
    
    while(getline(test, word))
    {
        begin = clock();
        ans = ap.get_all_word_ocurrences(word);
        end = clock();
        results << word << "\t" << ans.second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    }

    results.close();
    test.clear();
    test.seekg(0);

    cout << "   Terminado!!" << endl;

    results.open("results/sii_gawo_p4.csv");
    results << "word\toccurences\ttime\n";

    cout << "Inicia el proceso de Simple_II..." << endl;

    while(getline(test, word))
    {
        begin = clock();
        ans = sii.get_all_word_ocurrences(word);
        end = clock();
        results << word << "\t" << ans.second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    }
    results.close();
    test.close();

    cout << "   Terminado!!" << endl;

    return 1;
}

