"""
Takes a text file with a text format like this example:

00:00:10:23 publish 300100020


This file is taken from the randomSurfer p2p network simulation. However lines not of the proper format must be removed
along with any empty lines (there might be one at the end).
"""
default_input = 'C:\\eclipse\\workspace'
# root_dir will be set to my_input if they just press enter
root_dir = raw_input("Enter your workspace directory(default: %s):\n" % default_input) or default_input
fi = open(root_dir + "\\P2PSimulation\\p2p\\p2p-simulation\\coupled\\RSurfSimu\\fileout.txt", "r")
#fi = open("fileout.txt","r")
fileout = open("logEventsTemp.txt","w")
f = fi.readlines()
numLines = 0
for line in f:
    if(("online" in line) or ("offline" in line) or ("publish" in line) or ("remove" in line) or ("connect" in line) or ("query" in line) or ("queryhit" in line) or ("queryreachespeer" in line)):
        time = 0
        currentLine = line.split()
        
        timeList = currentLine[0].split(':')
        hoursToMS = int(timeList[0]) * 3600000
        minToMS = int(timeList[1]) * 60000
        secToMS = int(timeList[2]) * 1000
        time = hoursToMS + minToMS + secToMS + int(timeList[3])



        if (currentLine[1] == "online"):
            fileout.write("\n" + str(time) + ":" + currentLine[1] + ":" + currentLine[2])
           # print(str(time) + ":" + currentLine[1] + ":" + currentLine[2])
            numLines += 1




        elif (currentLine[1] == "offline"):
            fileout.write("\n" + str(time) + ":" + currentLine[1] + ":" + currentLine[2])
           # print(str(time) + ":" + currentLine[1] + ":" + currentLine[2])
            numLines += 1





        elif ("connect" in currentLine[1]):

            temp = currentLine[2][:-10]
            fromPeer = temp[:-2]
            if fromPeer != "":
                toFinal = int(temp) - (int(fromPeer)*100)
            else:
                toFinal = temp
                fromPeer = "0"
            fileout.write("\n" + str(time) + ":" + currentLine[1] + ":" + fromPeer + ":" + str(toFinal))
           # print(str(time) + ":" + currentLine[1] + ":" + fromPeer + ":" + str(toFinal))
            numLines += 1





        elif (currentLine[1] == "publish"):
            #print line
            peer = currentLine[2][:-10]
            if peer == "":
                peer = "0"
                doc = str(currentLine[2])
            else:
                doc = str(int(currentLine[2]) - (int(currentLine[2][:-4])*10000))
            if doc == "":
                doc = "0"

            fileout.write("\n" + str(time) + ":" + currentLine[1] + ":" + peer + ":" + doc)
           # print(str(time) + ":" + currentLine[1] + ":" + peer + ":" + doc)
            numLines += 1




        elif (currentLine[1] == "remove"):
            peer = currentLine[2][:-10]
            if peer == "":
                peer = "0"
                doc = currentLine[2]
            else:
                doc = str(int(currentLine[2]) - (int(currentLine[2][:-4])*10000))
            if doc == "":
                doc = "0"

            fileout.write("\n" + str(time) + ":" + currentLine[1] + ":" + peer + ":" + doc)
           # print(str(time) + ":" + currentLine[1] + ":" + peer + ":" + doc)
            numLines += 1






        elif (currentLine[1] == "queryhit"):
            peer = currentLine[2][:-12]
            if peer == "":
                peer = "0"
            temp = currentLine[2][:-5]
            if temp == "":
                msg = "0"
            else:
                msg = str(int(temp)-(int(peer)*10000000))
            doc = str(int(currentLine[2]) - (int(currentLine[2][:-4])*10000))
            if doc == "":
                doc = "0"
            fileout.write("\n" + str(time) + ":" + currentLine[1] + ":" + peer + ":" + doc + ":" + msg)
            #print(str(time) + ":" + currentLine[1] + ":" + peer + ":" + doc + ":" + msg)
            numLines += 1




            
        elif (currentLine[1] == "queryreachespeer"):
            
            #print line
            TTL = currentLine[2][:-14]
            if TTL == "":
                TTL = "0"
            peer = str(int(currentLine[2][:-10]) - int(TTL)*10000)
            if peer == "":
                peer = "0"
            msg = str((int(currentLine[2][:-5])) - (int(TTL)*10000 + int(peer))*100000)
            if msg == "":
                msg = "0"
            #print msg
            




                
            fileout.write("\n" + str(time) + ":" + currentLine[1] + ":" + peer + ":" + msg)
           # print(str(time) + ":" + currentLine[1] + ":" + peer + ":" + msg)
            numLines += 1




            
        elif currentLine[1] == "query":
            TTL = currentLine[2][:-14]
            if TTL == "":
                TTL = "0"
            peer = str(int(currentLine[2][:-10]) - int(TTL)*10000)
            if peer == "":
                peer = "0"
            msg = str((int(currentLine[2][:-5])) - (int(TTL)*10000 + int(peer))*100000)
            if msg == "":
                msg = "0"
            temp = currentLine[2][:-4]
            doc = str(int(currentLine[2]) - int(temp)*10000)
            if doc == "":
                doc = "0"





                
            fileout.write("\n" + str(time) + ":" + currentLine[1] + ":" + peer + ":" + doc + ":" + msg)
            #print(str(time) + ":" + currentLine[1] + ":" + peer + ":" + doc + ":" + msg)
            numLines += 1

fi.close()
fileout.close()
f = open("logEventsTemp.txt","r")
fileout = open("logEvents.txt", "w")
fileout.write(str(numLines))
for line in f:
    fileout.write(line)
f.close()
fileout.close()
print("Done converting the output file to the correct LogEvents format")
