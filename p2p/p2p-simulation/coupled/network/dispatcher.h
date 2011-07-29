/*******************************************************************
*
*  DESCRIPTION: Atomic Model : Dispatcher : gets a query from a peer message on its single input,
*  generates a message id, and outputs a message encoding the peer + message id on one port, and
*  a message encoding the query + message id on the other output port
*
*  AUTHOR: Alan
*
*
*******************************************************************/
/*handle duplicate inclusion of this header*/
#ifndef __DISP_H
#define __DISP_H
#define VERBOSE true
#define MAXID 100

/** include files **/
#include "atomic.h"  // class Atomic
#include "complexmessages.h" // functions to manage "coded" messages between components
//#include "NetGraph.h"
//#include "IntSet.h"

#include<map>

//#include "EventQueue.h"
//#include <queue> //STL for queue template class
//#include <set>
/** declarations **/
class Dispatcher: public Atomic
{
public:


	Dispatcher( const string &name = "Dispatcher" ) ;	 // Default constructor

	~Dispatcher();					// Destructor

	virtual string className() const
		{return "Dispatcher";};


protected:
	Model &initFunction()
		{
		return *this;
		};

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:

    const Port &msg_in ; // for getting messages input from peers : peer+query

	Port &peer_id ; // outputting peer + messageid
	Port &query_id ; // outputting query + messageid

	int id_counter; // keeping a count to generate the next message id
	int nextOutput_p; // next output
	int nextOutput_q; // next output

	//map<int, Time> a time stamp for the last seen message for a given id : the idea is that after a while we're not routing the message anymore and can forget about which nodes the message has visited.

	//Time lastpurge // stores the time of the last purge. If this occured more than <purgeinterval> ago we should re-purge the routingtable of old messages


};	// class Dispatcher


#endif    //__DISP_H
