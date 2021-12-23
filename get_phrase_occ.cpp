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
    uint64_t ap_size = ap.get_text_size();
    cout << "[Simple_II]" << endl;
    Simple_II sii(text_path);

    ofstream results;
    results.open("results_new/get_all_phrase_occurrences/ap_gp_" + id + ".csv");

    time_t begin;
    time_t end;
    pair<vector<uint64_t>*, uint64_t> ans;
    vector<uint64_t>* p = new vector<uint64_t>;
    results << "length\toccurences\ttime\n";

    ifstream phrases;
    phrases.open("tmp/" + id + "_phrases.txt");
    string phrase, word;
    uint64_t p_len = 0;

    cout << "Inicia el proceso de Alphabet Partitioning..." << endl;

    while(getline(phrases, phrase))
    {
        stringstream s_line(phrase);
        while(getline(s_line, word, '/'))
        {
            p->push_back(stoul(word));
            p_len++;
        }

        begin = clock();
        ans = ap.get_all_phrase_ocurrences(*p);
        end = clock();
        results << p_len << "\t" << ans.second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
        p->clear();
        p_len = 0;
    }
    phrases.clear();
    phrases.seekg(0);
    results.close();

    cout << "   Terminado!!" << endl;

    results.open("results_new/get_all_phrase_occurrences/sii_gp_" + id + ".csv");
    results << "length\toccurences\ttime\n";

    cout << "Inicia el proceso de Simple_II..." << endl;

    while(getline(phrases, phrase))
    {
        stringstream s_line(phrase);
        while(getline(s_line, word, '/'))
        {
            p->push_back(stoul(word));
            p_len++;
        }
        begin = clock();
        ans = sii.get_all_phrase_ocurrences(*p);
        end = clock();
        results << p_len << "\t" << ans.second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
        p->clear();
        p_len = 0;
    }

    phrases.close();
    results.close();

    cout << "   Terminado!!" << endl;

    return 1;
}
