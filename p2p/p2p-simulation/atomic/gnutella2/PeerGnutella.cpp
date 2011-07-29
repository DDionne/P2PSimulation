/*******************************************************************
*
*  DESCRIPTION: Atomic Model : PeerGnutella (part of the peer model)
*
*  AUTHOR: Alan
*
*
*  DATE: November 2010
*
*
*  This model manages its own list of connections and uses that list to propagate queries throughout the network.
*  for example: one peer queries, that query gets sent to all his connections, then all those connections send the query to
*  their own connections and so on.
*
*******************************************************************/

/** include files **/
#include <math.h>            // fabs( ... )
#include <stdlib.h>
#include "randlib.h"         // Random numbers library
#include "PeerGnutella.h"  // base header
#include "message.h"       // InternalMessage ....
//#include "..\..\lib\graph-c++.h"        // class graph
//#include "NetGraph.h"        // basic undirected graph with integers
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )
#include <fstream>
#include <sstream>



/*******************************************************************
* Function Name: PeerGnutella
* Description: constructor
********************************************************************/
PeerGnutella::PeerGnutella( const string &name )
: Atomic( name )
//, route_in( addInputPort( "route_in" ) )
//, route_out( addOutputPort( "route_out" ) )
//, in_n( addInputPort( "in_n" ) )
//, out_n( addOutputPort( "out_n" ) )
, query( addInputPort( "query"))
, q_in( addInputPort( "q_in"))
, q_out( addOutputPort( "q_out"))
, connections( addInputPort( "connections"))
{
	//initialising these values... not indispensable but always useful





	//Load connections from file, for testing (when only using 1 randomsurfer and 1 gnutella models)
//	string param = MainSimulator::Instance().getParameter( description(), "connection" );
//
//
//	istringstream sis2;
//	sis2.str(param); // open stream to string
//
//
//	// read list of documents
//	while(true)
//	{
//		int w;
//		char coma;
//		sis2 >> w;
//		if (w==-1)
//			break; //-1 stands for an empty list
//		connectionlist.insert(w);
//		if(!sis2.eof())
//			sis2 >> coma; //separating from next value (if there is one)
//		else
//			break; //otherwise we're done
//	}
}

/*
//external function with simultaneous messages
Model &PeerGnutella::externalFunction( const ExternalMessage &msg ){

}*/


/*******************************************************************
* Function Name: externalFunction
* Description: the router gets input from either the "outside" (a new message to route) or from the router (next step for routing)
********************************************************************/
Model &PeerGnutella::externalFunction( const ExternalMessage &msg ){
	if(VERBOSE) cout<<"external message: "<<msg.value()<<endl;
	if ( this->state() == passive)
	{
		if (msg.port() == connections){ //adds or removes a peer from the connectionlist
			if(VERBOSE) cout <<"Input in port connections"<<endl;
			int peerId = getSecondField(msg.value()); //gets the Id of the peer from the message
			int TTL = getFifthField(msg.value());
			if(VERBOSE) cout << "connections TTL (1 = add, 0 = remove): "<<TTL<<endl;
			if(VERBOSE) cout << "connections PeerId: "<<peerId<<endl;
			if(TTL == 1){
				if(connectionlist.find(peerId) == connectionlist.end()){
					if(VERBOSE) cout <<msg.time()<< " adding peer "<< peerId << " to list of connections"<<endl;
					connectionlist.insert(peerId);
				}
			}
			else{
				if(connectionlist.find(peerId) != connectionlist.end()){
					if(VERBOSE) cout <<msg.time()<< " removing peer "<<peerId << "from list of connections"<<endl;
					connectionlist.erase(peerId);
				}
			}
		}

		else if (msg.port() == query) //start to propagate a query
		{
			if(VERBOSE) cout << "Input in port query"<<endl;

			int query = getFirstField(msg.value());
			if(VERBOSE) cout <<msg.time()<<" query: "<<query<<endl;

			int messageId = getSecondField(msg.value());
			if(VERBOSE) cout <<msg.time()<<" MessageId: "<<messageId<<endl;

			int myId = getThirdField(msg.value());
			if(VERBOSE) cout <<msg.time()<<" myID: "<<myId<<endl;

			int TTL = getFifthField(msg.value());
			if(VERBOSE) cout <<msg.time()<<" TTL: "<<TTL<<endl;

			routingTable[myId].insert(messageId);


			if(messageId == 0){
				routingTable[myId].clear();
				if(VERBOSE) cout <<msg.time()<< "      Clearing RoutingTables"<<endl;
			}
			routingTable[myId].insert(messageId);

			if(TTL > 0){
				set<int>::iterator connection;
				for(connection = connectionlist.begin();connection != connectionlist.end();connection++){
					if(VERBOSE) cout <<msg.time()<<" Connection: "<<*connection<<endl;
					long long tosend = buildNewMessage(query,messageId,*connection,myId,(TTL-1));
					QueryQueue.push(tosend);
					if(VERBOSE) cout <<msg.time()<<" Output: "<<tosend<<endl;
				}
			}

		}
		else if(msg.port() == q_in) //keep propagating the query
		{
			if(VERBOSE) cout << "Input in port q_in"<<endl;

			int query = getFirstField(msg.value());
			if(VERBOSE) cout <<msg.time()<<" query: "<<query<<endl;

			int msgId = getSecondField(msg.value());
			if(VERBOSE) cout <<msg.time()<<" msgID: "<<msgId<<endl;

			int myId = getThirdField(msg.value());
			if(VERBOSE) cout <<msg.time()<<" myID: "<<myId<<endl;

			int fromId = getFourthField(msg.value());
			if(VERBOSE) cout <<msg.time()<<" fromId: "<<fromId<<endl;

			int TTL = getFifthField(msg.value());
			if(VERBOSE) cout <<msg.time()<<" TTL: "<<TTL<<endl;

			if(msgId == 0) routingTable[myId].clear();

			if(routingTable[myId].find(msgId) == routingTable[myId].end() && TTL > 0){
					set<int>::iterator connection;
					for(connection = connectionlist.begin();connection != connectionlist.end();connection++){
						if(*connection != fromId){
							if(VERBOSE) cout <<msg.time() <<" Connection: "<<*connection<<endl;
							QueryQueue.push(buildNewMessage(query,msgId,*connection,myId,(TTL-1)));
						}
						else{
							if(VERBOSE)cout << "No new connections"<<endl;
						}
					}
				routingTable[myId].insert(msgId);
			}
			else{
				if(VERBOSE) cout << "Message already in the routing table"<<endl;
			}

		}
	}
/*
		if (msg.port() == route_in) //new message to route
		{
			//expecting float values looking like 6,123 meaning route from peer 6 with msg id 123 (id<1000)
			//get the peerid, message id, generate new TTL, then put in "to output" variable // not a list ! [id, TTL, peer]

			int fromid = getFromId(msg.value());// get originating peer (from value of external msg)
			if(VERBOSE)cout<<" peerid:"<<fromid<<endl;
			int id = getMsgId(msg.value()); // get message id
			if(VERBOSE)cout<<"  message id:"<<id<<endl;



			//create "seen" list. The message ids are float values (that way we can generate them using a random function)
			// to create an empty list I use the default constructor, throught the shortcut of calling the [] operator.
			if (routingTable.find(id)==routingTable.end())
				routingTable[id]; //creates empty set mapped to id
			else
				routingTable[id].clear();// clear the set (this case is for a msgId being reused)

			routingTable[id].insert(peerid); // the new peer has now been visited (because the first thing will be to send himself the message !)

			// we need to propagate this message to the DB and route it in the network (separate issues)
			hitting = true;
			routing = true;
			//set the output values
			// from right, digits 1-3 are id, digits 4-6 are peerid, digit 7 is TTL exampel value : 6005123 = TTL=6, peerid 5, id = 123
			nextOutputDB =  buildMessage(id, fromid);
			nextOutputR =buildMessage(id, fromid, STANDARDTTL); //standardTTL is a constant defined in the h, should be a model parameter

			//if(VERBOSE) cout<<"next output:"<<nextOutputR<<endl;

		}

		else if (msg.port() ==  in_n){
			//expecting value = TTL * 100 + peerid + id, where id is the decimal part (<1)
			if(VERBOSE) cout<<"message from the network routing loop... "<<msg.value()<<"\n";

			long inval = msg.value();

			//extract our 3 values using static functions from complexmessages.h
			int OldTTL = getTTL(inval);
			if(VERBOSE)cout<<"old ttl:"<<OldTTL<<endl;
			int fromid = getFromId(inval);
			if(VERBOSE)cout<<" peerid:"<<peerid<<endl;
			int id = getMsgId(inval);
			if(VERBOSE)cout<<"  message id:"<<id<<endl;

			//check for already visited peer : search for "peerid" in set mapped to id in routing table
			set<long>::iterator finder = routingTable[id].find(fromid);
			if(finder==routingTable[id].end()){ //it's NOT in there
				//the unseen peer must get the message
				hitting = true;
				nextOutputDB = buildMessage(id, fromid); // we don't put in a TTL
				// the new peer has now been visited : we add him to the set of seen peers
				routingTable[id].insert(fromid);

				if(OldTTL>0){ // the unseen peer will also propagate the message because it still has some TTL
					routing=true;
					nextOutputR = buildMessage(id, fromid, OldTTL-1); //add the TTL for the routing msg
				}
				else {
					routing = false;
				}
			} else {
				hitting = false;
				routing = false;
			}

		}

	} //end if state is passive
*/
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
Model &PeerGnutella::internalFunction( const InternalMessage & ){

	//set these back
	if( !QueryQueue.empty()){
		holdIn(active,Time(0,0,0,2)); //wait 2 ms to dequeue
	}
	else{
		passivate(); // we just passivate immediately
	}


	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &PeerGnutella::outputFunction( const InternalMessage &msg )
{
	if (!QueryQueue.empty()) { // if we were or now are in the process of routing messages
		if(VERBOSE) cout<<"output coming...\n";
		long long message = QueryQueue.front();

		sendOutput( msg.time(), q_out, message);
		QueryQueue.pop(); // remove latest output message !
	}
/*
	if (routing) // if we have something to output
        {
	      sendOutput( msg.time(), q_out, nextOutputR); //nextOutput contains the next value to output
        }
	if(hitting)
	{
        sendOutput( msg.time(), q_out, nextOutputDB); //nextOutput contains the next value to output

	}
*/
	return *this;
}

PeerGnutella::~PeerGnutella()
{
	//do nothing
}
