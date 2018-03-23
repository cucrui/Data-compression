import pylab as pl
import numpy as np

def loadData(filename):
    inFile = open(filename, 'r')

    x = []
    y = []

    for line in inFile:
        color_cnt = line.split(' ')
        x.append(color_cnt[0]) #get string type data
        y.append(color_cnt[1])
    return (x, y)


def plotData(x1, y1, x2, y2, x3, y3):
    pl.figure(1)

    pl.plot(x1, y1, color = 'red')
    pl.plot(x2, y2, color = 'green')
    pl.plot(x3, y3, color = 'blue')

    pl.show()

(x1, y1) = loadData("r_cnt.txt")
(x2, y2) = loadData("g_cnt.txt")
(x3, y3) = loadData("b_cnt.txt")
plotData(x1, y1, x2, y2, x3, y3)


