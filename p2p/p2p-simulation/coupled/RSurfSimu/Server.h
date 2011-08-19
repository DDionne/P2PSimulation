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
#define MAXDOCS 100

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
#include <list>
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
    const Port &peer_in ;


	const Port &publish;
	const Port &remove;




	//outputs
	Port &queryhit ;
	Port &p_out;
	Port &link;
	/*******************************************************************/

	queue<long long> QueryhitQ; //queue of queryhits that need to be output
	queue<long long> publishQ; //queue of all the documents being published
	list<int> currentDocs; //list of all documents published by a peer
	queue<long long> docCheckQ;
	bool docFound;



};	// class Server


#endif    //__DB_H
