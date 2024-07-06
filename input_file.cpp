#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "input_file.h"


using namespace std;


vector<unsigned char> input_file(string path) {
    ifstream file;
    file.open(path);

    if (!file.is_open()) {
        cout << "Falied to open file. wtfadf\n";

        file.close();

        return vector<unsigned char>();
    }
    
    vector<unsigned char> res;
    int c;
    
    while ((c = file.get()) != EOF) {
        res.push_back((unsigned char)c);
    }

    file.close();

    return res;
}
