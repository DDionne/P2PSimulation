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
	//create a new Graph.
	thegraph= new GraphInt();

	currenttimefloat=0; // start the timer at 0.

	//get probability distribution for the network delay
	try
		{
			dist = Distribution::create( MainSimulator::Instance().getParameter( description(), "distribution" ) );

			MASSERT( dist );

			for ( register int i = 0 ; i < dist->varCount() ; i++ )
			{
				string parameter( MainSimulator::Instance().getParameter( description(), dist->getVar(i) ) ) ;
				dist->setVar( i, str2float( parameter ) ) ;
			}
		} catch( InvalidDistribution &e )
		{
			e.addText( "The model " + description() + " has distribution problems!" ) ;
			e.print(cerr);
			MTHROW( e ) ;
		} catch( MException &e )
		{
			MTHROW( e ) ;
		}

}

Model &LTSNetwork::initFunction()
{
passivate(); // a first change just to make sure that we start the clock right.
return *this;
}

/*******************************************************************
* Function Name: externalFunction
* Description: the Network gets input from outside
********************************************************************/
Model &LTSNetwork::externalFunction( const ExternalMessage &msg ){
		//advance my internal time
	if(VERBOSE) cout<<"External Transition :"<<endl<<"time: "<<msg.time().asMsecs()<<endl;
	currenttimefloat = msg.time().asMsecs();

	if (msg.port() == peer_online)
	{
		thegraph->online(msg.value());   //adds a node to the graph with the given value
		if(VERBOSE) cout<<"node "<<msg.value()<<" inserted\n";

	}
	else if (msg.port() == peer_offline){
		int inpeer = msg.value();

		//get all the connected nodes a disconnect them, plus let them know they've been disconnected
		set<int> connected = thegraph->getConnectedNodes(inpeer);
		set<int>::iterator sit;
		int count = 0;
		for ( sit=connected.begin() ; sit != connected.end(); sit++ ){
			count++;
			thegraph->disconnect(inpeer, *sit); //disconnect them
			EvQ.push(makeDisConnectEvent(inpeer, *sit,currenttimefloat+count)); // enqueue messages to be output with just 1 millisecond delay
			//DisconnectionQueue.push(buildMessage(*sit, inpeer)); //enqueue a message saying peer "inpeer" disconnects from "*sit"
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
			EvQ.push(makeConnectEvent(from, to,currenttimefloat+1)); // enqueue message to be output with just 1 millisecond delay
			//ConnectionQueue.push(buildMessage(to, from, 1)); // enqueue a connection message : adding the TTL makes it different from a disconnect message, further down the road

		//holdIn( active, Time(0.00f));
	}
	else if (msg.port() == peer_disconnect){
		int twonumbers, from, to;
		twonumbers = msg.value();
		from = getPeerId(twonumbers); //first and second field encoding of the peers
		to = getMessageId(twonumbers);
		if(VERBOSE) cout<<"disconnecting "<<from<<" and "<< to<<"\n";

		if(thegraph->disconnect(from, to))
			EvQ.push(makeDisConnectEvent(from, to,currenttimefloat+1));//DisconnectionQueue.push(twonumbers); // enqueue the original message to be re-output as confirmation that connection took place

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

			//generate a random network delay:
			float delay = static_cast<float>(distribution().get());
			if(VERBOSE) cout<<"enqueueing event for time = "<<currenttimefloat +delay*1000<<"\n";  //delay in milliseconds !
			EvQ.push(makeNetworkEvent(messageId, *sit, TTL, currenttimefloat + (delay*1000))); //enqueue a network event with the "*sit" peer (the other parts are not used for now)


			//holdIn( active, Time(0.01f));
		}

	}



	if (EvQ.empty()){ // this transition didn't enqueue anything
		if(VERBOSE) cout<<"LTS:Event queue is empty. Passivating."<<endl;
		passivate();
	} else {
	//calculate time until next scheduled change :

	float remainingdelay = EvQ.top().time - currenttimefloat; //EvQ.top() is the next NetworkEvent to be output

	if(VERBOSE) cout<<"next change at (ms):"<<EvQ.top().time<<"time until next change (s):"<<remainingdelay/1000<<endl;
	// hold until then
	holdIn(active, Time(remainingdelay/1000)); // time remaining until next scheduled change
	//then it will be time for an internal transition...
	}

	/*/ TEST : no external transition unless we're passive
	if (this->state()==passive){
		holdIn( active, Time(0,0,0,120)); //wait 120ms before doing something
	}*/

	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &LTSNetwork::internalFunction( const InternalMessage & msg){

	if(VERBOSE) cout<<"Internal Transition :"<<endl<<"time: "<<msg.time().asMsecs()<<endl;
		currenttimefloat = msg.time().asMsecs();//currenttimefloat + lastChange().asMsecs();

		//hack - test
		//lastChange(msg.time());
		//if(VERBOSE) cout<<"currenttime now:"<<currenttimefloat<<endl;

	if (!EvQ.empty()){
		//|| !DisconnectionQueue.empty() || !ConnectionQueue.empty()) {
		// if any of the queues are not empty

		//advance my internal time

			//calculate time until next scheduled change :
			float remainingdelay = EvQ.top().time - currenttimefloat; //EvQ.top() is the next NetworkEvent to be output

			// hold until then
			holdIn(active, Time(remainingdelay/1000)); // time remaining until next scheduled change

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

	if ( !EvQ.empty() ) // if we have messages to dequeue
        {
		if(VERBOSE) cout<<"LTS: output coming...\n";

		long message = buildMessage(EvQ.top().id, EvQ.top().peerid, EvQ.top().TTL);

		if(EvQ.top().connectEvent) //distinguish the three types of events
			if (EvQ.top().TTL==1)
				sendOutput( msg.time(), out_connect, message); //connect notification
			else
				sendOutput( msg.time(), out_disconnect, message); // disconnect notification
		else
			sendOutput( msg.time(), route_out, message); // message routing

        EvQ.pop(); //remove val from queue

        }
/*	if ( !ConnectionQueue.empty() ) // if we have connect messages to dequeue
        {
         sendOutput( msg.time(), out_connect, ConnectionQueue.front());
         ConnectionQueue.pop(); //remove val from queue
        }
	if ( !DisconnectionQueue.empty() ) // if we have disconnect messages to dequeue
        {
         sendOutput( msg.time(), out_disconnect, DisconnectionQueue.front());
         DisconnectionQueue.pop(); //remove val from queue
        }*/

	return *this;
}

LTSNetwork::~LTSNetwork()
{
	delete thegraph;
}
