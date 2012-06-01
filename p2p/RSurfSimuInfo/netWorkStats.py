from igraph import *



def add_vertex_with_attrs(graph, attrs):
    n = graph.vcount()
    graph.add_vertices(1)
    for key, value in attrs.iteritems():
        graph.vs[n][key] = value

def find_node_by_ID(graph, ID):
    index = 0
    for i in g.vs["peerId"]:
        if i == ID:
            return index
        else:
            index += 1
    return -1

def add_edge_between_peers(graph, fromId, toId):
    g.add_edges((find_node_by_ID(g,fromId), find_node_by_ID(g,toId)))

def remove_edge_between_peers(graph, fromId, toId):
    g.delete_edges(g.get_eid(find_node_by_ID(g,fromId),find_node_by_ID(g,toId)))
    #g.delete_edges((find_node_by_ID(g,fromId), find_node_by_ID(g,toId)))

def remove_node_by_ID(graph, ID):
    g.delete_vertices(g.vs[find_node_by_ID(g,ID)])



def get_shortest_path(fromId):
    return g.get_shortest_paths(find_node_by_ID(g,fromId),mode=ALL)


"""  Don't know exactly what this returns, because I don't understand the get_shortest_paths method...
Will try and fix soon
"""
def stats():
    componentSizeList = []
    for i in g.vs["peerId"]:
        path = 0
        a = get_shortest_path(i)
        compSize = 1
        for j in a:
            if len(j) > 1:
                compSize += 1
        componentSizeList.append(compSize)
    #print "    Number of Peers Online: "+str(onlineCount)
    fileout.write("    Number of Peers Online: " + str(onlineCount) + "\n")
    total = 0
    for i in componentSizeList:
        total += i
    #print "    Average Component Size per Peer: "+str(total/onlineCount)
    fileout.write("    Average Component Size per Peer: " + str(total/onlineCount) + "\n")
    compAmount = 0.0
    componentSize = 0
    while len(componentSizeList) > 0:
        a = componentSizeList[0]
        componentSize += a
        for i in range(a):
            componentSizeList.remove(a)
        compAmount+=1
    #print "    Number of Components in the network: "+str(compAmount)
    fileout.write("    Number of Components in the network: " + str(compAmount) + "\n")
    #print "    Average Component Size per Component: " + str(total/compAmount)
    fileout.write("    Average Component Size per Component: " + str(componentSize/compAmount) + "\n")

default_input = 'C:\\eclipse\\workspace'
# root_dir will be set to my_input if they just press enter
root_dir = raw_input("Enter your workspace directory(default: %s):\n" % default_input) or default_input
filein = open(root_dir + "\\P2PSimulation\\p2p\\p2p-simulation\\coupled\\RSurfSimu\\fileout.txt", "r")

#filein = open("fileout.txt", "r")
fileout = open("Output.txt", "w")

#g = Graph.Formula("0-->0")
#g.delete_vertices(g.vs)
g = Graph(0)

onlineCount = 0.0

connectCount= 0
for line in filein:
    if "online" in line:
        currentLine = line.split()
        peerId = currentLine[2]
        add_vertex_with_attrs(g, {"peerId":peerId})
        onlineCount += 1
        #print currentLine[0] + ":"
        fileout.write("\n"+currentLine[0] + "   -   Peer " + str(peerId) + " going online:\n")
        stats()
        #print "    Diameter: " + str(g.diameter())
        fileout.write("    Network Diameter: " + str(g.diameter()) + "\n")
        

    if "disconnect" not in line and "connect" in line:
        currentLine = line.split();
        peers = currentLine[2][:-8]
        #print peers
        if len(peers) > 3:
            
            fromPeer = peers[:-3]
            toPeer = str(int(peers) - (int(fromPeer)*1000))
            add_edge_between_peers(g,fromPeer,toPeer)
            #print currentLine[0] + ":"
            fileout.write("\n"+currentLine[0] + "   -   Peer " + str(fromPeer) + " connecting to Peer " + str(toPeer) + ":\n")
            stats()
            #print "    Diameter: " + str(g.diameter())
            fileout.write("    Network Diameter: " + str(g.diameter()) + "\n")
                

    if "disconnect" in line:
        currentLine = line.split()
        peers = currentLine[2][:-8]
        if len(peers) > 3:
            fromPeer = peers[:-3]
            toPeer = str(int(peers) - (int(fromPeer)*1000))
            remove_edge_between_peers(g,fromPeer,toPeer)
            #print currentLine[0] + ":"
            fileout.write("\n"+currentLine[0] + "   -   Peer " + str(fromPeer) + " disconnecting from Peer " + str(toPeer) + ":\n")
            stats()
            #print "    Diameter: " + str(g.diameter())
            fileout.write("    Network Diameter: " + str(g.diameter()) + "\n")

            
    if "offline" in line:
        onlineCount -=1
        currentLine = line.split()
        peerId = currentLine[2]
        remove_node_by_ID(g,peerId)
       # print currentLine[0] + ":"
        fileout.write("\n"+currentLine[0] + "   -   Peer " + str(peerId) + " going offline:\n")
        stats()
       # print "    Diameter: " + str(g.diameter())
        fileout.write("    Network Diameter: " + str(g.diameter()) + "\n")



##componentSizeList = []
##for i in g.vs["peerId"]:
##    path = 0
##    a = get_shortest_path(i)
##    compSize = 1
##    for j in a:
##        if len(j) > 1:
##            compSize += 1
##    componentSizeList.append(compSize)
##print "Number of Peers Online: "+str(onlineCount)
##total = 0
##for i in componentSizeList:
##    total += i
##print "Average Component Size per Peer: "+str(total/onlineCount)
##compAmount = 0
##while len(componentSizeList) > 0:
##    a = componentSizeList[0]
##    for i in range(a):
##        componentSizeList.remove(a)
##    compAmount+=1
##print "Number of Components in the network: "+str(compAmount)
##print "Average Component Size per Component: " + str(total/compAmount)

filein.close()
g.vs["label"] = g.vs["peerId"]
plot(g, layout = "fr")


