import numpy
import matplotlib.pyplot as plt

filein = open("Stats-Table.txt","r")

print "Choose an X-axis"
print "press 1 for Time"
print "press 2 for Peers Reached"
print "press 3 for QueryHits"
print "press 4 for Average Component Size"
print "press 5 for The Number of Components in the Network"
print "press 6 for The Network Diameter"
Xelement = input()
print "Choose a Y-axis"
print "press 1 for Time"
print "press 2 for Peers Reached"
print "press 3 for QueryHits"
print "press 4 for Average Component Size"
print "press 5 for The Number of Components in the Network"
print "press 6 for The Network Diameter"
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
        plotTitle += currentLine[Yelement]
        plotTitle += " vs. "
        plotTitle += currentLine[Xelement]
        
    else: 
        Xaxis.append(float(currentLine[Xelement]))
        Yaxis.append(float(currentLine[Yelement]))
    lineNumber+=1
plt.plot(Xaxis, Yaxis,marker='.', linestyle = 'None')
plt.title(plotTitle)
plt.show()
