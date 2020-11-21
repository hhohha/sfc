#!/usr/bin/env python3

from graphics import *
import time, sys

win = None
draw_objs = []

def init_draw(x=600, y=600):
    global win
    win = GraphWin('', x, y)


def draw_2d_map(m, x, vec):
    global draw_objs
    global win

    for obj in draw_objs:
        obj.undraw()
    draw_objs = []

    for point in m:
        c = Circle(Point(point[0], point[1]), 5)
        c.setFill('black')
        draw_objs.append(c)
        c.draw(win)

    for i in range(len(m)):
        if (i + 1) % x != 0:
            l = Line(Point(m[i][0], m[i][1]), Point(m[i+1][0], m[i+1][1]))
            draw_objs.append(l)
            l.draw(win)
        if (i + x) < len(m):
            l = Line(Point(m[i][0], m[i][1]), Point(m[i+x][0], m[i+x][1]))
            draw_objs.append(l)
            l.draw(win)

    for v in vec:
        c = Circle(Point(v[0], v[1]), 2)
        c.setOutline('red')
        c.setFill('red')
        draw_objs.append(c)
        c.draw(win)


def finish_draw():
    win.getMouse()
    win.close()
    
gridSize = int(sys.argv[1])

def readNextLine():
    line = sys.stdin.readline()
    while not line:
        time.sleep(0.1)
        line = sys.stdin.readline()
        
    return line

init_draw(gridSize * 100, gridSize * 100)
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
        
    draw_2d_map(m, gridSize, input_vec)
 
finish_draw()
