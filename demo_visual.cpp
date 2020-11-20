#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include "som.h"

typedef vector<vector<int>> vector2d;

using namespace std;

vector2d generateInput(int n){
    vector2d v;
    
    for (int i = 0; i < n; i++) {
        v.push_back(vector<int>{rand() % 600, rand() % 600});
    }
    
    return v;
}

int main(int argc, char **argv){
    
    if (argc < 5){
        cout << "wrong argumetns provided" << endl;
        return 1;
    }
    
    int size = -1;
    int inputCnt = -1;
    
    if (string(argv[1]) == "-s")
        size = atoi(argv[2]);
    else if (string(argv[1]) == "-i")
        inputCnt = atoi(argv[2]);
    
    if (string(argv[3]) == "-s")
        size = atoi(argv[4]);
    else if (string(argv[3]) == "-i")
        inputCnt = atoi(argv[4]);
    
    if (size == -1 || inputCnt == -1) {
        cout << "wrong argumetns provided" << endl;
        return 1;
    }
    
    if (size < 2 || size > 12) {
        cout << "invalid size" << endl;
        return 1;
    }
    
    if (inputCnt < 1) {
        cout << "invalid input count" << endl;
        return 1;
    }
    
    
    srand((unsigned) time(0));
    vector2d testInput = generateInput(inputCnt);
    
    cSom mySom = cSom(size, size, 2);
    mySom.initGrid();
    mySom.demo_learn(testInput, 1000000);
    
    
    cerr << "BYE" << endl;
    return 0;
    
}
