#include <sdsl/wavelet_trees.hpp>
#include <iostream>
#include <map>

using namespace std;
using namespace sdsl;

int main(){
    fstream my_file;
    my_file.open("alphabets/default.txt", ios::in);
    if (!my_file) {
		cout << "No such file";
	}
	else {
		char ch;

		while (1) {
			my_file >> ch;
			if (my_file.eof())
				break;
            if(ch == ',') cout << "weaaaa";
            cout << ch;
		}

	}
	my_file.close();
    return 0;
}