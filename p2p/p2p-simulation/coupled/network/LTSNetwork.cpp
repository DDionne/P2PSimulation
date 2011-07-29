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
, in_id( addInputPort( "in_id" ) )
, out_id( addOutputPort( "out_id" ) )

{
	//create a new Graph. Nothing else... ?
	thegraph= new GraphInt();


}

/*******************************************************************
* Function Name: externalFunction
* Description: the Network gets input from outside
********************************************************************/
Model &LTSNetwork::externalFunction( const ExternalMessage &msg ){
	//if(VERBOSE) cout<<"coucou\n";
 // if ( this->state() == passive) // in fact all this should happen whatever the state ! TODO !!
 // {
    if (msg.port() == peer_online)
     {

       thegraph->online(msg.value());   //adds a node to the graph with the given value
       if(VERBOSE) cout<<"node "<<msg.value()<<" inserted\n";
       //holdIn( active, Time(0.00f));
     }
    else if (msg.port() == peer_offline){
    	thegraph->offline(msg.value());

    	if(VERBOSE) cout<<"node "<<msg.value()<<" removed\n";
    	//holdIn( active, Time(0.00f));
    }
    else if (msg.port() == peer_connect){
        	int twonumbers, from, to;
        	twonumbers = msg.value();
        	from = floor(twonumbers /1000);
        	to = twonumbers%1000;
        	if(VERBOSE) cout<<"connecting "<<from<<" to "<< to<<"\n";

        	thegraph->connect(from,to);

        	//holdIn( active, Time(0.00f));
        }
    else if (msg.port() == peer_disconnect){
            	int twonumbers, from, to;
            	twonumbers = msg.value();
            	from = floor(twonumbers /1000);
            	to = twonumbers%1000;
            	if(VERBOSE) cout<<"disconnecting "<<from<<" and "<< to<<"\n";

            	thegraph->disconnect(from, to);

            	//holdIn( active, Time(0.00f));
            }
    else if (msg.port() == inroute){
    	//routing=true;
    	int inpeer, TTL, messageId;
    	inpeer = getPeerId(msg.value());
    	TTL= getTTL(msg.value());
    	messageId = getMessageId(msg.value());

    	if(VERBOSE) cout<<"LTS --- about to route a message from "<<inpeer<<"\n";

    	//get all the connected nodes and enqueue the "arrival of the message" event for all these new nodes
    	//find the nodes connected to this one
    	cout<<"LTS --- about to get Connected peers  : \n";

    	/*thegraph->getConnectedNodes(inpeer);
    	cout<<"getConnectedpeers (1) passed\n about to declare another intbag\n";*/
    	set<int> connected;
    	//cout<<"ok--Intbag Declared----------------\nabout to get connected peers again";
    	connected = thegraph->getConnectedNodes(inpeer);
    	//cout<<"ok--after connected peers----------------\n";


    	if(VERBOSE) cout<<"LTS --- loop for enqueuing nodes :"<<connected.size()<<" nodes to enqueue\n";
    	set<int>::iterator sit;

    	  if(VERBOSE) cout << "LTS --- connected nodes contains:";
    	  for ( sit=connected.begin() ; sit != connected.end(); sit++ ){
    	    if(VERBOSE) cout<<*sit<<";";
    		EvQ.push(makeNetworkEvent(messageId, *sit, TTL, 0.0f)); //enqueue a network event with the "*sit" peer (the other parts are not used for now)

    	//holdIn( active, Time(0.01f));
			}
    	  if(VERBOSE) cout<<endl;

  //}

  if (!EvQ.empty()) { // if we were or now are in the process of routing messages

    	holdIn( active, Time(0.03f)); // we wait 0.03s to dequeue

   } else {
    	holdIn( active, Time(0.00f)); // we just passivate immediately
   }



	return *this ;
 }
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &LTSNetwork::internalFunction( const InternalMessage & ){


	   if (!EvQ.empty()) { // if we were or now are in the process of routing messages
	    	holdIn( active, Time(0.01f)); // we wait 0.01s to dequeue
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
	//if(VERBOSE) cout<<"output coming...\n";
	if ( !EvQ.empty() ) // if we have messages to dequeue
        {
		long message = buildMessage(EvQ.front().id, EvQ.front().peerid, EvQ.front().TTL);
		 //if(VERBOSE) cout<<"outpupu\n";
         sendOutput( msg.time(), route_out, message);
        // if(VERBOSE) cout<<"outpopu\n";

        // if(VERBOSE) cout<<"output done !! "<<message<<endl;
         EvQ.pop(); //remove val from queue

        }
      else
        {
    	 // sendOutput( msg.time(), route_out, 999 );
    	  //(no output for graph construction)
    	}
	return *this;
}

LTSNetwork::~LTSNetwork()
{
	delete thegraph;
	//delete EvQ; not a pointer, doesn't need to be deleted manually
}
