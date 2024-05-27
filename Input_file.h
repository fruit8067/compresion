#ifndef input_file_H   // To make sure you don't declare the function more than once by including the header multiple times.
#define input_file_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

vector<unsigned char> input_file(string path);

#endif