#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <unistd.h>
#include "som.h"

typedef vector<vector<int>> vector2d;

using namespace std;

vector2d generateInput(int n, int clustersCnt){
    vector2d v;
    vector<int> clusters;
    
    for (int i = 0; i < clustersCnt; i++) {
        clusters.push_back(rand() % 800);
        clusters.push_back(rand() % 800);
    }
    
    
    for (int i = 0; i < n; i++) {
        int r = rand() % clustersCnt;
        int offsetX = clusters[r*2];
        int offsetY = clusters[r*2+1];

        v.push_back(vector<int>{rand() % 200 + offsetX, rand() % 200 + offsetY});
    }
    
    return v;
}

int main(int argc, char **argv){

    int size = 10;
    
    if (argc < 4) {
        cerr << "wrong arguments";
        return 1;
    }
    
    int inputCnt = atoi(argv[1]);
    int clustersCnt = atoi(argv[2]);
    bool displayProgress = string(argv[3]) == "-p";
    
    srand((unsigned) time(0));
    vector2d testInput = generateInput(inputCnt, clustersCnt);
    
    cSom mySom = cSom(size, size, 2);
    mySom.initGrid();
    
    mySom.print({-1, -1});
    
    for (unsigned i = 0; i < testInput.size(); i++) {
        int bmu = mySom.getBmu(testInput[i]);
         mySom.adjustWeights(bmu, testInput[i]);
         
         if (displayProgress) {
            mySom.print(testInput[i]);
            usleep(100000);
         }
    }
    mySom.print({-1, -1});
    cout << "0x0";
    
    
    cerr << "BYE" << endl;
    return 0;
    
}
