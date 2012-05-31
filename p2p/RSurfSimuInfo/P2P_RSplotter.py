import numpy
import matplotlib.pyplot as plt


default_input = 'C:\\eclipse\\workspace'
# root_dir will be set to my_input if they just press enter
root_dir = raw_input("Enter your workspace directory(default: %s):\n" % default_input) or default_input
filein = open(root_dir + "\\P2PSimulation\\p2p\\p2p-simulation\\coupled\\RSurfSimu\\fileout.txt", "r")

#filein = open("fileout.txt", "r")
fileout = open("DocInfo.txt", "w")
doclist = []
templist = []

lineCounter = 1

""" Fill in the lists with all the documents available, those that are never
actually published in the simulation will simply have a frequency of 0, just like
the documents that get published, then removed"""
for i in range(1,966):
    doclist.append(str(i))
    a = (str(i), 0)
    templist.append(a)


""" get data from the file """
for line in filein:


    """ Skip the first bunch of lines (they aren't actually part of the simulation """
    if lineCounter > 21 and "Simulation" not in line:
        #print("starting + lineCounter: " + str(lineCounter))
        #if("Simulation" not in line):
        #    print("true")
        currentLine = line.split()
        i = 0

        #


        """ increase the published documents frequency in the list by 1 """
        if "publish" in currentLine[1]:
            p = currentLine[2][:-8]
            if(len(p) != 0):
                i = -4
                docs = ""
                while i < 0:
                    docs += currentLine[2][i]
                    i += 1
                doc = docs.lstrip('0')
            else:
                
                doc = currentLine[2]
            #print(doc)


            #if doc not in doclist:
             #   doclist.append(doc)
             #   a = (doc, 1)
            #    templist.append(a)
           # else:

            i = doclist.index(doc)
            b = templist[i]
            c = (b[0], b[1] + 1)
            templist[i] = c

        """ Decrease the frequency of the document by 1 """
        if "remove" in currentLine[1]:
            p = currentLine[2][:-8]
            if(len(p) != 0):
                i = -4
                docs = ""
                while i < 0:
                    docs += currentLine[2][i]
                    i += 1
                doc = docs.lstrip('0')
            else:
                doc = currentLine[2]

            
            
            i = doclist.index(doc)
            b = templist[i]
            c = (b[0], b[1] - 1)
            templist[i] = c
    lineCounter += 1;

#for i in templist:
   # print(" (" + i[0] + ", " + str(i[1]) + ")")



""" Get the link probabilities and rebuild templist to accomodate this new set of data
 Also place them with their corresponding documents """
filein.close()
f = open("LinkProbability.txt")

for line in f:
    currentLine = line.split()
    if(currentLine[1] in doclist):
        i = doclist.index(currentLine[1])
        a = templist[i]
        b = (a[0], a[1], currentLine[0])
        templist[i] = b

f.close()



""" Make a new list which is the same as templist, but sorted by frequency """
other = sorted(templist, key = lambda a: a[1])


""" Write to a file, to save the results """
#print("sorting...")
for i in other:
    #print(i)
    fileout.write(str(i[0]) + "  " + str(i[1]) + "  " + str(i[2]) + "\n")
    


""" Plot the graph using the frequency as the Y-axis and the PageRank as the X-axis """     
Yaxis = []
Xaxis = []
for i in other:
    Yaxis.append(i[1])
    Xaxis.append(i[2])
plt.plot(Xaxis, Yaxis,marker='.', linestyle = 'None')
plt.title('Document Count vs. PageRank')
plt.show()

fileout.close()




                
            
