#!/bin/bash

if [ $# != 1 ]; then
    echo "usage: ./run_demo 1|2|3|4"
elif [ $1 = "1" ]; then
    stat display.py graphics.py > /dev/null || {
        echo "files display.py and graphics.py must be present for this demo to work"
        exit
    }
    make && ./som_demo visual -i 10 | ./display.py 6
elif [ $1 = "2" ]; then
    stat display.py graphics.py > /dev/null || {
        echo "files display.py and graphics.py must be present for this demo to work"
        exit
    }
    make && ./som_demo clusters -i 200 -c 3 | ./display.py 10
elif [ $1 = "3" ]; then
    stat display.py graphics.py > /dev/null || {
        echo "files display.py and graphics.py must be present for this demo to work"
        exit
    }
    make && ./som_demo clusters -i 5000 -c 5 -nd | ./display.py 10    
elif [ $1 = "4" ]; then
    stat train.csv > /dev/null || {
        echo "file train.csv must be present for this demo to work"
        exit
    }
    make && ./som_demo digits -f train.csv
else
    echo "usage: ./run_demo 1|2|3|4"
fi
