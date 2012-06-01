from igraph import *


#adds a vertex to the graph and gives it the specified attribute (e.g. {"peerId":35} will label it as 35 within the "peerId" attribute)
def add_vertex_with_attrs(graph, attrs):
    n = graph.vcount()
    graph.add_vertices(1)
    for key, value in attrs.iteritems():
        graph.vs[n][key] = value

#finds and return the index of the node using the ID of the peer (index might change whenever the graph changes)
def find_node_by_ID(graph, ID):
    index = 0
    for i in g.vs["peerId"]:
        if i == ID:
            return index
        else:
            index += 1
    return -1

#self-explanatory
def add_edge_between_peers(graph, fromId, toId):
    graph.add_edges((find_node_by_ID(g,fromId), find_node_by_ID(g,toId)))

#self-explanatory
def remove_edge_between_peers(graph, fromId, toId):
    graph.delete_edges(g.get_eid(find_node_by_ID(g,fromId),find_node_by_ID(g,toId)))

#self-explanatory
def remove_node_by_ID(graph, ID):
    graph.delete_vertices(g.vs[find_node_by_ID(g,ID)])


#returns a list of all the paths from the specified node in the graph to all other nodes
def get_shortest_path(graph, fromId):
    return graph.get_shortest_paths(find_node_by_ID(g,fromId),mode=ALL)


"""  uses the get_shortest_math method defined above to calculate the number of components in the graph along with the average
component size per peer/component. Outputs these statistics to the output file along with the amount of peers online
"""
def stats(graph):
    componentSizeList = []   #list of all the peer's component sizes

    #Iterate through each vertex
    for i in graph.vs["peerId"]:
        path = 0

        #get a list of list of all the paths from vertex i to all other vertices (inner empty list means there is no path)
        a = get_shortest_path(graph,i)
        compSize = 1   #number of peers in the component that i belongs to
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

    #Getting the amount of components in the graph
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
fileout = open("NetworkStatsOutput.txt", "w")

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
        stats(g)
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
            stats(g)
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
            stats(g)
            #print "    Diameter: " + str(g.diameter())
            fileout.write("    Network Diameter: " + str(g.diameter()) + "\n")

            
    if "offline" in line:
        onlineCount -=1
        currentLine = line.split()
        peerId = currentLine[2]
        remove_node_by_ID(g,peerId)
       # print currentLine[0] + ":"
        fileout.write("\n"+currentLine[0] + "   -   Peer " + str(peerId) + " going offline:\n")
        stats(g)
       # print "    Diameter: " + str(g.diameter())
        fileout.write("    Network Diameter: " + str(g.diameter()) + "\n")





filein.close()
fileout.close()
g.vs["label"] = g.vs["peerId"]
plot(g, layout = "fr")


