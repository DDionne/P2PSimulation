/*******************************************************************
*
*  DESCRIPTION: Atomic Model :  Router Router.
*  Simulates the Router protocol to route messages through a network
*
*  AUTHOR: Alan
*
*
*******************************************************************/
/*handle duplicate inclusion of this header*/
#ifndef __GTELLA_H
#define __GTELLA_H
#define VERBOSE false
#define STANDARDTTL 7

/** include files **/
#include "atomic.h"  // class Atomic
#include "..\complexmessages2.h" // functions to manage "coded" messages between components
#include<set>
#include<map>
#include<queue>


/** declarations **/
class Router: public Atomic
{
public:


	Router( const string &name = "Router Router" ) ;	 // Default constructor

	~Router();					// Destructor

	virtual string className() const
		{return "Router Router";};


protected:
	Model &initFunction()
		{
		return *this;
		};

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:


	const Port &q_in;
	const Port &query;
	const Port &connections;
	Port &localOut;
	Port &q_out;
	Port &connect_out;
	Port &disconnect_out;
	Port &qhit_out;


	// routingTables used for routing queryhits back to the peer that initially queried
	map<long,long> routingTable1;
	map<long,long> *secondary;
	map<long,long> routingTable2;
	map<long,long> *primary;


	set<int> msgIdList; // list of messages received (used to ignore other messages with the same value)
	set<int> msgIdList2;

	queue<long long> QueryQueue; // Queue that holds all the queries about to be sent to connections
	queue<long long> disconnectionQueue; //Queue that holds all the disconnections that have been made (used for output)
	queue<long long> connectionQueue; //Queue that holds all the connections that have been made by the peer (used for output)
	set<int> connectionlist; //set of all the peers that are connected by the current peer
	queue<long long> QueryHitQueue; // Queue that holds all the queryHits (sent to the randomSurfer)
	queue<long long> localQuery; // Queue that holds all the local queries (sent to the server)



};	// class Router


#endif    //__GTELLA_H
