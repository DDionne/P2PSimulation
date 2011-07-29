/*******************************************************************
*
*  DESCRIPTION: Atomic Model : Demultiplexer : gets a message on its single input and
*  forwards it to one of its numerous outputs according to a routing table.
*
*  AUTHOR: Alan
*
*
*******************************************************************/
/*handle duplicate inclusion of this header*/
#ifndef __DEMUX_H
#define __DEMUX_H
#define VERBOSE true
#define MAXID 100

/** include files **/
#include "atomic.h"  // class Atomic
#include "..\complexmessages2.h" // functions to manage "coded" messages between components
//#include "NetGraph.h"
//#include "IntSet.h"

#include <map>
#include <set>

//#include "EventQueue.h"
//#include <queue> //STL for queue template class
//#include <set>
/** declarations **/
class Demux: public Atomic
{
public:


	Demux( const string &name = "Demux" ) ;	 // Default constructor

	~Demux();					// Destructor

	virtual string className() const
		{return "Demux";};


protected:
	Model &initFunction()
		{
		return *this;
		};

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:

    const Port &msg_in ; // for getting messages
	const Port &table_in; // for updating the routing table
	const Port &connect_in; // for connection / disconnection messages

	Port &out_1 ; // for sending out the messages to peer 1 // this needs to  be generated through a script
	Port &out_2 ; // for sending out the messages to peer 2 // this needs to  be generated through a script
	Port &out_3 ; // for sending out the messages to peer 3 // this needs to  be generated through a script
	Port &out_4 ; // for sending out the messages to peer 4 // this needs to  be generated through a script
	Port &out_5 ; // for sending out the messages to peer 5 // this needs to  be generated through a script
	Port &out_6 ; // for sending out the messages to peer 5 // this needs to  be generated through a script
	Port &out_7 ; // for sending out the messages to peer 5 // this needs to  be generated through a script
	Port &out_8 ; // for sending out the messages to peer 5 // this needs to  be generated through a script
	Port &out_9 ; // for sending out the messages to peer 5 // this needs to  be generated through a script
	Port &out_10 ; // for sending out the messages to peer 5 // this needs to  be generated through a script
//..Port &out_k

	// table to associate message Ids to peers (conceptually for routing responses back to the peer that originated the query).
	map<int,set<int> > routingTable;

	bool routing; // if we're routing a queryhit
	bool connecting; // if we're routing a connect/ disconnect
	set<int> to_peers; //the set of peers that the next output is meant for
	int nextOutput; // next output
	int id_counter;

	//map<float, Time> a time stamp for the last seen message for a given id : the idea is that after a while we're not routing the message anymore and can forget about which nodes the message has visited.

	//Time lastpurge // stores the time of the last purge. If this occured more than <purgeinterval> ago we should re-purge the routingtable of old messages



};	// class Demux


#endif    //__DEMUX_H
