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
    for i in graph.vs["peerId"]:
        if i == ID:
            return index
        else:
            index += 1
    return -1

#self-explanatory
def add_edge_between_peers(graph, fromId, toId):
    graph.add_edges((find_node_by_ID(graph,fromId), find_node_by_ID(graph,toId)))

#self-explanatory
def remove_edge_between_peers(graph, fromId, toId):
    graph.delete_edges(graph.get_eid(find_node_by_ID(graph,fromId),find_node_by_ID(graph,toId)))

#self-explanatory
def remove_node_by_ID(graph, ID):
    graph.delete_vertices(graph.vs[find_node_by_ID(graph,ID)])


#returns a list of all the paths from the specified node in the graph to all other nodes
def get_shortest_path(graph, fromId):
    return graph.get_shortest_paths(find_node_by_ID(graph,fromId),mode=ALL)


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
    total = 0
    for i in componentSizeList:
        total += i
    #print "    Average Component Size per Peer: "+str(total/onlineCount)
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
    #print "    Average Component Size per Component: " + str(total/compAmount)
    latestAvgComponentSize = total/onlineCount
    latestNumberofComponents = compAmount
    diameter = graph.diameter()
    a = (latestAvgComponentSize,latestNumberofComponents, diameter)
    return a


def add_Query(query, time):
    statList.append((query,time,0,0,latestAvgComponentSize,latestNumberofComponents, diameter))

def search_Query(query):
    for i in reversed(statList):
        if query in i:
            return statList.index(i)

def got_Queryhit(query):
    index = search_Query(query)
    tempTuple = statList[index]
    newTuple = (query, tempTuple[1], tempTuple[2], tempTuple[3]+1, tempTuple[4], tempTuple[5], tempTuple[6])
    statList[index] = newTuple

def peer_Reached(query):
    index = search_Query(query)
    tempTuple = statList[index]
    newTuple = (query, tempTuple[1], tempTuple[2]+1, tempTuple[3], tempTuple[4], tempTuple[5], tempTuple[6])
    statList[index] = newTuple


default_input = 'C:\\eclipse\\workspace'
# root_dir will be set to my_input if they just press enter
root_dir = raw_input("Enter your workspace directory(default: %s):\n" % default_input) or default_input
filein = open(root_dir + "\\P2PSimulation\\p2p\\ressources\\LogEvents.txt", "r")



fileout = open("Stats-Table.txt","w")


g = Graph(0)
statList = []
latestAvgComponentSize = 1
latestNumberofComponents = 1
onlineCount = 0.0
diameter = 0

for line in filein:
    currentLine = line.split(":")
    if len(currentLine) > 1:
        if currentLine[1] == "query":
            add_Query(int(currentLine[4]), int(currentLine[0]))
            #print "querying....."

        elif currentLine[1] == "queryhit":
            got_Queryhit(int(currentLine[4]))
            #print "queryhit....."

        elif currentLine[1] == "queryreachespeer":
            peer_Reached(int(currentLine[3]))
            

        elif currentLine[1] == "online":
            onlineCount += 1
            peerId = int(currentLine[2])
            #print "Peer " + str(peerId) + " ONLINE"
            add_vertex_with_attrs(g, {"peerId":peerId})
            a = stats(g)
            latestAvgComponentSize = a[0]
            latestNumberofComponents = a[1]
            diameter = a[2]
            

        elif currentLine[1] == "offline":
            onlineCount -= 1
            peerId = int(currentLine[2])
            
            #print "Peer " + str(peerId) + " OFFLINE"
            remove_node_by_ID(g,peerId)
            a = stats(g)
            latestAvgComponentSize = a[0]
            latestNumberofComponents = a[1]
            diameter = a[2]

        elif currentLine[1] == "connect":
            fromPeer = int(currentLine[2])
            toPeer = int(currentLine[3])
            #print "Peer " + str(fromPeer) + " connecting to " + str(toPeer)
            add_edge_between_peers(g,fromPeer,toPeer)
            a = stats(g)
            latestAvgComponentSize = a[0]
            latestNumberofComponents = a[1]
            diameter = a[2]

        elif currentLine[1] == "disconnect":
            fromPeer = int(currentLine[2])
            toPeer = int(currentLine[3])
            
            #print "Peer " + str(fromPeer) + " disconnecting from " + str(toPeer)
            remove_edge_between_peers(g,fromPeer,toPeer)
            a = stats(g)
            latestAvgComponentSize = a[0]
            latestNumberofComponents = a[1]
            diameter = a[2]


fileout.write("Query   Time    PeersReached     Queryhits     AvgComponentSize     NumberofComponents       Diameter\n")  
for i in statList:
    fileout.write(str(i[0]) + "  " + str(i[1]) + "    " + str(i[2]) + "    " + str(i[3]) + "    " + str(i[4]) + "    " + str(i[5]) + "    " + str(i[6]) + "\n")
print "Done outputting to the text file"


