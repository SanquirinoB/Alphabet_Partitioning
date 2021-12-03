#include "Alphabet_Partitioning.cpp"
#include "Simple_II.cpp"
#include <time.h>

int main(){
    
    string text_path = "text/proteins_3(1).txt";
	string alph_path = "alphabets/proteins_alphabet.txt";

    cout << "[Alphabet_Partitioning]" << endl;
    Alphabet_Partitioning ap(text_path, alph_path);
    uint64_t ap_size = ap.get_text_size();
    cout << "[Simple_II]" << endl;
    Simple_II sii(text_path);

    ofstream results;
    results.open("results/get_snippet/ap_gs_p3.csv");

    time_t begin;
    time_t end;
    string ans;
    results << "lenght\ttime\n";

    double sizes[] = {1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 10.0, 10.5, 11.0, 11.5, 12.0, 12.5, 13.0, 13.5, 14.0, 14.5, 15.0, 15.5, 16.0, 16.5, 17.0, 17.5, 18.0, 18.5, 19.0, 19.5, 20.0, 20.5, 21.0, 21.5, 22.0, 22.5, 23.0};
    
    vector<pair<uint64_t, uint64_t>> pairs;
    uint64_t i;
    uint64_t n = 0;
    for(double div:sizes)
    {   
        uint64_t l = (uint64_t)(ap_size/pow(2,div));
        if(pow(2,div) > ap_size | l > 1153140) continue;
        cout << ap_size << "|" << pow(2,div) << "|" << l << endl;
        uint64_t m = ap_size - l + 1;
        for(int cosa = 1; cosa <= 10; cosa++)
        {
            i = (uint64_t)(rand() % m) + 1;
            pairs.push_back(make_pair(i,i+l));
            n++;
        }
    }
    for(uint64_t i = 0; i < n; i++)
    {
        begin = clock();
        ans = ap.get_snippet(pairs[i].first, pairs[i].second);
        end = clock();
        results << (pairs[i].second - pairs[i].first) << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    }

    results.close();

    results.open("results/get_snippet/sii_gs_p3.csv");
    results << "lenght\ttime\n";
    for(uint64_t i = 0; i < n; i++)
    {
        begin = clock();
        ans = sii.get_snippet(pairs[i].first, pairs[i].second);
        end = clock();
        results << (pairs[i].second - pairs[i].first) << "\t" << (float)(end - begin)/CLOCKS_PER_SEC << "\n";
    }
    results.close();

    return 1;
}
