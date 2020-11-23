// module: som_demo.cpp - program to demonstrate use of cSom class
// author: Jan Hammer, xhamme00

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

int sleep_time = 0;    // sleep between iterations (for visualization purposes)

// generate vector of random 2D inputs anywhere in the grid
intMatrix generateInput(int n, int gridSize){
    intMatrix v;
    
    for (int i = 0; i < n; i++) {
        v.push_back(vector<int>{rand() % ((gridSize - 1) * 100) , rand() % ((gridSize - 1) * 100)});
    }
    
    return v;
}

// generate vector of random 2D inputs in the clusters
intMatrix generateClusteredInput(int n, int gridSize, int clusterCnt, int clusterSize){
    intMatrix v;
    vector<int> clusters;
    
    //generate clusters (by top left point), the whole cluster must be inside the grid
    for (int i = 0; i < clusterCnt; i++) {
        clusters.push_back(rand() % ((gridSize - 1) * 100 - clusterSize));
        clusters.push_back(rand() % ((gridSize - 1) * 100 - clusterSize));
    }
    
    // generate random inputs - only into the clusters
    for (int i = 0; i < n; i++) {
        int r = rand() % clusterCnt;
        int offsetX = clusters[r*2];
        int offsetY = clusters[r*2+1];

        v.push_back(vector<int>{rand() % clusterSize + offsetX, rand() % clusterSize + offsetY});
    }

    return v;
}

// parse the CSV used for trining digit recognition, return as vector of int vectors (data)
// and vector of reference digits
void readCSV(string filename, intMatrix &data, vector<int> &reference) {
    ifstream fin(filename);
    
    if (!fin){
        cerr << "wrong file specified" << endl;
        exit(1);
    }
    
    string line;
    
    getline(fin, line); // first row contins just name of columns - skip it
    
    while(getline(fin, line)) {
        vector<int> tmpv; // 
        int n = 0;
        int ref = -1;
        
        for (int c: line) {
            if (c == ',') {
                if (ref == -1) {
                    // the first number in a row is always the reference, save it to the vector
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
        
        // end of line - save the last number of the line and the entire vector
        tmpv.push_back(n);
        data.push_back(tmpv);
    } 
    
    cout << "PROGRESS: parsing the input finished\n";
}

void usage() {
    cerr << "usage:" << endl;
    cerr << "som_demo visual -i INPUT_CNT -s GRID_SIZE [-sp SPEED]" << endl;
    cerr << "som_demo clusters -i INPUT_CNT -s GRID_SIZE [-c CLUSTER_CNT] [-cs CLUSTER_SIZE] [-sp SPEED] [-nd]" << endl;
    cerr << "som_demo digits -f FILENAME -s GRID_SIZE [-p CLASSIFY_PERCENT]"  << endl;
    exit(1);
}

// simple int-to-str function for parsing numerical cli args, no exceptions needed, just print usage of error
int toInt(string s) {
    int n = 0;

    for(auto c: s) {
        if (c >= '0' && c <= '9') {
            n *= 10;
            n += c - 48;
        } else {
            usage();
        }
    }
    
    return n;
}

// output is tailored to the display.py script
// 's' - grid size follows
// 'e' - end of data
// 'i' - input coordinates follows
// otherwise - neuron coordinates follow
void demo_visual(int inputCnt, int gridSize) {
    
    intMatrix inputVec = generateInput(inputCnt, gridSize);
    cSom mySom = cSom(gridSize, gridSize, 2);
    mySom.initGrid();
    
    cout << "s" << mySom.sx << endl;  // print size of the grid
    mySom.print(); // print grid before any alternations
    
    for (unsigned i = 0; i < inputVec.size(); i++) {
        int bmu = mySom.getBmu(inputVec[i]);
         mySom.adjustWeights(bmu, inputVec[i]);
         
         // print grid in each step and also the input point
         mySom.print(); 
         cout << "i" << inputVec[i][0] << " " << inputVec[i][1] << endl;
         usleep(sleep_time);
    }
    cout << "e" << endl; //end of data
}

// output is tailored to the display.py script - symbol meaning same as for demo_visual
void demo_clusters(int inputCnt, int gridSize, int clusterCnt, int clusterSize, bool displayProgress) {
    intMatrix inputVec = generateClusteredInput(inputCnt, gridSize, clusterCnt, clusterSize);
    cSom mySom = cSom(gridSize, gridSize, 2);
    mySom.initGrid();
    
    cout << "s" << mySom.sx << endl; // print size of the grid
    mySom.print(); // print the initial grid
    
    for (unsigned i = 0; i < inputVec.size(); i++) {
        int bmu = mySom.getBmu(inputVec[i]);
         mySom.adjustWeights(bmu, inputVec[i]);
         
         // print grid and input in each step
         // for many inputs the grid displaying can be slow and so can be surpressed
         if (displayProgress) {
            mySom.print();
            usleep(sleep_time);
         }
         cout << "i" << inputVec[i][0] << " " << inputVec[i][1] << endl;
    }
    
    // print the final grid
    if (!displayProgress) {
        mySom.print();
    }
    cout << "e"; // end of data
}


// learn to recognize digits on a training set, use the last part to test it
void demo_digits(string filename, int gridSize, int classifyPercent) {
    vector<int> reference;
    intMatrix data;
    
    readCSV(filename, data, reference);
    int classifyCnt = data.size() / 100 * classifyPercent; // how big a portion do we use for testing
    
    cSom mySom = cSom(gridSize, gridSize, 784);
    cout << "PROGRESS: SOM created" << endl;
    mySom.initRandom(30);
    cout << "PROGRESS: SOM initialized" << endl;
    
    mySom.learn(data, reference, 0, data.size() - classifyCnt);
    mySom.classify(data, reference, data.size() - classifyCnt, data.size());
    
    cout << "\nneuron map: " << endl;
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
    int clusterCnt = 3;
    int gridSize = -1;
    int clusterSize = 100;
    string filename = "";
    int classifyPercent = 5;
    int speed = 5;
    
     srand((unsigned) time(0));
    
     
    // argument parsing
    for (int i = 2; i < argc; i++) {
        string arg(argv[i]);
        
        if (arg == "-i" && i < argc - 1)
            inputCnt = toInt(argv[++i]);
        else if (arg == "-s" && i < argc - 1)
            gridSize = toInt(argv[++i]);
        else if (arg == "-cs" && i < argc - 1)
            clusterSize = toInt(argv[++i]);
        else if (arg == "-c" && i < argc - 1)
            clusterCnt = toInt(argv[++i]);
        else if (arg == "-nd")
            displayProgress = false;
        else if (arg == "-f" && i < argc - 1)
            filename = argv[++i];
        else if (arg == "-p" && i < argc - 1)
            classifyPercent = toInt(argv[++i]);
        else if (arg == "-sp" && i < argc - 1)
            speed = toInt(argv[++i]);
        else 
            usage();
    }
    
    if (speed < 1 || speed > 5){
        cerr << "speed must be between 1 and 5" << endl;
        return 1;
    } else {
        if (speed == 5)
            sleep_time = 0;
        else if (speed == 4)
            sleep_time = 100000;
        else if (speed == 3)
            sleep_time = 300000;
        else if (speed == 2)
            sleep_time = 1000000;
        else if (speed == 1)
            sleep_time = 5000000;
    }
        
    if (string(argv[1]) == "visual") {
        if (inputCnt < 1 || gridSize < 1)
            usage();
        if (gridSize < 3 || gridSize > 20) {
            cerr << "grid size must be between 3 and 20" << endl;
            return 1;
        }
        
        demo_visual(inputCnt, gridSize);
    } else if (string(argv[1]) == "clusters") {
        if (inputCnt < 1 || gridSize < 1)
            usage();
        
        if (gridSize < 3 || gridSize > 20) {
            cerr << "grid size must be between 3 and 20" << endl;
            return 1;
        }
        if (clusterSize < 10 || clusterSize > gridSize * 50) {
            cerr << "cluster size must be between 10 and gridSize * 50 (" << gridSize * 50 << ")" << endl;
            return 1;
        }
        
        demo_clusters(inputCnt, gridSize, clusterCnt, clusterSize, displayProgress);
    } else if (string(argv[1]) == "digits") {
        if (filename == "")
            usage();
        
        if (classifyPercent < 1 || classifyPercent > 50) {
            cerr << "classification percentage must be between 1 and 50" << endl;
            return 1;
        }
        
        if (gridSize < 5 || gridSize > 25) {
            cerr << "net size for digit classification must be between 5 and 25" << endl;
            return 1;
        }
        
        demo_digits(filename, gridSize, classifyPercent);
    } else {
        usage();
    }
    
    return 0;
}
