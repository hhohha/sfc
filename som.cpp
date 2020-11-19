#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <unistd.h>

using namespace std;

typedef vector<vector<int>> tInput2d;

class cNeuron {
public:
    vector<int> w;
    int size;
    vector<int> hits{0,0,0,0,0,0,0,0,0,0};
    
    cNeuron(int size) {
        w.resize(size);
        this->size = size;
    }
    
    void initRandom(int maxn) {
        for (unsigned i = 0; i < w.size(); i++) 
            w[i] = rand() % maxn;
    }

    int diffVec(vector<int> v) {
        if (v.size() != w.size())
            throw "cNeuron > diffVec: wrong vector dimension";
        
        int sum = 0;
        for (unsigned i = 0; i < v.size(); i++) {
            sum += pow(v[i] - w[i], 2);
        }
        
        return sum;
    }
    
    void addVec(vector<int> v, double coeff) {
        if (v.size() != w.size())
            throw "cNeuron > addVec: wrong vector dimension";
        
        for (unsigned i = 0; i < v.size(); i++) {
            w[i] += round((v[i] - w[i]) * coeff);
        }
    }
    
    int topHit() {
        int maxIdx = -1;
        int maxHits = 0;
        for (unsigned i = 0; i < hits.size(); i++) {
            if (hits[i] > maxHits) {
                maxHits = hits[i];
                maxIdx = i;
            }
        }
        
        return maxIdx;
    }
    
    void hit(int i) {
        hits[i]++;
    }
};

ostream &operator<<(ostream &output, const cNeuron& n) {
    unsigned i = 0;
    for (i = 0; i < n.w.size() - 1; i++) 
        output << n.w[i] << '-';
    if (n.w.size() > 0)
        output << n.w[i];
    return output;
}

class cSom {
public:
    vector<cNeuron> som;
    unsigned sx, sy, dimension;
    double const learnCoeff = 0.1;
    double const nbrCoeff = 0.75;
    
    cSom(unsigned sx, unsigned sy, unsigned dimension) {
        this->sx = sx;
        this->sy = sy;
        this->dimension = dimension;
        
        for (unsigned i = 0; i < sx * sy; i++){
            som.push_back(cNeuron(dimension));
        } 
    }
    
    void debugPrint() {
        for (unsigned i = 0; i < sx; i++) {
            for (unsigned j = 0; j < sy; j++) {
                cout << som[sx * i + j] << "   ";
            }
            cout << endl;
        }
    }
    
    void initGrid() {
        for (unsigned i = 0; i < sx; i++)
            for (unsigned j = 0; j < sy; j++) {
                som[i * sx + j].w[0] = i * 100;
                som[i * sx + j].w[1] = j * 100;
            }
    }
    
    void initRandom(unsigned maxn) {
        for (unsigned i = 0; i < sx; i++)
            for (unsigned j = 0; j < sy; j++)
                som[i * sx + j].initRandom(maxn);
    }
    
    vector<int> getNeighbors(unsigned n) {
        vector<int> retVec;
        if (n >= sx)
            retVec.push_back(n - sx);
        if (n < sx * sy - sx)
            retVec.push_back(n + sx);
        if ((n + 1) % sx > 0)
            retVec.push_back(n + 1);
        if (n % sx > 0)
            retVec.push_back(n - 1);
            
        return retVec;
    }

    void learn(vector<vector<int>> input, vector<int> ref) {
        cout << "learning started" << endl;
        //print({-1, -1});
        int cntr = 0;
        for (unsigned i = 0; i < input.size() - 1000; i++) {

            if (cntr == 1000) {
                cout << "parsed " << i << " items\n";
                cout << flush;
                cntr = 0;
            }
            cntr++;
            //usleep(1000000);
            
            int bmu = getBmu(input[i]);
            
            som[bmu].addVec(input[i], learnCoeff);
            som[bmu].hit(ref[i]);
            for(int j: getNeighbors(bmu)) {
                som[j].addVec(input[i], learnCoeff * nbrCoeff);
            }
            
            //print(input[i]);
        }
        cout << "learning finished" << endl;
    }
    
    void classify(vector<vector<int>> input, vector<int> ref) {
        cout << "classification started" << endl;
        int correct = 0;
        int wrong = 0;
        
        for (unsigned i = 41000; i < input.size(); i++) {
            int bmu = getBmu(input[i]);
            if (som[bmu].topHit() == ref[i]) {
                correct++;
            } else {
                wrong++;
            }
        }
        
        cout << "classification ended" << endl;
        cout << "correctly classified: " << correct << " items out of " << correct + wrong << endl;
        cout << "wrongly classified: " << wrong << " items out of " << correct + wrong << endl;
    }
    
    void print(vector<int> input) {
        cout << sx << "x" << sy << endl;
        cout << input[0] << " " << input[1] << endl;
        for (unsigned i = 0; i < som.size(); i++) {
            for (auto n: som[i].w) {
                cout << n << " ";
            }
            cout << endl;
        }
    }
    
    int getBmu(vector<int> vec) {
        int minval = 0;
        int bestNeuronIdx = -1;
        
        for (unsigned i = 0; i < som.size(); i++) {
            int tmp = som[i].diffVec(vec);
            if (tmp < minval || bestNeuronIdx == -1) {
                minval = tmp;
                bestNeuronIdx = i;
            }
        }
        
        return bestNeuronIdx;
    }
    
    cNeuron getNeuron(unsigned x, unsigned y) {
        unsigned i = sx * x + y;
        
        if (i >= som.size()) {
            cout << "sx = " << sx << ", sy = " << sy << ",   x = " << x << ", y = " << y << endl;
            throw "cSom > getNeuron: attemted to get neuron with bad index";
        }
        return som[i];
    }
};


tInput2d generate_input(){
    tInput2d vin;
    
    for (int i = 0; i < 20; i++) {
        vin.push_back(vector<int>{rand() % 600, rand() % 600});
    }
    
    return vin;
}

// tInput2d generate_input2(){
//     tInput2d vin;
//     
//     vector<int> v = {205,390};
//     vin.push_back(v);
//     
//     return vin;
// }


int main(){
    vector<int> reference;
    vector<vector<int>> data;
    
    ifstream fin("train.csv");
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
    
    cout << "\nREADING INPUT DONE" << endl;
    srand((unsigned) time(0));
    
    
//     tInput2d testInput = generate_input();
    
    cSom mySom = cSom(15, 15, 784);
    cout << "cSom created" << endl;
    mySom.initRandom(10);
    cout << "cSom initialized" << endl;
    
    mySom.learn(data, reference);
    mySom.classify(data, reference);
    
    for (unsigned i = 0; i < mySom.sx; i++) { 
        for (unsigned j = 0; j < mySom.sy; j++) {
            cout << mySom.getNeuron(i, j).topHit() << " ";
            //cout << mySom.som[i*mySom.sx + j].topHit() << " ";
        }
        cout << endl;
    }
    
    
    
//     mySom.initGrid();
    //mySom.debugPrint();
    
//     mySom.learn(testInput);
//     cout << "0x0" << endl;
    
    //cout << "<<<" << mySom.som[1] << endl;
    
    
    
    return 0;
    
}
