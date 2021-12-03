#include "Alphabet_Partitioning.cpp"
#include "Simple_II.cpp"
#include <time.h>

int main(){
    
    string text_path = "text/english_4(1).txt";
	string alph_path = "alphabets/english_alphabet.txt";
    string id = "e4";

    cout << "[Alphabet_Partitioning]" << endl;
    Alphabet_Partitioning ap(text_path, alph_path);
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

    while(getline(phrases, phrase))
    {
        begin = clock();
        ans = ap.get_all_phrase_ocurrences(phrase);
        end = clock();
        results << phrase.length() << "\t" << ans.second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    }
    phrases.clear();
    phrases.seekg(0);
    results.close();

    results.open("results/get_all_phrase_occurrences/sii_gp_" + id + ".csv");
    results << "length\toccurences\ttime\n";
    while(getline(phrases, phrase))
    {
        begin = clock();
        ans = sii.get_all_phrase_ocurrences(phrase);
        end = clock();
        results << phrase.length() << "\t" << ans.second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    }
    phrases.close();
    results.close();

    return 1;
}
