#ifndef __NETGRAPH
#define __NETGRAPH
#define VERBOSE true
//header file for my Network graph class
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <set>


#include "..\..\lib\graph-c++.h"
#include "..\..\lib\graph-c++-comp.h"


using namespace graph;


class GraphInt : public Graph<int, int, false, int>
{

    public:
    	GraphInt::GraphInt()
	{
	}


    //a peer goes online
     bool GraphInt::online(int peer);

     // a peer goes offline
     bool GraphInt::offline(int peer);

    //two peers connect
    bool GraphInt::connect(int p1, int p2);

    //two peers disconnect
    bool GraphInt::disconnect(int p1, int p2);

    //get the nodes connected to some input node
    //returns a set of integers containing the set of connected nodes
    set<int> GraphInt::getConnectedNodes(int innode);

    //same expect input directly a node_iterator
    set<int> GraphInt::getConnectedNodes(node_iterator nit);

    /*to read a graph from a file (or input stream)*/
    void GraphInt::read(std::istream & is);

    /*to output a graph to an output stream (e.g. the standard out)*/
    void GraphInt::write(std::ostream & os);

};

#endif //NETGRAPH
