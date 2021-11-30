#include "Alphabet_Partitioning.cpp"
#include "Simple_II.cpp"
#include <time.h>

int main(){
    
    string text_path = "text/text_2.txt";
	string alph_path = "alphabets/eng_1.txt";

    cout << "[Alphabet_Partitioning]" << endl;
    Alphabet_Partitioning ap(text_path, alph_path);
    cout << "[Simple_II]" << endl;
    Simple_II sii(text_path);

    time_t begin;
    time_t end;
    cout << "Busco en [Alphabet_Partitioning]" << endl;
    begin = clock();
    string wea_1 = ap.get_snippet(301, 5301);
    end = clock();
    cout << "   [Snippet] Se demoró " << (float)(end - begin)/CLOCKS_PER_SEC << endl;

    begin = clock();
    pair<vector<uint64_t>*, uint64_t> wea_2 = ap.get_all_word_ocurrences("lectures");
    end = clock();
    cout << "   [Words] Se demoró " << (float)(end - begin)/CLOCKS_PER_SEC << "| Ocurrencias: "  << wea_2.second << endl;

    begin = clock();
    pair<vector<uint64_t>*, uint64_t> wea_3 = ap.get_all_phrase_ocurrences("it is");
    end = clock();
    cout << "   [Phrase] Se demoró " << (float)(end - begin)/CLOCKS_PER_SEC << "| Ocurrencias: "  << wea_3.second << endl;

    cout << "Busco en [Simple_II]" << endl;
    begin = clock();
    string wea_4 = sii.get_snippet(300, 5300);
    end = clock();
    cout << "   [Snippet] Se demoró " << (float)(end - begin)/CLOCKS_PER_SEC << endl;

    begin = clock();
    pair<vector<uint64_t>*, uint64_t> wea_5 = sii.get_all_word_ocurrences("lectures");
    end = clock();
    cout << "   [Words] Se demoró " << (float)(end - begin)/CLOCKS_PER_SEC << "| Ocurrencias: "  << wea_5.second << endl;

    begin = clock();
    pair<vector<uint64_t>*, uint64_t> wea_6 = sii.get_all_phrase_ocurrences("it is");
    end = clock();
    cout << "   [Phrase] Se demoró " << (float)(end - begin)/CLOCKS_PER_SEC << "| Ocurrencias: "  << wea_6.second << endl;




}
