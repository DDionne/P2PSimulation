#include "DBGraph.h"
#define GVERBOSE false

//add a peer
bool DBGraph::addpeer(int peer){
	insert(peer);
	return true;
}

//add a document
bool DBGraph::adddoc(int d){
	insert(d *(-1)); // doc nodes have negative label "-d"
	return true;
}


// remove a peer
bool DBGraph::removepeer(int peer){
	erase(peer);	//removes ALL nodes of the graph with that value (node label)
	return true;
}

// remove a peer
bool DBGraph::removedoc(int doc){
	erase((-1)*doc);	//removes ALL nodes of the graph with that value (node label)
	return true;
}


//any connection being added to the graph
bool DBGraph::connect(int p1, int p2, int label, bool addmissingnodes){

	// insert an edge with label=1 from node (with label = ) "from" to node "to"

	node_iterator nit = find(p1); //find node from
	if (nit==end()){
		if (addmissingnodes){ //if we're supposed to add missing nodes
			insert(p1);
			nit= find(p1);
		} else
			return false;
	}
	node_iterator nit2 = find(p2); // find node to
	if (nit2==end()){
			if (addmissingnodes){
				insert(p2);
				nit2= find(p2);
			} else
				return false;
		}

	nit->insert_edge(nit2,label); // connect to node "to" with edge labeled 1
	if(GVERBOSE) cout<<"connecting " <<*nit<<" and" <<*nit2<<" with  label="<<label<<"\n";
	return true;
}

//publish
bool DBGraph::publish(int p1, int d){
	// insert an edge with label=1 from node (with label = )"from" to node "to", and add doc or peer if they're not in the list
	return connect(p1, -1*d, 1, true);
}

//a peer removes a doc
bool DBGraph::remove(int p1, int d){
    return disconnect(p1, -1*d); // disconnect the peer from the doc
}

//to add a link between two documents: link goes from d1 to d2, is labeled "label" (actually a number)
bool DBGraph::linkdocs(int d1, int d2, int label){
	return connect (-1*d1, -1*d2, label, true); // add the link. Also add the docs if missing (should we?)
}


/*get the set of documents published by peer p*/
set<int> DBGraph::getsDocsOfPeer(int p){
	set<int> docs = getConnectedNodes(p);
	set<int> final;
	set<int>::iterator doc;
	for ( doc=docs.begin() ; doc != docs.end(); doc++ ){
		final.insert(*doc * (-1));//add the positive integer corresponding to this doc (stored as negative int)
	}
	return final;
}

/* get the docs that the input doc links to, where the link label is lbl*/
set<int> DBGraph::getsDocsFromLink(int d, int lbl){
	set<int> docs = getConnectedNodes((-1)*d, lbl);//starting from a doc
	set<int> final;
	set<int>::iterator doc;
	for ( doc=docs.begin() ; doc != docs.end(); doc++ ){
		final.insert(*doc*(-1));//add the positive integer corresponding to this doc (stored as negative int)
	}
	return final;
}

/* get the docs that the input doc links to*/
set<int> DBGraph::getsDocsFromLink(int d){
	getsDocsOfPeer(d*(-1));
}

//TODO: make explicit the label of the edge that will be removed
bool DBGraph::disconnect(int p1, int p2){
	bool found = false;
	// delete an edge from node (with label = )"from" to node "to"
	node_iterator nit = find(p1); //find node p1
	node_iterator nit2 = find(p2); // find node to
	if (nit==end() || nit2==end()) return false; // one of the nodes is missing (not online)
	edge_iterator eito = nit->find_edge(nit2); //find edge to node p2

	//thegraph->erase_edge(nit,nito);
	nit->erase_edge(eito); // erase edge to node "p2"



	return true;

}

/*get connected nodes from a label
 *
 */
set<int> DBGraph::getConnectedNodes(int innode){

	node_iterator nit = find(innode); //find node "from"
	//int degre = nit->count_edges();
	//cout<<"step1\n";
	if (nit==end()) return set<int>();
	return getConnectedNodes(nit);
}

/*
 * get connected nodes from a node_iterator (uses edge direction : only return nodes connected by an edge directed *from* this node)
 */
set<int> DBGraph::getConnectedNodes(node_iterator nit){

	set<int> result;
	//cout<<"step2\n";
	if (nit==end()) return result;
	//cout<<"step3\n";
	int cnt =0;
    	for (edge_iterator ei = nit->begin(); ei != nit->end(); ei++) // browse the edges
    	{
    		//get nodes at other end of edge
    		node_iterator tonodes = ei.to();
    		if (*tonodes != *nit )  //this would be == if the to() node we're trying to get is in fact ni, but we're comparing the labels
    		{
    			if(GVERBOSE) cout<<"adding 'to' node!!!! node label="<<*tonodes<<"\n";
			}

    		//in any case ...
    		result.insert(*tonodes); //add the connected node to my result and increment cnt
    	}
    	//at this point, "result" should contain all the nodes connected to the
    	if(GVERBOSE) {
    		cout<<"finished adding nodes\n";
    		//result->display();
    		cout<<"about to return\n";

    	}

	//cout<<"step4\n";
	//random test
	//result->display();
	//result->insert(1);
	//cout<<"step5--\n";


	return result;
}









set<int> DBGraph::getConnectedNodes(int innode, int lbl){

	node_iterator nit = find(innode); //find node "from"
	//int degre = nit->count_edges();
	//cout<<"step1\n";
	if (nit==end()) return set<int>();
	return getConnectedNodes(nit, lbl);
}


/*
 * get connected nodes from a node_iterator (uses edge direction : only return nodes connected by an edge directed *from* this node)
 */
set<int> DBGraph::getConnectedNodes(node_iterator nit, int lbl){

	set<int> result;
	//cout<<"step2\n";
	if (nit==end()) return result;
	//cout<<"step3\n";
	int cnt =0;
    	for (edge_iterator ei = nit->begin(); ei != nit->end(); ei++) // browse the edges
    	{
    		//get nodes at other end of edge
    		node_iterator tonodes = ei.to();
    		if (*tonodes != *nit )  //this would be == if the to() node we're trying to get is in fact ni, but we're comparing the labels
    		{
    			if(GVERBOSE) cout<<"adding 'to' node!!!! node label="<<*tonodes<<"\n";
			}
    		if(GVERBOSE) cout<<"edge label="<<*ei<<"\n";
    		if(*ei == lbl){
    			//in any case ...
    			result.insert(*tonodes); //add the connected node to my result and increment cnt
    		}
    		//in any case ...
    		//result.insert(*tonodes); //add the connected node to my result and increment cnt
    	}
    	//at this point, "result" should contain all the nodes connected to the node with a specified link label
    	if(GVERBOSE) {
    		cout<<"finished adding nodes\n";
    		//result->display();
    		cout<<"about to return\n";

    	}

	//cout<<"step4\n";
	//random test
	//result->display();
	//result->insert(1);
	//cout<<"step5--\n";


	return result;
}
















/*read a graph from an input stream (supposed to be a file is but not necessary
 * this does not allow multiple links between docs... TODO: add support for multiple links
 * Expected format (example):
 * 3 6 	// number of peers, number of documents
 * 0 0 0 1 1 0 //peer-doc adjacency matrix :// peer 1 is connected to docs 4 and 5
 * 0 1 0 0 1 1 // peer 2 is connected to doc 2, doc 5, doc 6
 * 1 0 0 0 0 0 // peer 3 is connected to doc 1
 * 0 0 2 0 0 0 // doc-doc adjacency matrix starts here. This graph must be acyclic.
 * 6 0 0 0 0 0 // doc 2 links to doc 1 with label "6"
 * 0 6 0 0 0 0
 * 0 0 0 0 0 0
 * 0 4 0 0 0 0
 * 0 4 0 0 3 0 // doc 6 links to doc 2 with label 4 and to doc 5 with label 3.
 *
 * Here for a peer / document network we need a structure where the peers and documents are separate numerical intervals.
 * */

void DBGraph::read(std::istream & is)
{
	//TODO: make this work for peers / docs
	int i, p, d; //index, number of peers, number of docs

	/* read number of peers */
	is >> p;
	is >> d;

	/* label the peers from 1 to p, insert them */
	for (i = 1; i < p+1; i++)
		addpeer(i);

	//insert the docs labeled 1 to d
	for (i = 1; i < d+1; i++)
			adddoc(i);

	//The doc-peer adjacency matrix (positive values, 0 means no link):"
	for (i = 1; i < p+1; i++)
	{
		//node_iterator ni = this->find(i);
		for (int j = 1; j < d+1; j++)
		{
			int w;
			is >> w;
			if (w > 0)
				publish(i, j);
		}
	}
	//the doc-doc adjacency matrix
	for (i = 1; i < d+1; i++)
		{
			//node_iterator ni = this->find(i);
			for (int j = 1; j < d+1; j++)
			{
				int w;
				is >> w;
				if (w > 0)
					linkdocs(i, j, w);
			}
		}
}


//output graph to ostream;
/*
void DBGraph::write(std::ostream & os)
{
	//TODO: add the indications peer / doc
	os<<"Nodes: connect to: \n";
	for (node_iterator ni = begin(); ni!= end(); ni++) // iterate through the nodes
	{
		os<<*ni<<" : ";
		set<int> connections = getConnectedNodes(ni);
		for (set<int>::iterator nums = connections.begin(); nums!= connections.end(); nums++){
			os<<" "<<*nums;
		}
		os<<endl;
	}

}
*/
