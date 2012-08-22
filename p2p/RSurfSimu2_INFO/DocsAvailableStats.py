


default_input = 'C:\\eclipse\\workspace'
# root_dir will be set to my_input if they just press enter
root_dir = raw_input("Enter your workspace directory(default: %s):\n" % default_input) or default_input
filein = open("LogEventsTemp.txt", "r")
fileout = open("AvailableDocs.txt", "w")
docsAvailable = 0

"""
DocFreq and DocList are built simultaneously, therefore the docFreq at index "x"
corresponds to the docId in the docList at index "x"
"""
docFreq = [] #list of document frequencies
docList = [] #list of document ID's 


"""   Iterate through the file one line at a time   """
for line in filein:
    currentLine = line.split(':') #split the line so that the time, the action and the document can be easily retrieved
    if len(currentLine) == 4: #publish and remove only have 4 fields (time, action, peer, docId)
        if currentLine[1] == "publish":
            document = int(currentLine[3])
            docsAvailable += 1 #increase the amount of documents available by 1, since we are adding one
            if document in docList:
                docFreq[docList.index(document)] += 1 #document already in the list, increase freq by 1
            else:
                docList.append(document) #add document to the list of Document IDs
                docFreq.append(1) #add a 1 into docFreq to indicate that there is one present in the network
        if currentLine[1] == "remove":
            document = int(currentLine[3])
            index = docList.index(document)
            docsAvailable -= 1 #since a document was removed, decrease the amount by 1
            docFreq[index]-=1 #decrease the documents freq by 1
            if docFreq[index] == 0: #if the document reaches 0 frequenc, remove it from the list
                docFreq.pop(index)
                docList.remove(document)
for i in range(len(docFreq)):
    print "document: "+str(docList[i]) + " - Freq: " + str(docFreq[i])
    fileout.write("document: " + str(docList[i]) + "   Freq: " + str(docFreq[i])+"\n")


print("Total Document Available: " + str(docsAvailable))
            
