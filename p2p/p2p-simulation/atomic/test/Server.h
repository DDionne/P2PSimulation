/*******************************************************************
*
*  DESCRIPTION: Atomic Model :
*  Server : maintains a mock-up Server for the whole network and responds to queries
*
*  AUTHOR: Denis
*
*
*******************************************************************/
/*handle duplicate inclusion of this header*/
#ifndef __DB_H
#define __DB_H
#define SVERBOSE false

/** include files **/
#include "atomic.h"  // class Atomic
#include "..\complexmessages.h" // functions to manage "coded" messages between components
#include "..\complexmessages2.h"
#include "..\..\lib\graph-c++.h" // classes to handle the graph (network)
#include "DBGraph.h"
#include "SGraph.h"
#include <queue>
#include<map>
#include <set>
#include <stdio.h>
#define SIZE 104
/** declarations **/
class Server: public Atomic
{
public:


	Server( const string &name = "Server" ) ;	 // Default constructor

	~Server();					// Destructor

	virtual string className() const
		{return "Server";};


protected:
	Model &initFunction()
		{
		return *this;
		};

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:

	/**************** PORTS *********************************************/
    const Port &peer_in ; // for getting peer id in : a peerid coming in means the peer receives the query associated with the id
	const Port &query_in; // for updating the routing table (adding a query / msg id association)


	const Port &publish;
	const Port &remove;




	//outputs
	Port &queryhit ; // queryhit : docid + msgid
	Port &p_out;
	/*******************************************************************/

	SGraph* serverdoc;//graph of query - document associations (what query matches what document) + doc - doc links.
	queue<long long> QueryhitQ; //queue of queryhits that need to be output
	queue<long long> publishQ; //queue of all the documents being published
	set<int> currentDocs; //list of all documents published by a peer
	queue<long long> docCheckQ;
	SGraph* a[SIZE];



};	// class Server


#endif    //__DB_H
