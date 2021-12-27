#include "AP_exp.cpp"
#include "SII_exp.cpp"


int main(){
    
    string text_path, alph_path, id, size;
    text_path = "tmp/eng_3_size_9225123.txt";
    alph_path = "alphabets/english_alphabet.txt";
    id = "eng_3";
    size = "9225123";
    int b_size = 50000000;

    cout << "[ COMPRESION 50MB ]" << endl;

    cout << "[Alphabet_Partitioning]" << endl;
    Alphabet_Partitioning *ap = new Alphabet_Partitioning(text_path, stoul(size), alph_path, id, false, false);
    uint64_t ap_size = ap->size();

    delete ap;

    cout << "[Simple_II]" << endl;
    Simple_II *sii = new Simple_II(text_path);
    uint64_t sii_size = sii->size();

    delete sii;

    cout << "[AP] comprime en " << ((ap_size/b_size)*100) << "%, estructura pesa " << ap_size << " bytes." << endl;
    cout << "[SII] comprime en " << ((sii_size/b_size)*100) << "%, estructura pesa " << sii_size << " bytes." << endl;

    text_path = "tmp/eng_4_size_2306280.txt";
    alph_path = "alphabets/english_alphabet.txt";
    id = "eng_4";
    size = "2306280";
    b_size = 12000000;

    cout << "[ COMPRESION 12MB ]" << endl;

    cout << "[Alphabet_Partitioning]" << endl;
    ap = new Alphabet_Partitioning(text_path, stoul(size), alph_path, id, false, false);
    ap_size = ap->size();

    delete ap;

    cout << "[Simple_II]" << endl;
    sii = new Simple_II(text_path);
    sii_size = sii->size();

    delete sii;

    cout << "[AP] comprime en " << ((ap_size/b_size)*100) << "%, estructura pesa " << ap_size << " bytes." << endl;
    cout << "[SII] comprime en " << ((sii_size/b_size)*100) << "%, estructura pesa " << sii_size << " bytes." << endl;

   

    return 1;
}
