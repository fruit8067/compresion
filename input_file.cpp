#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "input_file.h"


using namespace std;


vector<unsigned char> input_file(string path) {
    ifstream file;
    file.open(path, ios::in | ios::binary);

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