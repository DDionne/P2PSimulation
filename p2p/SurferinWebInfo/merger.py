f = open('Simu1Frequency.txt')
p = f.readlines()
document = []
document2 = []
document3 = []
for line in p:
    document.append(line)
f.close()
f3 = open('LinkProbability.txt')
file2 = f3.readlines()
for i in file2:
    document2.append(i)
f3.close()
f2 = open('Merged.txt','w')
k = 0
for i in document:
    document3.append(document[k][:-1] + "   " + document2[k])
    k += 1
for i in document3:
    f2.write(i)
f2.close()




