Simple Self-Organizing Maps (SOM) implementation

Running demo 1,2 and 3 requires the graphics library available at https://mcsp.wartburg.edu/zelle/python/graphics.py
Running demo 4 requires the train.csv data set available at: https://www.kaggle.com/c/digit-recognizer/data?select=train.csv

Running the demo script: ./run_demo.sh 1 | 2 | 3 | 4

Compilation: make

Usage:
som_demo visual -i INPUT_CNT -s GRID_SIZE [-sp SPEED]
som_demo clusters -i INPUT_CNT -s GRID_SIZE [-c CLUSTER_CNT] [-cs CLUSTER_SIZE] [-sp SPEED] [-nd]
som_demo digits -f FILENAME -s GRID_SIZE [-p CLASSIFY_PERCENT]

There are three modes
 - visual: displays 2D net, generates random points
 - clusters: similarly to visual, but inputs are generated in clusters
 - digits: demonstrates detection of digits
 
-i: number of randomly generated inputs
-c: number of clusters
-nd: don't visualize during the training
-f: name of input file
-s: size of the net
-sp: speed of input generation
-cs: size of the clusters
-p: percenta of input data used for classification (not for training)

Examples of usage:
./som_demo visual -i 10 -s 6 -sp 2 | ./display.py
./som_demo clusters -i 200 -c 3 -s 10 -cs 200 | ./display.py
./som_demo clusters -i 5000 -c 5 -nd -s 10 -cs 300 | ./display.py 
./som_demo digits -f train.csv -p 2 -s 12
