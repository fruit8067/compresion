#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

static unordered_map<char,int> charFreq;
static int intFreq[10] = {0,0,0,0,0,0,0,0,0,0};


vector<unsigned char> input_file(string path) {
    ifstream file;
    file.open(path, ios::in | ios::binary);

    if (!file.is_open()) {
        cout << "Falied to open file.\n";

        file.close();

        return vector<unsigned char>();
    }
    

    vector<unsigned char> res;
    int c;
    

    while ((c = file.get()) != 'EOF' && (c = file.get()) != '/') {
        res.push_back((unsigned char)c);
    }
    

    file.close();

    return res;
}


int main(){

    return 0;
}
