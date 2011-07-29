#ifndef __DBGRAPH
#define __DBGRAPH
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


class DBGraph : public Graph<int, int, true, int> //directed graph with nodes and edges labeled with ints.. also can have int tags attached to nodes... we don't use that.
{

    public:
    	DBGraph::DBGraph()
	{
	}


     //add a peer
     bool DBGraph::addpeer(int peer);
     //add a document
     bool DBGraph::adddoc(int doc);

     // remove a peer from the DB (not sure why we might do this)
     bool DBGraph::removepeer(int peer);

     // remove a doc from the DB altogether (not sure why we might do this)
     bool DBGraph::removedoc(int peer);

     //a peer publishes a doc
     bool DBGraph::publish(int p, int d);

     //a peer removes a doc from its repository
     bool DBGraph::remove(int p1, int d);

     //to add a link between two documents: link goes from d1 to d2, is labeled "label" (actually a number)
     bool DBGraph::linkdocs(int d1, int d2, int label);

     /*to read a graph from a file (or input stream)*/
     void DBGraph::read(std::istream & is);

     /*to output a graph to an output stream (e.g. the standard out)*/
     void DBGraph::write(std::ostream & os);

     /*get the set of documents published by peer p*/
     set<int> DBGraph::getsDocsOfPeer(int p);

     /* get the docs that the input doc links to, where the link label is lbl*/
     set<int> DBGraph::getsDocsFromLink(int d, int lbl);

    private:

    //private methods to deal with graph-level repeated stuff
    bool DBGraph::connect(int p1, int p2, int label, bool addmissingnodes);
    bool DBGraph::disconnect(int p1, int p2);

     //get the nodes connected to some input node
     //returns a set of integers containing the set of connected nodes
     set<int> DBGraph::getConnectedNodes(int innode);

    //same expect input directly a node_iterator
    set<int> DBGraph::getConnectedNodes(node_iterator nit);


};

#endif //DBGraph
