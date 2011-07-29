#ifndef __SGRAPH
#define __SGRAPH
#define VERBOSE true
//header file for my Network graph class
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <set>


#include "..\..\atomic\server\DBGraph.h"



using namespace graph;


class SGraph : public DBGraph //directed graph with nodes and edges labeled with ints.. also can have int tags attached to nodes... we don't use that.
{

	public:


	//adds a query
	bool SGraph::addquery(int query);

	//removes a query
	bool SGraph::removequery(int query);

	//Creates a query-doc link
	bool SGraph::querydoclink(int query, int doc);

	//Unlinks a query to a doc
	bool SGraph::removequerylink(int query, int doc);

	set<int> SGraph::getDocsFromLink(int d);

	set<int> SGraph::getDocsFromLink(int d, int lbl);
	//gets the set of documents linked to by a specific query
	set<int> SGraph::getDocsFromQuery(int query);

    /*to read a graph from a file (or input stream)*/
    void SGraph::read(std::istream & is);




};
#endif //ServGraph
