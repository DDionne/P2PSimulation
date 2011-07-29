/*******************************************************************
*
*  DESCRIPTION: Atomic Model :  PeerGnutella Router.
*  Simulates the PeerGnutella protocol to route messages through a network
*
*  AUTHOR: Alan
*
*
*******************************************************************/
/*handle duplicate inclusion of this header*/
#ifndef __GTELLA_H
#define __GTELLA_H
#define VERBOSE true
#define STANDARDTTL 7

/** include files **/
#include "atomic.h"  // class Atomic
#include "..\complexmessages2.h" // functions to manage "coded" messages between components
//#include "NetGraph.h"
//#include "IntSet.h"
#include<set>
#include<map>
#include<queue>

//#include "EventQueue.h"
//#include <queue> //STL for queue template class
//#include <set>
/** declarations **/
class PeerGnutella: public Atomic
{
public:


	PeerGnutella( const string &name = "PeerGnutella Router" ) ;	 // Default constructor

	~PeerGnutella();					// Destructor

	virtual string className() const
		{return "PeerGnutella Router";};


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
	Port &q_out;


	// stores the set of peer nodes visited by a particular message (identified by the float key)
	map<long,set<long> > routingTable;

	queue<long long> QueryQueue; // Queue that holds all the queries about to be sent to connections;
	set<int> connectionlist; //set of connection the peer has


};	// class PeerGnutella


#endif    //__GTELLA_H
