#!/usr/bin/env python3

from graphics import *
import time, sys

win = None
draw_objs = []

def init_draw(x, y):
    global win
    win = GraphWin('', x, y)



def draw_input(x, y):
    global win
    
    c = Circle(Point(x+25, y+25), 2)
    c.setOutline('red')
    c.setFill('red')
    c.draw(win)
    
def draw_net(m, x):
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

def finish_draw():
    win.getMouse()
    win.close()

def readNextLine():
    line = sys.stdin.readline()
    while not line:
        line = sys.stdin.readline()
        
    return line

sx, sy = 0, 0
m = []

while True:
    line = readNextLine()
    
    if line[0] == 's':
        sx = sy = int(line[1:])
        init_draw(sx * 100 - 50, sy * 100 - 50)
    elif line[0] == 'i':
        c1, c2 = map(lambda x: int(x), line[1:].split())
        draw_input(c1, c2)
    elif line[0] == 'e':
        break
    else:
        c1, c2 = map(lambda x: int(x), line.strip().split())
        m.append([c1+25, c2+25])
        
    if len(m) == sx * sy:
        draw_net(m, sx)
        m = []
 
finish_draw()
