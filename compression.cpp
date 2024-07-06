#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "input_file.cpp"
#include <stdlib.h>
#include <fstream> 
#include <bitset>
#include <windows.h>
#include <cstdint>
#define Q_SZ 100


std::unordered_map<char, int> charFreq;
int intFreq[10] ={0,0,0,0,0,0,0,0,0,0};

void Lz77OutputCharFrequency(std::string output){
    for(int i = 0; i < output.length(); i++){
        if(isdigit(output[i])){
            intFreq[(int)output[i]] += 1;
        }
        else{
            charFreq[output[i]] += 1;
        }
    }
}


//만들어진 허프만 코드는 여기에 이진수 형태로 저장된다.
std::unordered_map<char, std::string> HuffmanCodeTable;
struct node{
    char c; // 허프만 코드 할당 문자
    int freq; //빈도수
    struct node* left;
    struct node* right;
};

//min_heap == 우선순위 큐, 빈도수가 적을수록 우선순위가 높다
struct node* PQUE[Q_SZ + 1];
int idx = 0;

//huffman code를 만드는 전역변수 배열
char code[30] = {0}; //초기에는 null character로 초기화
int code_idx = -1; //코드를 추가해야 하는 위치, 011

struct node* createNode(char _c, int _freq) {
    struct node* _new_node = (struct node*)malloc(sizeof(struct node));
    _new_node->c = _c;
    _new_node->freq = _freq;
    _new_node->left = _new_node->right = 0;
    return _new_node;
};

void enqueue(struct node* _new_node){
    //overflow(which i dont give a shit)
    if (idx == Q_SZ){
        return;
    }
    idx += 1;
    PQUE[idx] = _new_node; // 노드의 주소를 넣는다.
    //자기 인덱스
    int _cur_idx = idx;
    //부모 인덱스
    int _p_idx = _cur_idx / 2;

    while (_cur_idx > 1){
        //내가 부모보다 작으면 (빈도수가 작으면, 위로 올라간다. 부모와 자리교체)
        if (PQUE[_cur_idx]->freq < PQUE[_p_idx]->freq){

            //자리 교체
            struct node* temp = PQUE[_cur_idx];
            PQUE[_cur_idx] = PQUE[_p_idx];
            PQUE[_p_idx] = temp;
            
            //
            _cur_idx = _p_idx;
            _p_idx = _cur_idx / 2;
        }
        else{
            break;
        }
    }
}

struct node* dequeue(void){
    //queue empty
    if (idx == 0){
        return 0;
    }
    struct node* _res = PQUE[1]; //나갈 놈
    //나간놈 대신해서, 마지막 애가 올라온다.
    //minheap 조건 (complete binary tree 조건 충족을 위해서)
    PQUE[1] = PQUE[idx];
    idx--;

    //minheap조건 : 부모는 자식보다 작아야 한다.

    int _cur_idx = 1;
    int _child_idx = _cur_idx * 2; //일단 왼쪽 자식

    while (1){
        if(_child_idx > idx){ //자식이 없어요ㅠ
            break;
        }
        //1. 일단 오른쪽 자식이 있어야 하고, 2. 왼, 오: 둘중 더 작은 애와만 비교하면 된다.
        if((_child_idx + 1 <= idx) && (PQUE[_child_idx]->freq > PQUE[_child_idx + 1]->freq)) {
            _child_idx += 1; //진짜로 비교해야할 애는 오른쪽 아이임.
        }
        //드디어 결전의 날, 부모와 자식간의 혈투
        if(PQUE[_cur_idx]->freq > PQUE[_child_idx]->freq) {
            //자식에게 발림(자식보다 freq가 큼) -> 자식에게 자리를 주고 내려와야 함.
            
            //자리 교체
            struct node* temp = PQUE[_cur_idx];
            PQUE[_cur_idx] = PQUE[_child_idx];
            PQUE[_child_idx] = temp;
            _cur_idx = _child_idx;
            _child_idx = 2 * _cur_idx;

        }
        else{
            break;
        }
    }
    return _res;
}

//minheap에 담긴 노드 정보들을 이용해서
//huffman tree를 만들고
//마지막에 root node에 대한 주소를 반환한다.
struct node* buildHuffmanTree(void){

    //1. 2개를 꺼낸다.
    //단, 1개만 있으면, 반환하고 끝
    while(1){
        struct node* _first = dequeue();
        struct node* _second = dequeue();

        if(_second == 0){
            return _first;
        }
        //2. 이진트리를 만들어서, 다신 enqueue
        struct node* _combined = createNode(0, _first->freq + _second->freq);
        _combined->left = _first;
        _combined->right = _second;

        enqueue(_combined);
    }
}

void genHuffmanCode(struct node* _node){
    //leaf node(맨 밑에 있는 노드)에 도착했음
    if(_node == 0){
        return;
    }

    //왼쪽 자식 노드로 갈때마다 0을 추가
    code_idx++;
    code[code_idx] = '0';
    genHuffmanCode(_node->left);
    //아 맨 밑에 있는 노드에 도착헀어? 그럼 왼쪽 갔다는 표지 지울께
    code[code_idx] = 0;
    code_idx--;


    if(_node->c != 0){ // node에 글자가 있다.
        HuffmanCodeTable[_node->c] = code; // '글자'에 할당된 이진수는 '이진수' 이다.
    }
    
    //오른쪽 자식 노드로 갈때마다 1을 추가
    code_idx++;
    code[code_idx] = '1';
    //아 맨 밑에 있는 노드에 도착헀어? 그럼 오른쪽 갔다는 표지 지울께
    genHuffmanCode(_node->right);
    code[code_idx] = 0;
    code_idx--;
}

struct Token {
    int offset;
    int length;
    unsigned char nextChar;
};

std::vector<Token> compressLZ77(const std::vector<unsigned char> input, int windowSize) {
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

//data : 2진수 허프만부호화, charFreq : 문자 빈도수, intFreq: 숫자 빈도수
void makeAFile(std::string data, std::unordered_map<char, int> charFreq, int intFreq[10]){
    FILE* fp = fopen("./output/output.sngr", "wb");

	DWORD64 length = data.length();
    BYTE* val = (BYTE*)malloc(length);

    for (int i = 0; i < length; i++) {
        val[i] = std::stoi(data.substr(i*8, 8), nullptr, 2);
    }

	fwrite(&length, sizeof(DWORD32), 1, fp);
    fwrite(val, sizeof(BYTE), length, fp);

    BYTE totFreq = charFreq.size();

    for(int i = 0; i<10; i++) {
        if(intFreq[i] == 0) {
            continue;
        }

        totFreq += 1;
    }

    fwrite(&totFreq, sizeof(BYTE), 1, fp);

    for(int i = 0; i<10; i++) {
        if(intFreq[i] == 0) {
            continue;
        }

        BYTE key = i;
        int value = intFreq[i];
        fwrite(&key, sizeof(BYTE), 1, fp);
        fwrite(&value, sizeof(int), 1, fp);
    }


    for(pair<char,int> elem : charFreq) {
        char key = elem.first;
        int value = elem.second;
        fwrite(&key, 1, 1, fp);
        fwrite(&value, sizeof(int), 1, fp);
    }

    free(val);
	fclose(fp);
} 

//entire compression(encode) process function
int main() {
    vector<unsigned char> input = input_file("./test.txt");
    int windowSize = 10000;

    std::vector<Token> compressed = compressLZ77(input, windowSize);

    std::string Lz77Output;


    for (const auto& token : compressed) {
        //std::cout << token.offset << "," << token.length << "," << token.nextChar << "/";
        Lz77Output += "(";
        Lz77Output += std::to_string(token.offset);
        Lz77Output += ",";
        Lz77Output += std::to_string(token.length);
        Lz77Output += ",";
        Lz77Output.push_back(token.nextChar);
        Lz77Output += ")";
    }

    Lz77OutputCharFrequency(Lz77Output);
    //std::cout << Lz77Output << "\n";
    for(pair<char,int> elem : charFreq){
        enqueue(createNode(elem.first, elem.second));
        cout << elem.first << " " << elem.second << endl;
    }

    for(int i = 0; i<10; i++){
        if(intFreq[i] == 0){
            continue;
        }
        enqueue(createNode(static_cast<char>(48 + i), intFreq[i]));
        cout << i << " " << intFreq[i] << endl;
    }

    //완성된 노드
    struct node* completed_node = buildHuffmanTree();

    //std::cout << completed_node->freq;

    genHuffmanCode(completed_node);
    std::cout<< Lz77Output <<std::endl;
    for(std::pair<char, std::string> elem : HuffmanCodeTable){
        std::cout<<"key : "<<elem.first<<" value : "<<elem.second<<std::endl;
    }

    std::string outputData;
    for(char chr : Lz77Output){
        outputData += HuffmanCodeTable[chr];
    }

    std::cout<<outputData;
    //makeAFile(outputData, charFreq, intFreq);
    free(completed_node);

    return 0;
}


//enqueue testing

//    for(int i = 1; i<=idx; i++){
 //       printf("%c, %d \n", PQUE[i]->c, PQUE[i]->freq);
 //   }

//dequeue testing

//    while(1){
//        struct node* temp = dequeue();
 //       if(temp != 0){
  //          printf("%c --------------> %d \n", temp->c, temp->freq);
 //       }
 //   }
