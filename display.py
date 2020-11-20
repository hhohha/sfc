#!/usr/bin/env python3

import numpy as np
import sys
import numpy.matlib, time, random, sys
from somlib import *

def readNextLine():
    line = sys.stdin.readline()
    while not line:
        time.sleep(0.1)
        line = sys.stdin.readline()
        
    return line


init_draw(550, 550)
input_vec = []

while True:
    line = readNextLine()
    sx, sy = map(lambda x: int(x), line.split('x'))
    m = []
    
    if sx == 0 and sy == 0:
        break
    
    line = readNextLine()
    c1, c2 = map(lambda x: int(x), line.split())
    input_vec.append([c1, c2])
    
    for i in range(sx * sy):
        line = readNextLine()
        c1, c2 = map(lambda x: int(x), line.strip().split())
        m.append([c1, c2])
        
    draw_2d_map(m, 6, input_vec)
 
finish_draw()
