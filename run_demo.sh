#!/bin/bash

# module: run_demo.sh - launcher of som_demo.sh 
# author: Jan Hammer, xhamme00

if [ $# != 1 ]; then
    echo "usage: ./run_demo 1|2|3|4"
elif [ $1 = "1" ]; then
    stat display.py graphics.py > /dev/null || {
        echo "files display.py and graphics.py must be present for this demo to work"
        exit
    }
    make && ./som_demo visual -i 10 -s 6 -sp 2 | ./display.py
elif [ $1 = "2" ]; then
    stat display.py graphics.py > /dev/null || {
        echo "files display.py and graphics.py must be present for this demo to work"
        exit
    }
    make && ./som_demo clusters -i 200 -c 3 -s 10 -cs 200 | ./display.py
elif [ $1 = "3" ]; then
    stat display.py graphics.py > /dev/null || {
        echo "files display.py and graphics.py must be present for this demo to work"
        exit
    }
    make && ./som_demo clusters -i 5000 -c 5 -nd -s 10 -cs 300 | ./display.py 
elif [ $1 = "4" ]; then
    stat train.csv > /dev/null || {
        echo "file train.csv must be present for this demo to work"
        exit
    }
    make && ./som_demo digits -f train.csv -p 2 -s 15
else
    echo "usage: ./run_demo 1|2|3|4"
fi
