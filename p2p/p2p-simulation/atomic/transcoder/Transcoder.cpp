/*******************************************************************
*
*  DESCRIPTION: Atomic Model : Transcoder : encodes / decodes messages between network and peer innards.
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
, connect1( addInputPort( "connect1" ) )
, inconnect1( addInputPort( "inconnect1" ) )
, disconnect1( addInputPort( "disconnect1" ) )
, indisconnect1( addInputPort( "indisconnect1" ) )
, online1( addInputPort( "online1" ) )
, offline1( addInputPort( "offline1" ) )
, query1( addInputPort( "query1" ) )
, publish1( addInputPort( "publish1" ) )
, remove1( addInputPort( "remove1" ) )

, connect2( addOutputPort( "connect2" ) )
, inconnect2( addOutputPort( "inconnect2" ) )
, disconnect2( addOutputPort( "disconnect2" ) )
, indisconnect2( addOutputPort( "indisconnect2" ) )
, online2( addOutputPort( "online2" ) )
, offline2( addOutputPort( "offline2" ) )
, query2( addOutputPort( "query2" ) )
, publish2( addOutputPort( "publish2" ) )
, remove2( addOutputPort( "remove2" ) )

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

	if (msg.port() == connect1) //putting something in the routing table
		{
		next_port_out = connect2;
		nextOutput = msg.value()
		}
		else


			//expecting a message including the peerid and a messageId encoded according to the spec of "complexmessages.h"

			int peerid = getPeerId(msg.value());// get originating peer (from value of external msg)
			if(VERBOSE)cout<<" peerid:"<<peerid<<endl;
			int id = getMessageId(msg.value()); // get message id
			if(VERBOSE)cout<<"  message id:"<<id<<endl;
		}

		else if (msg.port() ==  msg_in){
			//expecting value = TTL * 100 + peerid + id, where id is the decimal part (<1)
			if(VERBOSE) cout<<"message to be demultiplexed... "<<msg.value()<<"\n";

			long inval = msg.value();

			//extract our 3 values using static functions from complexmessages.h
			//HACK : the msg payload will be encoded in the space meant for the peerId
			int payload = getPeerId(inval);
			if(VERBOSE)cout<<" message payload:"<<payload<<endl;
			int id = getMessageId(inval);
			if(VERBOSE)cout<<"  message id:"<<id<<endl;

			//check for already visited peer : search for "peerid" in set mapped to id in routing table
			to_peer = routingTable[id]; // get the peer that the message id points to
			//TODO : what does this return if the msgid is not in the table ?
			routing = true;
			nextOutput = buildMessage(id, payload); // put the payload back in the message in place of the peerid
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
		//NextOutput contains the next message to be output, to_peer the integer identifying the peer that the message is for
		switch ( to_peer ) {
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


        }
	return *this;
}

Demux::~Demux()
{
	//do nothing
}
