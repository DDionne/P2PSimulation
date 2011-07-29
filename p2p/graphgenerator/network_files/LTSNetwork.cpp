/*******************************************************************
*
*  DESCRIPTION: Atomic Model : Network (LTS system)
*
*  AUTHOR: Alan
*
*
*  DATE: November 2010
*
*******************************************************************/

/** include files **/
#include <math.h>            // fabs( ... )
#include <stdlib.h>
#include "randlib.h"         // Random numbers library
#include "LTSNetwork.h"  // base header
#include "message.h"       // InternalMessage ....
//#include "..\..\lib\graph-c++.h"        // class graph
//#include "NetGraph.h"        // basic undirected graph with integers
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )



/*******************************************************************
* Function Name: LTSNetwork
* Description: constructor
********************************************************************/
LTSNetwork::LTSNetwork( const string &name )
: Atomic( name )
, peer_online( addInputPort( "peer_online" ) )
, peer_offline( addInputPort( "peer_offline" ) )
, peer_connect( addInputPort( "peer_connect" ) )
, peer_disconnect( addInputPort( "peer_disconnect" ) )
, inroute( addInputPort( "inroute" ) )
, route_out( addOutputPort( "route_out" ) )
, out_connect( addOutputPort( "out_connect" ) )
, out_disconnect( addOutputPort( "out_disconnect" ) )

{
	//create a new Graph. Nothing else... ?
	thegraph= new GraphInt();


}

/*******************************************************************
* Function Name: externalFunction
* Description: the Network gets input from outside
********************************************************************/
Model &LTSNetwork::externalFunction( const ExternalMessage &msg ){

	if (msg.port() == peer_online)
	{

		thegraph->online(msg.value());   //adds a node to the graph with the given value
		if(VERBOSE) cout<<"node "<<msg.value()<<" inserted\n";
		//holdIn( active, Time(0.00f));
	}
	else if (msg.port() == peer_offline){
		int inpeer = msg.value();

		//get all the connected nodes a disconnect them, plus let them know they've been disconnected
		set<int> connected = thegraph->getConnectedNodes(inpeer);
		set<int>::iterator sit;
		for ( sit=connected.begin() ; sit != connected.end(); sit++ ){
			thegraph->disconnect(inpeer, *sit); //disconnect them
			DisconnectionQueue.push(buildMessage(*sit, inpeer)); //enqueue a message saying peer "inpeer" disconnects from "*sit"
		}

		thegraph->offline(inpeer);

		if(VERBOSE) cout<<"node "<<msg.value()<<" removed\n";
		//holdIn( active, Time(0.00f));
	}
	else if (msg.port() == peer_connect){
		int twonumbers, from, to;
		twonumbers = msg.value();
		from = getPeerId(twonumbers); //first and second field encoding of the peers
		to = getMessageId(twonumbers);
		if(VERBOSE) cout<<"connecting "<<from<<" to "<< to<<"\n";

		if(thegraph->connect(from,to))
			ConnectionQueue.push(buildMessage(to, from, 1)); // enqueue a connection message : adding the TTL makes it different from a disconnect message, further down the road

		//holdIn( active, Time(0.00f));
	}
	else if (msg.port() == peer_disconnect){
		int twonumbers, from, to;
		twonumbers = msg.value();
		from = getPeerId(twonumbers); //first and second field encoding of the peers
		to = getMessageId(twonumbers);
		if(VERBOSE) cout<<"disconnecting "<<from<<" and "<< to<<"\n";

		if(thegraph->disconnect(from, to))
			DisconnectionQueue.push(twonumbers); // enqueue the original message to be re-output as confirmation that connection took place

		//holdIn( active, Time(0.00f));
	}
	else if (msg.port() == inroute){
		//routing=true;
		int inpeer, TTL, messageId;
		inpeer = getPeerId(msg.value());
		TTL= getTTL(msg.value());
		messageId = getMessageId(msg.value());

		if(VERBOSE) cout<<"about to route a message from"<<inpeer<<"\n";

		//get all the connected nodes and enqueue the "arrival of the message" event for all these new nodes
		//find the nodes connected to this one
		set<int> connected = thegraph->getConnectedNodes(inpeer);

		//if(VERBOSE) cout<<"loop for enqueuing nodes :"<<connected.size()<<" nodes to enqueue";
		set<int>::iterator sit;

		//if(VERBOSE) cout << "connected nodes contains:";
		for ( sit=connected.begin() ; sit != connected.end(); sit++ ){
			//    if(VERBOSE) cout<<"bang!  "<<*sit<<"\n";
			EvQ.push(makeNetworkEvent(messageId, *sit, TTL, 0.0f)); //enqueue a network event with the "*sit" peer (the other parts are not used for now)

			//holdIn( active, Time(0.01f));
		}
	}

	// TEST : no external transition unless we're passive
	if (this->state()==passive){
		holdIn( active, Time(0,0,0,120)); //wait 120ms before doing something
	}

	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &LTSNetwork::internalFunction( const InternalMessage & ){

	if (!EvQ.empty() || !DisconnectionQueue.empty() || !ConnectionQueue.empty()) {
		// if any of the queues are not empty
	    	holdIn( active, Time(0,0,0,10)); // we wait 10ms to dequeue
	    	// that is, we only have a useless fixed timing between two messages getting through the network
	    } else {
	    	passivate(); // we just passivate immediately
	    }


	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &LTSNetwork::outputFunction( const InternalMessage &msg )
{
	if(VERBOSE) cout<<"LTS: output coming...\n";
	if ( !EvQ.empty() ) // if we have messages to dequeue
        {
		long message = buildMessage(EvQ.front().id, EvQ.front().peerid, EvQ.front().TTL);

		sendOutput( msg.time(), route_out, message);

        EvQ.pop(); //remove val from queue

        }
	if ( !ConnectionQueue.empty() ) // if we have connect messages to dequeue
        {
         sendOutput( msg.time(), out_connect, ConnectionQueue.front());
         ConnectionQueue.pop(); //remove val from queue
        }
	if ( !DisconnectionQueue.empty() ) // if we have disconnect messages to dequeue
        {
         sendOutput( msg.time(), out_disconnect, DisconnectionQueue.front());
         DisconnectionQueue.pop(); //remove val from queue
        }

	return *this;
}

LTSNetwork::~LTSNetwork()
{
	delete thegraph;
}
