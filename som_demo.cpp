#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <unistd.h>
#include "som.h"

using namespace std;

typedef vector<vector<int>> vector2d;

vector2d generateInput(int n){
    vector2d v;
    
    for (int i = 0; i < n; i++) {
        v.push_back(vector<int>{rand() % 500, rand() % 500});
    }
    
    return v;
}

vector2d generateClusteredInput(int n, int clusterCnt){
    vector2d v;
    vector<int> clusters;
    
    for (int i = 0; i < clusterCnt; i++) {
        clusters.push_back(rand() % 800);
        clusters.push_back(rand() % 800);
    }
    
    
    for (int i = 0; i < n; i++) {
        int r = rand() % clusterCnt;
        int offsetX = clusters[r*2];
        int offsetY = clusters[r*2+1];

        v.push_back(vector<int>{rand() % 200 + offsetX, rand() % 200 + offsetY});
    }
    
    return v;
}

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

void usage() {
    cerr << "usage:" << endl;
    cerr << "som_demo visual -i INPUT_CNT" << endl;
    cerr << "som_demo clusters -c CLUSTER_CNT -i INPUT_CNT [-nd]" << endl;
    cerr << "som_demo digits -f FILENAME"  << endl;
    exit(1);
}

void demo_visual(int inputCnt) {
    int size = 6;
    
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
}

void demo_clusters(int inputCnt, int clusterCnt, bool displayProgress) {
    int size = 10;
    vector2d testInput = generateClusteredInput(inputCnt, clusterCnt);
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
}

void demo_digits(string filename) {
    vector<int> reference;
    vector<vector<int>> data;
    
    readCSV(filename, data, reference);
    
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
}

int main (int argc, char **argv) {
    
    if (argc < 2)
        usage();

    bool displayProgress = true;
    int inputCnt = -1;
    int clusterCnt = -1;
    string filename = "";
    
     srand((unsigned) time(0));
    
    //add try/catch block for stoi
    try {
        for (int i = 2; i < argc; i++) {
            string arg(argv[i]);
            
            if (arg == "-i" && i < argc - 1) {
                i++;
                inputCnt = stoi(argv[i]);
            } else if (arg == "-c" && i < argc - 1) {
                i++;
                clusterCnt = stoi(argv[i]);
            } else if (arg == "-nd") {
                displayProgress = false;
            } else if (arg == "-f" && i < argc - 1) {
                i++;
                filename = argv[i];
            } else {
                usage();
            }
        } 
    } catch (invalid_argument e) {
        usage();
    }
        
    
    if (string(argv[1]) == "visual") {
        if (inputCnt < 1)
            usage();
        demo_visual(inputCnt);
    } else if (string(argv[1]) == "clusters") {
        if (inputCnt < 1 || clusterCnt < 1)
            usage();
        demo_clusters(inputCnt, clusterCnt, displayProgress);
    } else if (string(argv[1]) == "digits") {
        if (filename == "")
            usage();
        demo_digits(filename);
    } else {
        usage();
    }
    
    return 0;
}
