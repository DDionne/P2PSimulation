#script to generate a small-world graph, where the graph is implemented by a dict()

import numpy
import os
import shutil
import time

#generate social graph of acquaintances

print("Please input number of peers to simulate")
size = input()

print("Please input the storage capacity for each peer")
cap = input()

print("Please input the STANDARDTTL for each peer")
TTL = input()

print("Please input the number of documents available (e.g. from 1-966, put 965")
numDoc = input()



#generate social graph
G= dict()

G[0] = []
for i in range(1,size):
    #random decision for the parameters of the new nodes : how many existing nodes it contacts
    rrr = numpy.random.random()
    rr1=1 #1 primary contact
    if (rrr<0.05) and (len(G.keys()) >1): rr1 = 2 #or 2
    rr2 = numpy.random.poisson(1.6) #some secondary contacts
    available = list(G.keys())
    contact1 = numpy.random.randint(0,len(available)) #available is integers from 0 to i-1
    available.remove(contact1)
    G[i] = [contact1] #insert node i, connect to contact1
    G[contact1].append(i) #connect contact1 to i
    if (rr1==2):
        contact2 = available[numpy.random.randint(0,len(available))]
        G[i].append(contact2) #connect
        G[contact2].append(i)
    #print rr1," primary contact(s) added"
    #secondary contacts
    available = list(G[contact1]) #re-define available
    if (rr1==2): available = available.extend(G[contact2])
    if (available != None):
        setav = list(set(available)) #remove duplicates
        if(i in setav) : setav.remove(i)
        if(contact1 in setav) : setav.remove(contact1)
        if (rr1==2):
            if (contact2 in setav) : setav.remove(contact2)
        #print "available for secondary contacts :", setav
        num=0
        for j in range(1, rr2): # the number of secondary contacts
            #we now have only available nodes for secondary contact: maybe there are none left
            if (len(setav)==0): break
            contactj = numpy.random.randint(0,len(setav))
            G[i].append(setav[contactj]) #connect
            G[setav[contactj]].append(i)
            setav.remove(setav[contactj])
            num=j
        #print num," secondary contacts added"

    
print "Social acquaintances graph for the P2P network; acquaintances of each peer:"
for k in range(size):
    print k, ": ",G[k]
    
###output an adjacency matrix to a file
##fifi = open(dirname+"/socialgraph"+str(size)+".txt", "w")
##fifi.write(str(size))
##
##for i in range(size):
##    fifi.write("\n")
##    for j in range(size):
##        if j in G[i]:
##            fifi.write("1 "),
##        else:
##            fifi.write("0 "),
##        
##fifi.close()

#generating friends list for each peer
friendslist = []
for k in range(size):
    friends = ""
    for j in G[k]: # write out friends list for peer k
        friends = friends+","+str(j)
    friends = friends[1:] #remove initial comma
    friendslist.append(friends) #add to array


#generating querylist
#a random sequence of queries, number of queries follows poisson distribution of mean 6, queries uniformly taken from [0,9]
#note that queries may be repeated.
#update 23.10.2010 : the sequence of queries by each peer is repeated. This will give more chances for publishes to occur and for files to actually propagate
querylist = []
for k in range(size):
    rrr = numpy.random.randint(0,10,numpy.random.poisson(6)) 
    queries = list(rrr) # to list
    qstring = ""
    for j in queries: # write out friends list for peer k
        qstring = qstring+","+str(j)
    qstring = qstring[1:] #remove initial comma
    querylist.append(qstring) #add to array

#generating docs/k/.dat
#docs will be initially distributed between the peers
#so that each document is only stored by 1 or 2 peers.
DocDistrib = dict()
for d in range(1,numDoc+1): # <--------------------------- Modified this
    #they are stored by one - three of the peers (num of peers = size)
    nodes = list(numpy.random.randint(0,size,numpy.random.randint(1,4)))
    for nn in nodes:
        if (not (DocDistrib.has_key(nn))):
            DocDistrib[nn] = [d]
        else:
            DocDistrib[nn].append(d) #for each node storing the document, add the doc to its list of stored docs

#save the initial document distribution in a data structure.
docsdistribution =[]
for k in range(size):
    #docfile = open(dirname+"/docs"+str(k)+".dat", "w")
    doclist= ""
    if DocDistrib.has_key(k):
        #docfile.write( str(len(DocDistrib[k]))+"\n")
        for j in DocDistrib[k]:
            doclist=doclist+","+str(j)
        doclist= doclist[1:]
    else:
        doclist="-1" #"code" for a empty list
    docsdistribution.append(doclist)

#generating MultiPeer_size_.MA
default_input = 'C:\\eclipse\\workspace'
# user_input will be set to my_input if they just press enter
root_dir = raw_input("Enter your workspace directory(default: %s):\n" % default_input) or default_input
MAFile = open(root_dir + "\\P2PSimulation\\p2p\\p2p-simulation\\coupled\\RSurfSimu\\MultiPeer"+str(size)+".MA", "w")
#load a piece of the MA file from a separate file
partfile = open("RS_each_peer3.txt", "r")
eachpeerspec = ""
for line in partfile:
    eachpeerspec=eachpeerspec+line
partfile.close()
#load another piece
netspec =""
partfile = open("NetSpec_script.txt","r")
for line in partfile:
    netspec=netspec+line
partfile.close()

#list of parts making up the final MA file, with number of times each is repeated
parts = [(1,"[top]\ncomponents : Network IdGen@msgIdGen log@Logger Linker@LinkFinder"),
         (size," Peer{num}") ,
         (1,"\n"),
         (size, "Link : LinkFind@Peer{num} in_q@Linker\n"), #change
         (size, "Link : q_out{num}@Linker Link_in@Peer{num}\n"), #change
         (size, "Link : queryhit@Peer{num} qhit_in@log\n"),
         (size, "Link : disconnect@Peer{num} in_disconnect@log\n"),
         (size, "Link : connect@Peer{num} in_connect@log\n"),
         (size, "Link : out_{num}@IdGen in_query@log\n"),
         (size, "Link : publish@Peer{num} in_publish@log\n"),
         (size, "Link : online@Peer{num} in_online@Network\n"),
         (size, "Link : online@Peer{num} in_online@log\n"),
         (size, "Link : offline@Peer{num} in_offline@log\n"),
         (size, "Link : offline@Peer{num} in_offline@Network\n"),
         (size, "Link : connect_out@Peer{num} peer_connect@Network\n"),
         (size, "Link : disconnect_out@Peer{num} peer_disconnect@Network\n"),
         (size, "Link : c_out{num}@Network connect_in@Peer{num}\n"),
         (size, "Link : query_out@Peer{num} msg_in@IdGen\n"),
         (size, "Link : out_{num}@IdGen query_in@Peer{num}\n"),
         (size, "Link : q_out@Peer{num} inroute@Network\n"),
         (size, "Link : route_out{num}@Network q_in@Peer{num}\n"),
         (size, "Link : queryreachespeer@Peer{num} queryReached@log\n"),
         (1, "[Linker]\n"),
         (1, "datafile : serverdoc.dat\n"),
         (size, eachpeerspec), #read from file because this part is big
         (1, "\n[Network]\n components : net@PhysicalNetwork LTS@NetworkGraph\n"),
         (size, "out : c_out{num}\n"),
         (size, "out : route_out{num}\n"),
         (1, netspec),
         (1, "\n"),
         (size, "Link : c_out{num}@LTS c_out{num}\n"),
         (size, "Link : route_out{num}@net route_out{num}\n"),]
                

for part in parts:
    for j in range(part[0]): #the first element is a number indicating how many times the second is repeated
        MAFile.write(part[1].format(num=str(j),dlist=docsdistribution[j],flist=friendslist[j],qlist=querylist[j], cap = str(cap), TTL = str(TTL), numDoc = str(numDoc))) #write other element replacing the {num} with the appropriate integer

MAFile.close()

#===============================================
print("\n\n\nGenerating the testing file\n===================================================")
#generating the testing file :
testingfile = open(root_dir + "\\P2PSimulation\\p2p\\p2p-simulation\\coupled\\RSurfSimu\\testing1.bat", "w")

line1 = "cd " + root_dir + "\\P2PSimulation\\p2p\\p2p-simulation\\coupled\\RSurfSimu" + "\n"

print("please input number of hours for the simulation (from 0 to 99)")
hours = input()

print("please input number of minutes for the simulation (from 0 to 99)")
minutes = input()

print("please input number of seconds for the simulation (from 0 to 99)")
seconds = input()

print("please input number of milliseconds for the simulation (0 to 999)")
milliseconds = input()

line2 = 'simu.exe -mMultiPeer' + str(size) + '.MA -oGnOUT.out -t'+ '"' + '%02d' %hours + ':' + '%02d' %minutes + ':' + '%02d'%seconds + ':' + '%03d' %milliseconds +'"'+ ' > fileout.txt\n'

line3 = 'pause' 

testingfile.write(line1)
testingfile.write(line2)
testingfile.write(line3)
testingfile.close()

print "All files generated for a network size =", size
#time.sleep(5)
