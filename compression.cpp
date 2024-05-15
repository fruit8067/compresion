#include <iostream>
#include <string>
#include <vector>
#include "input_file.cpp"

struct Token {
    int offset;
    int length;
    char nextChar;
};

std::vector<Token> compressLZ77(const std::string& input, int windowSize) {
    std::vector<Token> tokens;
    int inputSize = input.size();
    int currentIndex = 0;

    while (currentIndex < inputSize) {
        Token token;
        token.offset = 0;
        token.length = 0;
        token.nextChar = input[currentIndex];

        for (int i = std::max(0, currentIndex - windowSize); i < currentIndex; ++i) {
            int j = 0;
            while (currentIndex + j < inputSize && input[i + j] == input[currentIndex + j] && j < windowSize) {
                ++j;
            }
            if (j > token.length) {
                token.offset = currentIndex - i;
                token.length = j;
                token.nextChar = input[currentIndex + j];
            }
        }

        tokens.push_back(token);
        currentIndex += (token.length + 1);
    }

    return tokens;
}

int main() {
    std::string input = input_file();
    int windowSize = 4;

    std::vector<Token> compressed = compressLZ77(input, windowSize);

    std::cout << "Compressed data:\n";
    for (const auto& token : compressed) {
        std::cout << "(" << token.offset << "," << token.length << "," << token.nextChar << ")\n";
    }

    return 0;
}
