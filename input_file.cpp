#include <iostream>
#include <fstream>
#include <vector>
#include "Input_file.h"


using namespace std;


std::vector<unsigned char> input_file() {
    ifstream file;
    file.open("test.txt", ios::in | ios::binary);

    if (!file.is_open()) {
        cout << "Falied to open file.\n";

        file.close();

        return 0;
    }
    
    vector<unsigned char> res;
    unsigned char c;
     
    while ((c = file.get()) != EOF) {
        res.push_back(c);
    }

    file.close();

    return res;
}