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
    uint64_t ap_size = ap.get_text_size();
    cout << "[Simple_II]" << endl;
    Simple_II sii(text_path);

    ofstream results;
    results.open("results/get_all_phrase_occurrences/ap_gp_" + id + ".csv");

    time_t begin;
    time_t end;
    pair<vector<uint64_t>*, uint64_t> ans;
    results << "length\toccurences\ttime\n";

    ifstream phrases;
    phrases.open("experiments/phrases_" + id + ".txt");
    string phrase;

    cout << "Inicia el proceso de Alphabet Partitioning..." << endl;

    while(getline(phrases, phrase))
    {
        cout << "Inicia busqueda de frase de largo: " << phrase.length() << endl;
        begin = clock();
        ans = ap.get_all_phrase_ocurrences(phrase);
        end = clock();
        cout << "   Encontrada en " << (float)(end - begin)/CLOCKS_PER_SEC << "segundos." << endl;
        results << phrase.length() << "\t" << ans.second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    }
    phrases.clear();
    phrases.seekg(0);
    results.close();

    cout << "   Terminado!!" << endl;

    results.open("results/get_all_phrase_occurrences/sii_gp_" + id + ".csv");
    results << "length\toccurences\ttime\n";

    cout << "Inicia el proceso de Simple_II..." << endl;

    while(getline(phrases, phrase))
    {
        begin = clock();
        ans = sii.get_all_phrase_ocurrences(phrase);
        end = clock();
        results << phrase.length() << "\t" << ans.second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    }
    phrases.close();
    results.close();

    cout << "   Terminado!!" << endl;

    return 1;
}
