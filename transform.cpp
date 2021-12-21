#include "AP_exp.cpp"

int main()
{
    string text_path, size, alph_path, id;

    text_path = "experiments/phrases_e3.txt";
    size = "0";
    alph_path =  "alphabets/english_alphabet.txt";
    id = "eng_3_phrases";

    Alphabet_Partitioning AP(text_path, stoul(size), alph_path, id, true, true);

    text_path = "experiments/phrases_e4.txt";
    size = "0";
    alph_path =  "alphabets/english_alphabet.txt";
    id = "eng_4_phrases";

    Alphabet_Partitioning AP2(text_path, stoul(size), alph_path, id, true, true);

    // Transform phrases

}