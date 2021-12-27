#include "AP_exp.cpp"
#include "SII_exp.cpp"
#include <time.h>

int main(){
    
    // string text_path, alph_path, id, size;
	// cout << "Ingrese el path del archivo a comprimir: ";
    // cin >> text_path;
    // cout << "Ingrese la cantidad de palabras: ";
    // cin >> size;
    // cout << "Ingrese el path del alfabeto a usar: ";
    // cin >> alph_path;
    // cout << "Ingrese el id para resultados: ";
    // cin >> id;
    Alphabet_Partitioning *ap;
    Simple_II *sii;
    ofstream results;
    ifstream phrases;
    time_t begin;
    time_t end;
    pair<vector<uint64_t>*, uint64_t> *ans = new pair<vector<uint64_t>*, uint64_t>;
    vector<uint64_t>* p = new vector<uint64_t>;
    string phrase, word;
    uint64_t p_len = 0;
    uint64_t wea = 1;
    uint64_t ind = 1;

    // phrases.open("tmp/eng_4_phrases.txt");

    // cout << "[ Inicia ENG_4 ] " << endl << endl;
    // cout << "[Alphabet_Partitioning]" << endl;
    // ap = new Alphabet_Partitioning("tmp/eng_4_size_2306280.txt", 2306280, "alphabets/english_alphabet.txt", "eng_4", false, false);
    
    // results.open("results_new/get_all_phrase_occurrences/ap_gp_eng_4_2.csv");

    // results << "length\toccurences\ttime\n";

    // cout << "Inicia el proceso de Alphabet Partitioning..." << endl;
    
    // while(getline(phrases, phrase))
    // {
    //     stringstream s_line(phrase);
    //     while(getline(s_line, word, '/'))
    //     {
    //         p->push_back(stoul(word));
    //         p_len++;
    //     }

    //     begin = clock();
    //     *ans = ap->get_all_phrase_ocurrences(*p);
    //     end = clock();
    //     results << p_len << "\t" << ans->second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    //     if(ans->second == 0)
    //     {
    //         cout << "Frase " << wea << " no funciona!!!!"<< endl;
    //         return 1;
    //     }
    //     wea++;
    //     p->clear();
    //     p_len = 0;
    // }
    // phrases.clear();
    // phrases.seekg(0);
    // results.close();
    
    // cout << "   Experimentacion finalizada!" << endl;
    // cout << "   Liberando memoria..." << endl;
    // delete ap;

    // cout << "Terminado!!" << endl;

    // cout << "[Simple_II]" << endl;
    // sii = new Simple_II("tmp/eng_4_size_2306280.txt");
    // cout << "Inicia el proceso de Simple_II..." << endl;

    // results.open("results_new/get_all_phrase_occurrences/sii_gp_eng_4_2.csv");
    // results << "length\toccurences\ttime\n";
    // cout << "length\toccurences\ttime\n";

    
    // while(getline(phrases, phrase))
    // {
    //     stringstream s_line(phrase);
    //     while(getline(s_line, word, '/'))
    //     {
    //         p->push_back(stoul(word));
    //         p_len++;
    //     }
    //     if(p_len == 1339) break;
    //     cout << "Busco frase " << ind << " de " << p_len << " palabras" << endl;
    //     begin = clock();
    //     *ans = sii->get_all_phrase_ocurrences(*p);
    //     end = clock();
    //     cout << p_len << "\t" << ans->second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    //     results << p_len << "\t" << ans->second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    //     if(ans->second == 0)
    //     {
    //         cout << "Frase " << ind << " no funciona!!!!"<< endl;
    //         return 1;
    //     }
        
    //     p->clear();
    //     p_len = 0;
    //     ind++;
    //     cout << "   Encontrada en " << (float)(end - begin)/CLOCKS_PER_SEC << " segundos con " << ans->second << " ocurrencias" << endl;
    //     results << "   Encontrada en " << (float)(end - begin)/CLOCKS_PER_SEC << " segundos con " << ans->second << " ocurrencias" << endl;
    //     cout << "   Limpiamos memoria..." << endl;       
    // }

    // phrases.close();
    // results.close();

    // cout << "Terminado!!" << endl;

    // delete sii;

    cout << "[ Inicia ENG_3 ] " << endl;

    cout << "[Alphabet_Partitioning]" << endl;
    ap = new Alphabet_Partitioning("tmp/eng_3_size_9225123.txt", 9225123, "alphabets/english_alphabet.txt", "eng_3", false, false);

    results.open("results_new/get_all_phrase_occurrences/ap_gp_eng_3_2.csv", ios_base::app);

    results << "length\toccurences\ttime\n";

    phrases.open("tmp/eng_3_phrases.txt");
    p_len = 0;

    cout << "Inicia el proceso de Alphabet Partitioning..." << endl;
    wea = 1;
    while(getline(phrases, phrase))
    {
        stringstream s_line(phrase);
        while(getline(s_line, word, '/'))
        {
            p->push_back(stoul(word));
            p_len++;
        }
        begin = clock();
        *ans = ap->get_all_phrase_ocurrences(*p);
        end = clock();
        results << p_len << "\t" << ans->second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
        cout << p_len << "\t" << ans->second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
        if(ans->second == 0)
        {
            cout << "Frase " << wea << " no funciona!!!!"<< endl;
            return 1;
        }
        wea++;
        p->clear();
        p_len = 0;
    }
    phrases.clear();
    phrases.seekg(0);
    results.close();
    
    cout << "   Experimentacion finalizada!" << endl;
    cout << "   Liberando memoria..." << endl;
    delete ap;

    cout << "       Terminado!!" << endl;

    cout << "[Simple_II]" << endl;
    sii = new Simple_II("tmp/eng_3_size_9225123.txt");
    cout << "Inicia el proceso de Simple_II..." << endl;

    results.open("results_new/get_all_phrase_occurrences/sii_gp_eng_3_2.csv");
    results << "length\toccurences\ttime\n";
    cout << "length\toccurences\ttime\n";

    ind = 1;
    while(getline(phrases, phrase))
    {
        stringstream s_line(phrase);
        while(getline(s_line, word, '/'))
        {
            p->push_back(stoul(word));
            p_len++;
        }
        if(p_len == 334) break;
        cout << "Busco frase " << ind << " de " << p_len << " palabras" << endl;
        begin = clock();
        *ans = sii->get_all_phrase_ocurrences(*p);
        end = clock();
        results << p_len << "\t" << ans->second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
        cout << p_len << "\t" << ans->second << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
        if(ans->second == 0)
        {
            cout << "Frase " << ind << " no funciona!!!!"<< endl;
            return 1;
        }
        p->clear();
        p_len = 0;
        ind++;
        cout << "   Encontrada en " << (float)(end - begin)/CLOCKS_PER_SEC << " segundos con " << ans->second << " ocurrencias" << endl;

    }

    phrases.close();
    results.close();

    cout << "   Terminado!!" << endl;

    delete sii;

    return 1;
}
