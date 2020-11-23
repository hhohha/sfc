#!/usr/bin/env python3

# module: display.py - simple displayer of som_demo output
# author: Jan Hammer, xhamme00

from graphics import *
import sys

win = None
draw_objs = []

def init_draw(x, y):
    global win
    win = GraphWin('', x, y)

# draw one input point
def draw_input(x, y):
    global win
    
    c = Circle(Point(x+25, y+25), 2)
    c.setOutline('red')
    c.setFill('red')
    c.draw(win)
    
# draw the entire net
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

# read line by line output from som_demo
# 's' - grid size follows
# 'e' - end of data
# 'i' - input coordinates follows
# otherwise - neuron coordinates follow
while True:
    line = readNextLine()
    
    if line[0] == 's':
        # grid size received (should happen only once)
        sx = sy = int(line[1:])
        init_draw(sx * 100 - 50, sy * 100 - 50)
    elif line[0] == 'i':
        # input point received
        c1, c2 = map(lambda x: int(x), line[1:].split())
        draw_input(c1, c2)
    elif line[0] == 'e':
        # end of data stream
        break
    else:
        # one neuron coordinates received
        c1, c2 = map(lambda x: int(x), line.strip().split())
        m.append([c1+25, c2+25])
        
    # draw the whole net only when we have all points    
    if len(m) == sx * sy:
        draw_net(m, sx)
        m = []
 
finish_draw()
