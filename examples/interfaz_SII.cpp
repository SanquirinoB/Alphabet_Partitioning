#include "../Simple_II.cpp"

int main(){
    
	string text_path = "../text/text_3.txt";

	Simple_II test(text_path);

	cout << "Funcionalidades disponibles: \n(1) Get Snippet \n(2) All Word Ocurrences\n(3) All Phrase Ocurrences\n(4) Exit" << endl;
	int option;
	uint64_t i,j;
	string input_text;

	// TODO: Porque las consultas 2 y 3 rompen el while?

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
			pair<vector<uint64_t>*, uint64_t> result = test.get_all_word_ocurrences(input_text);
			cout << "Resultado: " << endl;
			cout << result.second << " ocurrencias" << endl;
			for(uint64_t pos : (*result.first))
			{
				cout << pos << '|';
			}
			cout << endl;

			input_text = "";

		} else if(option == 3)
		{
			// TODO: porque esta consulta rompe el while del input??
			cout << "[All Phrase Ocurrences] Ingrese una frase (string):" << endl;
			cout << "	Frase (string): ";
			cin >> input_text;
			pair<vector<uint64_t>*, uint64_t> result = test.get_all_phrase_ocurrences(input_text);
			cout << "Resultado: " << endl;
			cout << result.second << " ocurrencias" << endl;
			for(uint64_t pos : (*result.first))
			{
				cout << pos << '|';
			}
			cout << endl;

			input_text = "";
		} else if(option == 4){
			return 1;
		}
	}
}