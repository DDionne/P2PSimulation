/*******************************************************************
*
*  DESCRIPTION: Atomic Model :  Gnutella Router.
*  Simulates the Gnutella protocol to route messages through a network
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
#include "complexmessages.h" // functions to manage "coded" messages between components
//#include "NetGraph.h"
//#include "IntSet.h"
#include<set>
#include<map>

//#include "EventQueue.h"
//#include <queue> //STL for queue template class
//#include <set>
/** declarations **/
class Gnutella: public Atomic
{
public:


	Gnutella( const string &name = "Gnutella Router" ) ;	 // Default constructor

	~Gnutella();					// Destructor

	virtual string className() const
		{return "Gnutella Router";};


protected:
	Model &initFunction()
		{
		return *this;
		};

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:

    const Port &route_in ;
	const Port &in_n;
    //const Port &in_id;
	Port &out_n ; // for sending out network messages
	Port &route_out ;
	//Port &out_id;


	// stores the set of peer nodes visited by a particular message (identified by the float key)
	map<long,set<long> > routingTable;

	bool routing; //a boolean indicating if there is anything to output towards the network (route)
	bool hitting;// a boolean indicating whether we've got a "hit" (a message has reached a new peer)
	long nextOutputR; // this is a float representation of the next message that will be output to the router (depending if the above "routing" is true /false)
	long nextOutputDB; // next output to the DB

	//map<float, Time> a time stamp for the last seen message for a given id : the idea is that after a while we're not routing the message anymore and can forget about which nodes the message has visited.

	//Time lastpurge // stores the time of the last purge. If this occured more than <purgeinterval> ago we should re-purge the routingtable of old messages



};	// class Gnutella


#endif    //__GTELLA_H
