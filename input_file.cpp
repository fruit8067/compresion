#include <iostream>
#include <fstream>
#include <string>
#include "Input_file.h"


using namespace std;


std::string input_file() {
    ifstream file;
    file.open("test.txt", ios::in | ios::binary);

    if (!file.is_open()) {
        cout << "Falied to open file.\n";

        file.close();

        return 0;
    }
    
    string str;
    int c;
    int i = 0;
    while ((c = file.get()) != EOF && i < 1000) {

        str += c;

        i += 1;
    }

    file.close();

    return str;
}