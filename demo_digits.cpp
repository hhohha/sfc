#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "som.h"

using namespace std;

void readCSV(string filename, vector<vector<int>> &data, vector<int> &reference) {
    ifstream fin(filename);
    
    if (!fin){
        cerr << "wrong file specified" << endl;
        exit(1);
    }
    
    string line;
    
    getline(fin, line);
    
    while(getline(fin, line)) {
        vector<int> tmpv; 
        int n = 0;
        int ref = -1;
        
        for (int c: line) {
            
            if (c == ',') {
                if (ref == -1) {
                    ref = n;
                    reference.push_back(ref);
                } else{
                    tmpv.push_back(n);
                }
                n = 0;
                
            } else if (c >= '0' && c <= '9') {
                n *= 10;
                n += c - 48;
            }
        }
        tmpv.push_back(n);
        data.push_back(tmpv);
    } 
    
    cout << "\nREADING INPUT DONE\n";
}

int main(int argc, char **argv){
    
    if (argc != 2) {
        cerr << "wrong arguments" << endl;
        return 1;
    }

    srand((unsigned) time(0));
    
    vector<int> reference;
    vector<vector<int>> data;
    
    readCSV(argv[1], data, reference);
    
    cSom mySom = cSom(15, 15, 784);
    cout << "cSom created" << endl;
    mySom.initRandom(256);
    cout << "cSom initialized" << endl;
    
    mySom.learn(data, reference, 0, data.size() - 1000);
    mySom.classify(data, reference, data.size() - 1000, data.size());
    
    for (unsigned i = 0; i < mySom.sx; i++) { 
        for (unsigned j = 0; j < mySom.sy; j++) { 
            cout << setw(2) << mySom.getNeuron(i, j).topHit() << " ";
        }
        cout << endl;
    }
    
    return 0;
}
