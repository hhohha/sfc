#!/bin/bash

if [ $# != 1 ]; then
    echo "usage: ./run_demo 1|2|3|4"
elif [ $1 = "1" ]; then
    stat display.py graphics.py > /dev/null || {
        echo "files display.py and graphics.py must be present for this demo to work"
        exit
    }
    make && ./demo_visual 10 | ./display.py 6
elif [ $1 = "2" ]; then
    stat display.py graphics.py > /dev/null || {
        echo "files display.py and graphics.py must be present for this demo to work"
        exit
    }
    make && ./demo_visual_clusters 200 3 -p | ./display.py 10
elif [ $1 = "3" ]; then
    stat display.py graphics.py > /dev/null || {
        echo "files display.py and graphics.py must be present for this demo to work"
        exit
    }
    make && ./demo_visual_clusters 5000 5 -np | ./display.py 10    
elif [ $1 = "4" ]; then
    stat train.csv > /dev/null || {
        echo "file train.csv must be present for this demo to work"
        exit
    }
    make && ./demo_digits train.csv
else
    echo "usage: ./run_demo 1|2|3|4"
fi
