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

/** include files **/
#include <math.h>            // fabs( ... )
#include <stdlib.h>
#include "randlib.h"         // Random numbers library
#include "dispatcher.h"  // base header
#include "message.h"       // InternalMessage ....
//#include "..\..\lib\graph-c++.h"        // class graph
//#include "NetGraph.h"        // basic undirected graph with integers
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )



/*******************************************************************
* Function Name: Demux
* Description: constructor
********************************************************************/
Dispatcher::Dispatcher( const string &name )
: Atomic( name )
, msg_in( addInputPort( "msg_in" ) )
, peer_id( addOutputPort( "peer_id" ) )
, query_id( addOutputPort( "query_id" ) )

{
	//initialising these values... not indispensable but always safer
	id_counter=0;
	nextOutput_p= 0;
	nextOutput_q=0;

}

/*******************************************************************
* Function Name: externalFunction
* Description: the router gets input from either the "outside" (a new messgae to route) or from the router (next step for routing)
********************************************************************/
Model &Dispatcher::externalFunction( const ExternalMessage &msg ){
	if(VERBOSE) cout<<"external message: "<<msg.value()<<endl;
	if ( this->state() == passive)
	{
		if (msg.port() == msg_in) //putting something in the routing table
		{
			//expecting a message including the peerid and a messageId encoded according to the spec of "complexmessages.h"

			int peer = getPeerId(msg.value());// get originating peer (from value of external msg)
			if(VERBOSE)cout<<" peer :"<<peer<<endl;
			int qq = getMessageId(msg.value()); // get query (encoded as messageid)
			if(VERBOSE)cout<<"  query :"<<qq<<endl;

			//generate new message id
			int msgid;
			msgid = id_counter;
			id_counter = (id_counter + 1)%MAXID; //count

			nextOutput_q = buildMessage(msgid, qq);
			nextOutput_p = buildMessage(msgid, peer);

		}


	} //end if state is passive
	else{
		cout<<"Dispatcher error: message received while in active state"<<endl;
	}
	// we have an instantaneous change back to the passive state (will output the next output values where relevant)
	holdIn( active, Time(0.00f));

	return *this ;
}


/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &Dispatcher::internalFunction( const InternalMessage & ){


	passivate(); // we just passivate immediately

	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &Dispatcher::outputFunction( const InternalMessage &msg )
{
		sendOutput( msg.time(), peer_id, nextOutput_p);
		sendOutput( msg.time(), query_id, nextOutput_q);

	return *this;
}

Dispatcher::~Dispatcher()
{
	//do nothing
}
