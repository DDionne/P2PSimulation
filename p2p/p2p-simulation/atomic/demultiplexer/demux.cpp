/*******************************************************************
*
*  DESCRIPTION: Atomic Model : Demultiplexer : gets a message on its single input and
*  forwards it to one of its numerous outputs according to a routing table.
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
#include "demux.h"  // base header
#include "message.h"       // InternalMessage ....
//#include "..\..\lib\graph-c++.h"        // class graph
//#include "NetGraph.h"        // basic undirected graph with integers
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )



/*******************************************************************
* Function Name: Demux
* Description: constructor
********************************************************************/
Demux::Demux( const string &name )
: Atomic( name )
, msg_in( addInputPort( "msg_in" ) )
, table_in( addInputPort( "table_in" ) )
, connect_in( addInputPort( "connect_in" ) )
, out_1( addOutputPort( "out_1" ) )
, out_2( addOutputPort( "out_2" ) )
, out_3( addOutputPort( "out_3" ) )
, out_4( addOutputPort( "out_4" ) )
, out_5( addOutputPort( "out_5" ) )
, out_c1( addOutputPort( "out_c1" ) )
, out_c2( addOutputPort( "out_c2" ) )
, out_c3( addOutputPort( "out_c3" ) )
, out_c4( addOutputPort( "out_c4" ) )
, out_c5( addOutputPort( "out_c5" ) )

{
	//initialising these values... not indispensable but always safer
	routing = false;
	nextOutput= 0;
}



/*******************************************************************
* Function Name: externalFunction
* Description: the router gets input from either the "outside" (a new messgae to route) or from the router (next step for routing)
********************************************************************/
Model &Demux::externalFunction( const ExternalMessage &msg ){
	if(VERBOSE) cout<<"external message: "<<msg.value()<<endl;
	if ( this->state() == passive)
	{
		if (msg.port() == table_in) //putting something in the routing table
		{
			//expecting a message including the peerid and a messageId encoded according to the spec of "complexmessages.h"

			int peerid = getPeerId(msg.value());// get originating peer (from value of external msg)
			if(VERBOSE)cout<<" peerid:"<<peerid<<endl;
			int id = getMessageId(msg.value()); // get message id
			if(VERBOSE)cout<<"  message id:"<<id<<endl;

			//create "mapping entry" list. The message ids are float values (that way we can generate them using a random function)
			// to create an empty list I use the default constructor, throught the shortcut of calling the [] operator.
			routingTable[id]=peerid; //associates peerid to id in routing table.
			if(VERBOSE)cout<<"  RoutingTable id:"<<id<<"associated with peer:"<<routingTable[id]<<endl;

		}


		else if (msg.port() ==  msg_in){
			//expecting value = TTL * 100 + peerid + id, where id is the decimal part (<1)
			if(VERBOSE) cout<<"message to be demultiplexed... "<<msg.value()<<"\n";

			long inval = msg.value();

			//extract our 3 values using static functions from complexmessages.h
			//HACK : the msg payload will be encoded in the space meant for the peerId, the originating peer in the TTL spot
			int origin = getTTL(inval);
			int payload = getPeerId(inval);
			if(VERBOSE)cout<<" message payload:"<<payload<<endl;
			int id = getMessageId(inval);
			if(VERBOSE)cout<<"  message id:"<<id<<endl;

			//search for "peerid" in set mapped to id in routing table
			map<int,int>::iterator finder =routingTable.find(id);
			if (!(finder==routingTable.end())){
			routing = true;
			to_peers.clear();
			to_peers.insert(finder->second); //finder accesses both key and value
			} else{
				cout<<"DEMUX::Error : msgid -> peer not set";
			}
			nextOutput = inval; // just spit back out the same value //buildMessage(id, payload, origin); // put the payload back in the message in place of the peerid
		}

		else if (msg.port() ==  connect_in){ // this message is a notification to peers that they were connected
			//expecting value = TTL ;  peer1id + peer2id
			if(VERBOSE) cout<<"connection message "<<msg.value()<<"\n";

			long inval = msg.value();

			//extract our 3 values using static functions from complexmessages.h
			//HACK : the msg payload will be encoded in the space meant for the peerId
			int p1 = getPeerId(inval);

			int p2 = getMessageId(inval);
			int ttl = getTTL(inval); //=1 if this is a connect message, 0 for disconnect
			//if(VERBOSE)cout<<"  message id:"<<id<<endl;

			//check for already visited peer : search for "peerid" in set mapped to id in routing table
			to_peers.clear();
			to_peers.insert(p1);
			to_peers.insert(p2);
			//TODO : what does this return if the msgid is not in the table ?
			connecting = true;
			nextOutput = buildMessage(p1, p2,ttl); // put the payload back in the message in place of the peerid
		}

	} //end if state is passive
	else{
		cout<<"error: message received while in active state"<<endl;
	}
	// we have an instantaneous change back to the passive state (will output the next output values where relevant)
	holdIn( active, Time(0.00f));

	return *this ;
}


/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &Demux::internalFunction( const InternalMessage & ){

	//set these back
	routing = false;
	connecting = false;
	passivate(); // we just passivate immediately

	return *this;
}

/*******************************************************************
 * Function Name: outputFunction
 ********************************************************************/
Model &Demux::outputFunction( const InternalMessage &msg )
{
	if(VERBOSE) cout<<"output coming...\n";
	if (routing) // if we have anything to output
	{
		set<int>::iterator it= to_peers.begin(); // there's just one
		//NextOutput contains the next message to be output, to_peer the integer identifying the peer that the message is for
		switch ( *it ) {
		case 1 :
			sendOutput( msg.time(), out_1, nextOutput);
			break;
		case 2 :
			sendOutput( msg.time(), out_2, nextOutput);
			break;
		case 3 :
			sendOutput( msg.time(), out_3, nextOutput);
			break;
		case 4 :
			sendOutput( msg.time(), out_4, nextOutput);
			break;
		case 5 :
			sendOutput( msg.time(), out_5, nextOutput);
			break;
		default :

			cout<<"error : output port `to_peer' not set"<<endl;
			//ERROR
		}


	} else if(connecting) {
		for (set<int>::iterator it= to_peers.begin();it!=to_peers.end();it++){
			//NextOutput contains the next message to be output, to_peer the integer identifying the peer that the message is for
			switch ( *it ) {
			case 1 :
				sendOutput( msg.time(), out_c1, nextOutput);
				break;
			case 2 :
				sendOutput( msg.time(), out_c2, nextOutput);
				break;
			case 3 :
				sendOutput( msg.time(), out_c3, nextOutput);
				break;
			case 4 :
				sendOutput( msg.time(), out_c4, nextOutput);
				break;
			case 5 :
				sendOutput( msg.time(), out_c5, nextOutput);
				break;
			default :

				cout<<"error : output port `to_peer' not set"<<endl;
			}//end switch
		}//end for
	}//end if
	return *this;
}

Demux::~Demux()
{
	//do nothing
}
