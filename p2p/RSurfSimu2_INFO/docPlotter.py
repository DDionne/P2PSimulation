import numpy
import matplotlib.pyplot as plt

filein = open("AvailableDocs.txt", "r")
fi = open("AvailableDocs2.txt", "r")
Xaxis = []
for i in range(966):
    Xaxis.append(i)

Yaxis1 = []
Yaxis1Docs = 0
Yaxis2 = []
Yaxis2Docs = 0

for i in range(966):
    Yaxis1.append(0)
    Yaxis2.append(0)

for line in filein:
    currentline = line.split()
    doc = int(currentline[1])
    freq = int(currentline[3])
    Yaxis1[doc] = freq
    Yaxis1Docs += freq

for line in fi:
    currentline = line.split()
    doc = int(currentline[1])
    freq = int(currentline[3])
    Yaxis2[doc] = freq
    Yaxis2Docs += freq

print "RSurfSimu2 = Red dots"
print "total amount of docs for the modified simulation: " + str(Yaxis1Docs)

print "RSurfSimu = green +"
print "total amount of docs for the original simulation: " + str(Yaxis2Docs)

plt.plot(Xaxis,Yaxis1, color='r', marker='.',linestyle='None')
plt.plot(Xaxis,Yaxis2, color='g', marker='+',linestyle='None')
plt.show()
