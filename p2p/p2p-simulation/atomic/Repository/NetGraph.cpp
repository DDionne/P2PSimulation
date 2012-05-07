#include "NetGraph.h"
#define GVERBOSE false

//a peer goes online
bool GraphInt::online(int peer){
	insert(peer);
	return true;
}

// a peer goes offline
bool GraphInt::offline(int peer){
	erase(peer);	//removes ALL nodes of the graph with that value (node label)
	return true;
}

//two peers connect
bool GraphInt::connect(int p1, int p2){

	// insert an edge with label=1 from node (with label = )"from" to node "to"

	node_iterator nit = find(p1); //find node from
	nit->insert_edge(find(p2),1); // connect to node "to" with edge labeled 1

	return true; //at some point I want to return false if there is something wrong (one of the peers not found...)
}

//two peers disconnect
bool GraphInt::disconnect(int p1, int p2){
	// delete an edge from node (with label = )"from" to node "to"
	node_iterator nit = find(p1); //find node p1
	edge_iterator eito = nit->find_edge(find(p2)); //find edge to node p2

	//thegraph->erase_edge(nit,nito);
	nit->erase_edge(eito); // erase edge to node "p2"

}

/*get connected nodes from a label
 *
 */
set<int> GraphInt::getConnectedNodes(int innode){

	node_iterator nit = find(innode); //find node "from"
	//int degre = nit->count_edges();
	//cout<<"step1\n";
	return getConnectedNodes(nit);
}

/*
 * get connected nodes from a node_iterator
 */
set<int> GraphInt::getConnectedNodes(node_iterator nit){

	set<int> result;
	//cout<<"step2\n";

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
    		else  //else maybe the new node is the "from" node (graph is undirected)
			{
				tonodes = ei.from();
				if(GVERBOSE) cout<<"adding 'from' node!!!! node label="<<*tonodes<<"\n";
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

/*read a graph from an input stream (supposed to be a file is but not necessary
 *
 * Expected format (example):
 * 6 	// number of nodes
 * 0 0 0 1 1 0 // adjacency matrix : 1 is connected to 4 and 5
 * 0 0 0 0 1 1
 * 0 0 0 1 0 1
 * 1 0 1 0 0 0 // 4 connected to 1 and 3 .. if the graph is undirected is this necessary ?
 * 1 1 0 0 0 0
 * 0 1 1 0 0 0
 *
 * */

void GraphInt::read(std::istream & is)
{
	int i, n;

	/* read number of nodes */
	is >> n;

	/* label the nodes from 0 to n-1 */
	for (i = 0; i < n; i++)
		this->insert(i);

	//"The adjacency matrix (positive values, 0 means no link):"

	for (i = 0; i < n; i++)
	{
		node_iterator ni = this->find(i);
		for (int j = 0; j < n; j++)
		{
			int w;
			is >> w;
			if (w > 0)
				ni->insert_edge(this->find(j), w);
		}
	}
}

/*
 * output graph to ostream;
 */
void GraphInt::write(std::ostream & os)
{
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
