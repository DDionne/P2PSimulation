import numpy
import matplotlib.pyplot as plt

filein = open("Stats-Table.txt","r")

print "Choose an X-axis"
print "press 1 for time"
print "press 2 for Peers Reached"
print "press 3 for qhits"
print "press 4 for Average Component Size"
print "press 5 for the number of components in the network"
print "press 6 for the Network Diameter"
Xelement = input()
print "Choose a Y-axis"
print "press 1 for time"
print "press 2 for Peers Reached"
print "press 3 for qhits"
print "press 4 for Average Component Size"
print "press 5 for the number of components in the network"
print "press 6 for the Network Diameter"
Yelement = input()


print Xelement
print Yelement

Xelement = int(Xelement)
Yelement = int(Yelement)
plotTitle = ''
Xaxis = []
Yaxis = []
lineNumber = 0
for line in filein:
    currentLine = line.split()
    if lineNumber == 0:
        plotTitle += currentLine[Xelement]
        plotTitle += " vs. "
        plotTitle += currentLine[Yelement]
        
    else: 
        Xaxis.append(float(currentLine[Xelement]))
        Yaxis.append(float(currentLine[Yelement]))
    lineNumber+=1
plt.plot(Xaxis, Yaxis,marker='.', linestyle = 'None')
plt.title(plotTitle)
plt.show()
