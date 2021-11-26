#include "Alphabet_Partitioning.cpp"

int main(){
    
	string text_path = "text/text_3.txt";
	string alph_path = "alphabets/eng_1.txt";

	Alphabet_Partitioning test(text_path, alph_path);
	cout << "Funcionalidades disponibles: \n(1) Get Snippet \n(2) All Word Ocurrences\n(3) All Phrase Ocurrences\n(4) Exit" << endl;
	int option;
	uint64_t i,j;
	string input_text;
	while(true)
	{
		cout << "Ingrese una opcion (nro):" << endl;
		cin >> option;

		if(option == 1){
			cout << "[Get Snippet] Ingrese un intervalo (int):" << endl;
			cout << "	Inicio (int): ";
			cin >> i;
			cout << "	Fin (int): ";
			cin >> j;
			cout << "Resultado: " << test.get_snippet(i,j) << endl;

		} else if(option == 2)
		{
			cout << "[All Word Ocurrences] Ingrese una palabra (string):" << endl;
			cout << "	Palabra (string): ";
			cin >> input_text;

			cout << "Resultado: " << endl;
			for(uint64_t pos : (*test.get_all_word_ocurrences(input_text)))
			{
				cout << pos << '|';
			}
			cout << endl;

			input_text = "";
			sleep(2);

		} else if(option == 3)
		{
			cout << "[All Phrase Ocurrences] Ingrese una frase (string):" << endl;
			cout << "	Frase (string): ";
			cin >> input_text;

			cout << "Resultado: " << endl;
			for(uint64_t pos : (*test.get_all_phrase_ocurrences(input_text)))
			{
				cout << pos << '|';
			}
			cout << endl;

			input_text = "";
			sleep(2);
		} else if(option == 4){
			return 1;
		}
	}
}