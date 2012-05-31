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

def avg_component_size_per_peer():
    componentList = []
    for i in g.vs["peerId"]:
        pathList = get_shortest_path(i)
        #for j in pathList:
        #    if len(j)!= 0:
        #        print j
        print pathList
    total = 0
    #print len(componentList)
    #for i in componentList:
       # total += i
       #print i
    #a = total/len(componentList)
   # return a

filein = open("fileout.txt","r")
fileout = open("Output.txt", "w")

#g = Graph.Formula("0-->0")
#g.delete_vertices(g.vs)
g = Graph(0)
print g

onlineCount = 0
connectCount= 0
for line in filein:
    if "online" in line:
        currentLine = line.split()
        peerId = currentLine[2]
        add_vertex_with_attrs(g, {"peerId":peerId})
        onlineCount += 1
        

    if "disconnect" not in line and "connect" in line:
        currentLine = line.split();
        peers = currentLine[2][:-8]
        #print peers
        if len(peers) > 3:
            
            fromPeer = peers[:-3]
            toPeer = str(int(peers) - (int(fromPeer)*1000))
            add_edge_between_peers(g,fromPeer,toPeer)
            #print(currentLine[0] + " - Diameter: " + str(g.diameter()))
           # a = avg_component_size_per_peer();
            #print(currentLine[0] + " - avg_component_size_per_peer: " + str(a) + " - Diameter: " + str(g.diameter()))
                

    if "disconnect" in line:
        currentLine = line.split()
        peers = currentLine[2][:-8]
        if len(peers) > 3:
            fromPeer = peers[:-3]
            toPeer = str(int(peers) - (int(fromPeer)*1000))
            remove_edge_between_peers(g,fromPeer,toPeer)
            #print(currentLine[0] + " - Diameter: " + str(g.diameter()))
           # a = avg_component_size_per_peer();
            #print(currentLine[0] + " - avg_component_size_per_peer: " + str(a) + " - Diameter: " + str(g.diameter()))

            
    if "offline" in line:
        currentLine = line.split()
        peerId = currentLine[2]
        remove_node_by_ID(g,peerId)


for i in g.vs["peerId"]:
    if(i == "44"):
        print "true"
        path = 0
        for j in get_shortest_path(i):
            for k in j:
                if k > 0:
                    path += 1;
            if(path != 0):
                print path
            path = 0
        
        


filein.close()
print g
print "Diameter of the graph:",
print g.diameter()
print "number of connections that have occured: " + str(connectCount)
print "number of peers that went onlines: " + str(onlineCount)
g.vs["label"] = g.vs["peerId"]
plot(g, layout = "fr")


