import numpy
import matplotlib.pyplot as plt

filein = open("Stats-Table-netlogo.txt","r")

print "Choose an X-axis"
print "press 1 for Time"
print "press 2 for Peers Reached"
print "press 3 for QueryHits"
print "press 4 for Average Component Size"
print "press 5 for The Number of Components in the Network"
print "press 6 for The Network Diameter"
print "press 7 for the Number of Documents in the Network"
print "press 8 for the Number of Peers Online"
Xelement = input()
print "Choose a Y-axis"
print "press 1 for Time"
print "press 2 for Peers Reached"
print "press 3 for QueryHits"
print "press 4 for Average Component Size"
print "press 5 for The Number of Components in the Network"
print "press 6 for The Network Diameter"
print "press 7 for the Number of Documents in the Network"
print "press 8 for the Number of Peers Online"
Yelement = input()


Xelement = int(Xelement) #turn them into integers so I can use them as an index later
Yelement = int(Yelement)
plotTitle = ''
Xaxis = []
Yaxis = []
lineNumber = 0
for line in filein:
    currentLine = line.split()
    if lineNumber == 0:
        if Yelement != 9:
            plotTitle += currentLine[Yelement]
        else:
            plotTitle += "Qhit/PeersReached ratio"
        plotTitle += " vs. "
        plotTitle += currentLine[Xelement]
    elif Yelement == 9:
        Xaxis.append(float(currentLine[1]))
        if float(currentLine[2]) == 0:
            Yaxis.append(0)
        else:
            Yaxis.append(float(currentLine[3])/float(currentLine[2]))
        
    else: 
        Xaxis.append(float(currentLine[Xelement]))
        Yaxis.append(float(currentLine[Yelement]))
    lineNumber+=1
plt.plot(Xaxis, Yaxis,marker='.', linestyle = 'None')
plt.title(plotTitle)
plt.show()
