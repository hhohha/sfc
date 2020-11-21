#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <unistd.h>
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

    if (argc < 2) {
        cerr << "wrong arguments" << endl;
        return 1;
    }
    
    int size = 6;
    int inputCnt = atoi(argv[1]);
    
    srand((unsigned) time(0));
    vector2d testInput = generateInput(inputCnt);
    
    cSom mySom = cSom(size, size, 2);
    mySom.initGrid();
    
    mySom.print({-1, -1});
    for (unsigned i = 0; i < testInput.size(); i++) {
        int bmu = mySom.getBmu(testInput[i]);
         mySom.adjustWeights(bmu, testInput[i]);
         mySom.print(testInput[i]);
         usleep(1000000);
         
    }
    cout << "0x0";
    
    
    cerr << "BYE" << endl;
    return 0;
    
}
