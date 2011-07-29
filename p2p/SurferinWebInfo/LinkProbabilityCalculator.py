#!/usr/bin/env python

from pageRank import *

def LinkProbability(fileIn,fileOut):
    '''
reads files of the format:
  194: 189 193 197 212         first number is document, other are documents
  701: 699 700                 it links to
  195: 236 264 265 295 311
  196: 236 238 253 271 274 295
  197: 171 186 189 193 194 217
  198: 187 220 229 244
  199: 200 212
  200: 204 208
  201: 200 217


Reads from file and calculates the pagerank of each document. This function takes
two arguments, fileIn and fileOut. fileIn is the file that is going to be read
while fileOut is the output file for the results. They should both be strings.
'''
    
    f = open(fileIn)
    fileout = open(fileOut,'w')
    file = f.readlines()
    linklist = []
    dictionnary = {}
    for j in range(965):
        linklist.append([])

    for line in file:
        templist = line.split()
        doc = int(templist[0][:-1])
        docindex = doc-1
        i = 1
        while(i < len(templist)):
            linklist[docindex].append(int(templist[i])-1)

            i = i + 1

    probabilities = pageRank(linklist, alpha = 0.85)
    document = 1
    for k in probabilities:

        dictionnary[document] = k
        document += 1
    
    #fileout.write('Document     Probability\n')
    #sorting dictionnay by value
    #following taken from "http://www.saltycrane.com/blog/2007/09/how-to-sort-python-dictionary-by-keys/"
    for key, value in sorted(dictionnary.iteritems(), key=lambda (k,v): (v,k)):

        #fileout.write("   " + str(key) + "        " + str(value) +"\n")
        fileout.write(str(key) +"     " + str(value) + "\n")
    f.close()
    fileout.close()




def LinkCounter(fileIn,fileOut):
    '''
reads files of the format:
  ex:
  01:16:34:068 online 31
  01:16:35:068 publish 358000
  01:16:45:068 publish 546000
  01:16:55:068 publish 490000
  01:17:05:068 publish 513000
  01:17:15:068 publish 472000
  01:17:25:068 publish 470000
  01:17:35:068 publish 429000
  01:17:45:068 publish 588000

reads from Input file and outputs all the times a document has been reached
in order from least frequent to most. Both arguments should be string. fileIn
is the file being read and fileOut is the output file.
'''
    f = open(fileIn)
    fileout = open(fileOut,'w')
    file = f.readlines()
    documenttotallist = []
    documentlist = set()
    dictionnary = {}
    frank = {}
    for line in file:
        if("publish" in line):
            templist = line.split()
            tempdoc = templist[2][:-3]                
            documenttotallist.append(tempdoc)
            #if tempdoc not in documentlist:
            documentlist.add(tempdoc)
    totalpublishes = 0
    for i in documentlist:
        counter = documenttotallist.count(i)
        dictionnary[i] = counter
        totalpublishes += counter
    print(totalpublishes)
    #sorting dictionnay by value
    #following taken from "http://www.saltycrane.com/blog/2007/09/how-to-sort-python-dictionary-by-keys/"
    for key, value in sorted(dictionnary.iteritems(), key=lambda (k,v): (v,k)):
    
        #fileout.write(str(key) + "   " + str(value)+ "\n")
        fileout.write(str(value) + "    " + str(key) + "\n")
    f.close()
    fileout.close()

LinkCounter('Simu500h.txt','Simu1Frequency.txt')
LinkProbability('links-sample-1k.txt','LinkProbability.txt') 


    



        
              
