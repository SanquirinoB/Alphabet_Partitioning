#include "AP_exp.cpp"
#include "SII_exp.cpp"
#include <time.h>


int main(){
    
    string text_path, alph_path, id, size;
	// cout << "Ingrese el path del archivo a comprimir: ";
    // cin >> text_path;
    // cout << "Ingrese la cantidad de palabras: ";
    // cin >> size;
    // cout << "Ingrese el path del alfabeto a usar: ";
    // cin >> alph_path;
    // cout << "Ingrese el id para resultados: ";
    // cin >> id;

    text_path = "tmp/eng_3_size_9225123.txt";
    alph_path = "alphabets/english_alphabet.txt";
    id = "eng_3";
    size = "9225123";

    // text_path = "tmp/eng_4_size_2306280.txt";
    // alph_path = "alphabets/english_alphabet.txt";
    // id = "eng_4";
    // size = "2306280";

    cout << "[Alphabet_Partitioning]" << endl;
    Alphabet_Partitioning *ap = new Alphabet_Partitioning(text_path, stoul(size), alph_path, id, false, false);
    uint64_t ap_size = ap->get_text_size();
    cout << "[Simple_II]" << endl;
    Simple_II *sii = new Simple_II(text_path);

    ofstream results;
    results.open("results_new/get_snippet/ap_gs_" + id + "_2.csv");

    time_t begin;
    time_t end;
    uint64_t ans;
    results << "length\ttime\n";

    int sizes[] = {10, 100};
    
    vector<pair<uint64_t, uint64_t>> pairs;
    uint64_t i;
    uint64_t n = 0;
    for(double l:sizes)
    {   
        uint64_t m = ap_size - l + 1;
        for(int cosa = 1; cosa <= 1000; cosa++)
        {
            i = (uint64_t)(rand() % m) + 1;
            pairs.push_back(make_pair(i,i+l));
            n++;
        }
    }
    cout << "Inicia el proceso de Alphabet Partitioning..." << endl;

    for(uint64_t i = 0; i < n; i++)
    {
        begin = clock();
        ans = ap->get_snippet(pairs[i].first, pairs[i].second);
        end = clock();
        results << (pairs[i].second - pairs[i].first) << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    }

    results.close();

    cout << "   Experimentacion finalizada!" << endl;
    cout << "   Liberando memoria..." << endl;
    delete ap;
    
    cout << "   Terminado!!" << endl;

    results.open("results_new/get_snippet/sii_gs_" + id + "_2.csv");
    results << "length\ttime\n";

    cout << "Inicia el proceso de Simple_II..." << endl;

    for(uint64_t i = 0; i < n; i++)
    {
        begin = clock();
        ans = sii->get_snippet(pairs[i].first, pairs[i].second);
        end = clock();
        results << (pairs[i].second - pairs[i].first) << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    }
    results.close();

    cout << "   Terminado!!" << endl;

    return 1;
}
