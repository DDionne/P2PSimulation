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

def checkEdge(graph, fromId, toId):
    a = get_shortest_path(graph,fromId)
    if len(a[find_node_by_ID(graph, toId)])-1 == 1:
        return 1
    else:
        return -1
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


""" handles the query event in the log_events file, basically adds a query to the list with 0 peers reached/queryhits along with the appropriate
    Network stats at the time of the query """ 
def add_Query(query, time):
    statList.append((query,time,1,0,latestAvgComponentSize,latestNumberofComponents, diameter, totalDocuments, onlineCount))

""" Searches for the index of the latest query made with the proper ID (iterates from the back of the list to the front) """
def search_Query(query):
    for i in reversed(statList):
        if i[0] == query:
            return statList.index(i)
    return -1 #if the query isn't found, this should never happen


""" Handles the Queryhit event by finding the index of the query which received a queryhit and updating its queryhit information by
    incrementing it by one """
def got_Queryhit(query):
    index = query#search_Query(query)
    tempTuple = statList[index]
    newTuple = (query, tempTuple[1], tempTuple[2], tempTuple[3]+1, tempTuple[4], tempTuple[5], tempTuple[6], tempTuple[7], tempTuple[8])
    if tempTuple[0] != newTuple[0]:
        print "value changed..."
    statList[index] = newTuple

""" Handles the Peer Reached event by finding the index of the query which reached another peer and updating its peers reached information
    by incrementing it by one """
def peer_Reached(query):
    index = query#search_Query(query)
    tempTuple = statList[index]
    newTuple = (query, tempTuple[1], tempTuple[2]+1, tempTuple[3], tempTuple[4], tempTuple[5], tempTuple[6], tempTuple[7], tempTuple[8])
    if tempTuple[0] != newTuple[0]:
        print "value changed..."
    statList[index] = newTuple



#default_input = 'C:\\eclipse\\workspace'
# root_dir will be set to my_input if they just press enter
#root_dir = raw_input("Enter your workspace directory(default: %s):\n" % default_input) or default_input
filein = open("fileout.txt", "r")



fileout = open("Stats-Table-netlogo.txt","w")

#g = Graph.Formula("0-->0")
#g.delete_vertices(g.vs)
g = Graph(0)    #create an empty undirected graph
statList = []   #list of all the relevent information
documentsPerPeer = [0 for x in range(100)]
latestAvgComponentSize = 1
latestNumberofComponents = 1
onlineCount = 0.0
diameter = 0
totalQhits = 0
totalPReaches = 0
totalDocuments = 0
lineNum = 0
linePrint = 100000

for line in filein:
    currentLine = line.split(":")
    if len(currentLine) > 1:
        if currentLine[1] == "query":
            add_Query(int(currentLine[4]), float(currentLine[0]))

        elif currentLine[1] == "queryhit":
            got_Queryhit(int(currentLine[4])) #currentLine[4] corresponds to the queryID of the queryhit message
            totalQhits += 1

        elif currentLine[1] == "queryreachespeer":
            peer_Reached(int(currentLine[3])) #currentLine[3] corresponds to the queryID of the Peers Reached message
            totalPReaches += 1
            

        elif currentLine[1] == "online":
            onlineCount += 1                  #keep track of how many peers are online by incrementing it everytime a peer comes online
            peerId = int(currentLine[2])      #currentLine[2] corresponds to the Id of the peer which went online
            totalDocuments += documentsPerPeer[peerId]
            add_vertex_with_attrs(g, {"peerId":peerId}) #add the peer to the graph with its corresponding ID
            a = stats(g)    #update the appropriate network information
            latestAvgComponentSize = a[0]   
            latestNumberofComponents = a[1]
            diameter = a[2]
            

        elif currentLine[1] == "offline":
            onlineCount -= 1                  #keep track of how many peers are online by decrementing whenever a peer goes offline
            peerId = int(currentLine[2])      #currentLine[2] corresponds to the Id of the peer which went offline
            totalDocuments -= documentsPerPeer[peerId]
            remove_node_by_ID(g,peerId)       #remove the peer from the network graph
            a = stats(g)                      #update the appropriate network information
            latestAvgComponentSize = a[0]
            latestNumberofComponents = a[1]
            diameter = a[2]

        elif currentLine[1] == "connect":
            fromPeer = int(currentLine[2])    #currentLine[2] corresponds to the peer which is establishing the connection
            toPeer = int(currentLine[3])      #currentLine[3] corresponds to the peer which is being connected to
            add_edge_between_peers(g,fromPeer,toPeer)   #adds a connection (an edge) between the two peers in the network graph
            a = stats(g)                      #update the appropriate network information
            latestAvgComponentSize = a[0]     
            latestNumberofComponents = a[1]
            diameter = a[2]

        elif currentLine[1] == "disconnect":
            #print lineNum
            #g.vs["label"] = g.vs["peerId"]
            #plot(g, layout = "fr")
           
            fromPeer = int(currentLine[2])    #same idea as for the connect section, but instead of connecting, they are disconnecting (remove the edge)
            toPeer = int(currentLine[3])


            #if lineNum == 835249:
                #print g.get_adjlist()
           # a = get_shortest_path(g,fromPeer)
           # print "disconnect " + str(fromPeer) + " from " + str(toPeer) + " " + str(len(a[find_node_by_ID(g, toPeer)])-1)
        
            
            remove_edge_between_peers(g,fromPeer,toPeer)
            a = stats(g)
            latestAvgComponentSize = a[0]
            latestNumberofComponents = a[1]
            diameter = a[2]

        elif currentLine[1] == "publish":
            documentsPerPeer[int(currentLine[2])] += 1
            totalDocuments += 1

        elif currentLine[1] == "remove":
            documentsPerPeer[int(currentLine[2])] -= 1
            totalDocuments -= 1
        lineNum += 1
        if lineNum == linePrint:
            print lineNum
            linePrint += 100000


fileout.write("Query   Time(s)    Peers-Reached     QueryHits     Average-Component-Size     Number-of-Components       Diameter        Number-of-Documents      Peers-Online\n")  
for i in statList:
    fileout.write(str(i[0]) + "  " + str(i[1]) + "    " + str(i[2]) + "    " + str(i[3]) + "    " + str(i[4]) + "    " + str(i[5]) + "    " + str(i[6]) + "    " + str(i[7]) + "    " + str(i[8]) + "\n")
print "Done outputting to the text file"
print totalPReaches
print totalQhits


