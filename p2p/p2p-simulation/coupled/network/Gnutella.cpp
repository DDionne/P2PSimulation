/*******************************************************************
*
*  DESCRIPTION: Atomic Model : Gnutella Router
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
#include "Gnutella.h"  // base header
#include "message.h"       // InternalMessage ....
//#include "..\..\lib\graph-c++.h"        // class graph
//#include "NetGraph.h"        // basic undirected graph with integers
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )



/*******************************************************************
* Function Name: Gnutella
* Description: constructor
********************************************************************/
Gnutella::Gnutella( const string &name )
: Atomic( name )
, route_in( addInputPort( "route_in" ) )
, route_out( addOutputPort( "route_out" ) )
, in_n( addInputPort( "in_n" ) )
, out_n( addOutputPort( "out_n" ) )

{
	//initialising these values... not indispensable but always useful
	routing = false;
	hitting = false;
	nextOutputDB = 0.0f;
	nextOutputR= 0.0f;
}

/*
//external function with simultaneous messages
Model &Gnutella::externalFunction( const ExternalMessage &msg ){

}*/


/*******************************************************************
* Function Name: externalFunction
* Description: the router gets input from either the "outside" (a new messgae to route) or from the router (next step for routing)
********************************************************************/
Model &Gnutella::externalFunction( const ExternalMessage &msg ){

	if ( this->state() == passive)
	{
		if (msg.port() == route_in) //new message to route
		{
			//expecting float values looking like 6,123 meaning route from peer 6 with msg id 123 (id<1000)
			//get the peerid, message id, generate new TTL, then put in "to output" variable // not a list ! [id, TTL, peer]
			if(VERBOSE) cout<<"Gnutella : new routing message : "<<msg.value()<<endl;
			int peerid = getPeerId(msg.value());// get originating peer (from value of external msg)
			if(VERBOSE)cout<<" peerid:"<<peerid<<endl;
			int id = getMessageId(msg.value()); // get message id
			if(VERBOSE)cout<<"  message id:"<<id<<endl;



			//create "seen" list. The message ids are float values (that way we can generate them using a random function)
			// to create an empty list I use the default constructor, throught the shortcut of calling the [] operator.
			routingTable[id]; //creates empty set mapped to id
			routingTable[id].insert(peerid); // the new peer has now been visited (because the first thing will be to send himself the message !)

			// we need to propagate this message to the DB and route it in the network (separate issues)
			hitting = true;
			routing = true;
			//set the output values
			// from right, digits 1-3 are id, digits 4-6 are peerid, digit 7 is TTL exampel value : 6005123 = TTL=6, peerid 5, id = 123
			nextOutputDB =  buildMessage(id, peerid);
			nextOutputR =buildMessage(id, peerid, STANDARDTTL); //standardTTL is a constant defined in the h, should be a model parameter

			//if(VERBOSE) cout<<"next output:"<<nextOutputR<<endl;

		}

		else if (msg.port() ==  in_n){
			//expecting value = TTL * 100 + peerid + id, where id is the decimal part (<1)
			if(VERBOSE) cout<<"Gnutella : message from the network routing loop... "<<msg.value()<<"\n";

			long inval = msg.value();

			//extract our 3 values using static functions from complexmessages.h
			int OldTTL = getTTL(inval);
			//if(VERBOSE)cout<<"old ttl:"<<OldTTL<<endl;
			int peerid = getPeerId(inval);
			//if(VERBOSE)cout<<" peerid:"<<peerid<<endl;
			int id = getMessageId(inval);
			//if(VERBOSE)cout<<"  message id:"<<id<<endl;

			//check for already visited peer : search for "peerid" in set mapped to id in routing table
			set<long>::iterator finder = routingTable[id].find(peerid);
			if(finder==routingTable[id].end()){ //it's NOT in there
				//the unseen peer must get the message
				if(VERBOSE)cout<<" peerid:"<<peerid<<"unseen by msg id ="<<id<< endl;
				hitting = true;
				nextOutputDB = buildMessage(id, peerid); // we don't put in a TTL
				// the new peer has now been visited : we add him to the set of seen peers
				routingTable[id].insert(peerid);

				if(OldTTL>0){ // the unseen peer will also propagate the message because it still has some TTL
					routing=true;
					nextOutputR = buildMessage(id, peerid, OldTTL-1); //add the TTL for the routing msg
					if(VERBOSE)cout<<" message will be re-cycled TTL >0 "<< endl;
				}
				else {
					routing = false;
				}
			} else {
				if(VERBOSE)cout<<" peerid:"<<peerid<<"already visited by msg id ="<<id<< endl;
				hitting = false;
				routing = false;
			}

		}

	} //end if state is passive
	else{
		cout<<"error: message received while in active state"<<endl;
	}
	// we have an instantaneous change back to the passive state (will output the next output values where relevant)
	holdIn( active, Time(0.01f));

	return *this ;
}


/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &Gnutella::internalFunction( const InternalMessage & ){

	//set these back
	routing = false;
	hitting = false;
	passivate(); // we just passivate immediately

	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &Gnutella::outputFunction( const InternalMessage &msg )
{

	if (routing) // if we have something to output
        {
	      sendOutput( msg.time(), out_n, nextOutputR); //nextOutput contains the next value to output
	      if(VERBOSE) cout<<"Gnutella : routing to LTS ..."<<nextOutputR<<endl;
        }
	if(hitting)
	{
        sendOutput( msg.time(), route_out, nextOutputDB); //nextOutput contains the next value to output

	}
	return *this;
}

Gnutella::~Gnutella()
{
	//do nothing
}
