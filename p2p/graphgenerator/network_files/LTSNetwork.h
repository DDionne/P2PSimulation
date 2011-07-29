/*******************************************************************
*
*  DESCRIPTION: Atomic Model : Network
*  Modeled as a labeled transition system.
*
*  AUTHOR: Alan
*
*
*******************************************************************/
/*handle duplicate inclusion of this header*/
#ifndef __NETWORK_H
#define __NETWORK_H
#define VERBOSE false
/** include files **/
#include "atomic.h"  // class Atomic
#include "NetworkEvent.h"
#include "..\..\lib\graph-c++.h" // classes to handle the graph (network)
#include "NetGraph.h"
#include "..\complexmessages.h" // static functions to manipulate "coded" messages in a consistent way across components

//#include "EventQueue.h"
#include <queue> //STL for queue template class
//#include <set>
/** declarations **/
class LTSNetwork: public Atomic
{
public:


	LTSNetwork( const string &name = "Network" ) ;	 // Default constructor

	~LTSNetwork();					// Destructor

	virtual string className() const
	{return "Network";};



	//a queue of networks events as defined in NetworkEvent.h [just a struct], using the Standard Template Library
	//for now no real use in defining this...
	class EventQueue : public queue<networkevent> {
	public :
		EventQueue(){

		}
	};

	protected:
		Model &initFunction()
		{
			return *this;
		};

		Model &externalFunction( const ExternalMessage & );

		Model &internalFunction( const InternalMessage & );

		Model &outputFunction( const InternalMessage & );

	private:
		const Port &peer_online ;
		const Port &peer_offline ;
		const Port &peer_connect ;
		const Port &peer_disconnect ;
		const Port &inroute ;
		Port &route_out ;
		Port &out_connect;
		Port &out_disconnect;

		bool routing; // test
		int messageval;
		GraphInt* thegraph;

		EventQueue EvQ; //queue of events (for now time / id are not used)
		queue<int> DisconnectionQueue; // list of messages to notify peers of disconnections
		queue<int> ConnectionQueue; // list of messages to notify peers of new connections being successful

		//visitor thevisitor; //todo : at some point we may want a list of visitors


};	// class LTSNetwork


#endif    //__NETWORK_H
