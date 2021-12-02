#include "Alphabet_Partitioning.cpp"
#include "Simple_II.cpp"

int main(){

    // TODO:"Stand by por error en la forma en la que se computa el tamano de la clase"
    
    string text_path = "text/text_3.txt";
    double text_size = 1.7e6;
	string alph_path = "alphabets/eng_1.txt";

    cout << "[Alphabet_Partitioning]" << endl;
    Alphabet_Partitioning ap(text_path, alph_path);
    cout << "[Simple_II]" << endl;
    Simple_II sii(text_path);
    

    cout << "[Structure size]" << endl;
    cout << "Alphabet_Partitioning: " << sizeof(ap) << endl;
    cout << "Simple_II: " << sizeof(sii) << endl;
    cout << "[Compression Ratio]" << endl;
    cout << "Alphabet_Partitioning: " << ((double)sizeof(ap))/text_size << endl;
    cout << "Simple_II: " << ((double)sizeof(sii))/text_size << endl;

}