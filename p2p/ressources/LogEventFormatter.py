


"""
Takes a text file with a text format like this example:

00:00:10:23 publish 300100020


This file is taken from the randomSurfer p2p network simulation. However lines not of the proper format must be removed
along with any empty lines (there might be one at the end).
"""
fi = open("fileout.txt.txt","r")
fileout = open("logEvents.txt","w")
f = fi.readlines()
for line in f:
    time = 0
    currentLine = line.split()
    timeList = currentLine[0].split(':')
    hoursToMS = int(timeList[0]) * 3600000
    minToMS = int(timeList[1]) * 60000
    secToMS = int(timeList[2]) * 1000
    time = hoursToMS + minToMS + secToMS + int(timeList[3])
    if ("online" in line):
        fileout.write( str(time) + ":" + currentLine[1] + ":" + currentLine[2] + "\n")
    elif ("offline" in line):
        fileout.write( str(time) + ":" + currentLine[1] + ":" + currentLine[2] + "\n")
    elif ("connect" in line):
        toInitial = currentLine[2][:-8]
        fromPeer = currentLine[2][:-11]
        if fromPeer != "":
            toFinal = int(toInitial) - (int(fromPeer)*1000)
        else:
            toFinal = toInitial
            fromPeer = "0"
        fileout.write( str(time) + ":" + currentLine[1] + ":" + fromPeer + ":" + str(toFinal) + "\n")
    elif ("publish" in line):
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
        if p == "":
            p = "0"
        fileout.write( str(time) + ":" + currentLine[1] + ":" + p + ":" + doc + "\n")
    elif ("remove" in line):
        peer = currentLine[2][:-8]
        if(len(peer) != 0):
            i = -3
            docId = ""
            while i < 0:
                docId += currentLine[2][i]
                i += 1
            doc = docId.lstrip('0')
        else:
            peer = "0"
            doc = currentLine[2]
        fileout.write( str(time) + ":" + currentLine[1] + ":" + peer + ":" + doc + "\n")
    elif (" queryhit " in line):
        peer = currentLine[2][:-8]
        if(len(peer) != 0):
            i = -8
            msgId = ""
            while i < -5:
                msgId += currentLine[2][i]
                i += 1
            msg = msgId.lstrip('0')
        if len(peer) != 0 or len(msgId) != 0:
            i = -4
            docs = ""
            while i < 0:
                docs += currentLine[2][i]
                i += 1
            doc = docs.lstrip('0')

        else: 
            doc = currentLine[2]
        if peer == "":
            peer = "0"
        if msg == "":
            msg = "0"
        fileout.write( str(time) + ":" + currentLine[1] + ":" + peer + ":" + doc + ":" + msg + "\n")
    elif (" queryreachespeer " in line):
        peer = currentLine[2][:-8]
        if(len(peer) != 0):
            i = -8
            msgId = ""
            while i < -5:
                msgId += currentLine[2][i]
                i += 1
            msg = msgId.lstrip('0')
            if(msg == ""):
                msg = "0"
        else:
            msg = currentLine[2][:-5]
        if peer == "":
            peer = "0"
        fileout.write( str(time) + ":" + currentLine[1] + ":" + peer + ":" + msg + "\n") 
    elif ("query" in line and "queryhit" not in line and "queryreachespeer" not in line):
        i = -4
        docs = ""
        while i < 0:
            docs += currentLine[2][i]
            i += 1
        doc = docs.lstrip('0')
        i = -11
        peer = ""
        while i < -8:
            peer += currentLine[2][i]
            i += 1
        p = peer.lstrip('0')
        i = -8
        msgId = ""
        while i < -5:
            msgId += currentLine[2][i]
            i += 1
        msg = msgId.lstrip('0')
        if(msg == ""):
            msg = "0"
        if(p == ""):
            p = "0"
        fileout.write( str(time) + ":" + currentLine[1] + ":" + p + ":" + doc + ":" + msg + "\n")
fi.close()
fileout.close()
            
            
            
        

    
