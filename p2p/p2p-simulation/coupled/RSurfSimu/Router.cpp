/*******************************************************************
*
*  DESCRIPTION: Atomic Model : The router component represents the query routing behavior of a peer.
*  The component maintains the current list of neighbors of the peer (based on inputs from the
*  NetworkGraph component), and receives input queries, both from the local peer (from RandomSurfer
*  component) and from other peers, via the network. The peer then routes the queries to its neighbors,
*  implementing a specific search protocol.
*
*
*  AUTHOR: Alan
*
*
*  DATE: November 2010
*
*
*
*******************************************************************/

/** include files **/
#include <math.h>            // fabs( ... )
#include <stdlib.h>
#include "randlib.h"         // Random numbers library
#include "Router.h"  // base header
#include "message.h"       // InternalMessage ....
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )
#include <fstream>
#include <sstream>



/*******************************************************************
* Function Name: Router
* Description: constructor
********************************************************************/
Router::Router( const string &name )
: Atomic( name )
, query( addInputPort( "query"))
, q_in( addInputPort( "q_in"))
, q_out( addOutputPort( "q_out"))
, connections( addInputPort( "connections"))
, connect_out( addOutputPort( "connect_out"))
, disconnect_out( addOutputPort( "disconnect_out"))
, qhit_out( addOutputPort( "qhit_out"))
, localOut( addOutputPort( "localOut"))
{
	//initialising these values... not indispensable but always useful
	primary = &routingTable1;
	secondary = &routingTable2;
}



/*******************************************************************
* Function Name: externalFunction
* Description: propagates queries to other peers according to it's list of connections, sends messages to the server checking for
* queryhits and routs these messages back to the initial sender of a query for queryhits
********************************************************************/
Model &Router::externalFunction( const ExternalMessage &msg ){
	if(VERBOSE) cout<<"external message: "<<msg.value()<<endl;
	//if ( this->state() == passive)
	//{

		if (msg.port() == connections){ //adds or removes a peer from the connectionlist, TTL of 1 is for adding connections
										//while a TTL of 0 is for removing a connection
			if(VERBOSE) cout <<"Input in port connections"<<endl;
			int peerId = getThirdField(msg.value()); //gets the Id of the peer from the message
			int fromId = getFourthField(msg.value());
			int TTL = getFifthField(msg.value());
			//if(VERBOSE) cout << "connections TTL (1 = add, 0 = remove):                 "<<TTL<<endl;
			if(TTL == 1){
				if(connectionlist.find(peerId) == connectionlist.end()){
					//if(VERBOSE) cout <<msg.time()<<"Peer "<<fromId<< " adding peer "<< peerId << " to list of connections"<<endl;
					connectionlist.insert(peerId);
					connectionQueue.push(buildNewMessage(0,0,0,peerId,fromId,0));
				}
			}
			else if (TTL == 0){
				if(connectionlist.find(peerId) != connectionlist.end()){
					//if(VERBOSE) cout <<msg.time()<<"Peer "<<fromId<< " removing peer "<< peerId << " from list of connections"<<endl;
					connectionlist.erase(peerId);
					disconnectionQueue.push(buildNewMessage(0,0,0,peerId,fromId,0));
				}
			}
		}

		else if (msg.port() == query) //start to propagate a query to all connections
		{
			if(VERBOSE) cout << "Input in port query"<<endl;

			int query = getFirstField(msg.value());
			if(VERBOSE) cout <<msg.time()<<" query: "<<query<<endl;

			int msgId = getSecondField(msg.value());
			if(VERBOSE) cout <<msg.time()<<"  MessageId: "<<msgId<<endl;

			int myId = getThirdField(msg.value());
			if(VERBOSE) cout <<msg.time()<<"   myID: "<<myId<<endl;

			int TTL = getFifthField(msg.value());
			if(VERBOSE) cout <<msg.time()<<"    TTL: "<<TTL<<endl;

			int qhit = getFifthDigit(msg.value());
			if(VERBOSE) cout <<msg.time()<<"     qhit: "<<qhit<<endl;

			if(msgIdList.size() == 200){
				msgIdList2.clear();
				msgIdList2 = msgIdList;
				msgIdList.clear();
				//if(VERBOSE) cout <<msg.time()<< "      Clearing msgIdList"<<endl;
			}


			msgIdList.insert(msgId);



			/* removes the msgId from routingTables so that when peer receives a queryhit for a his own query, he doesn't rout
			 * it back to whomever it might have been previously routing to */
			if((*primary).find(msgId) != (*primary).end()){
				(*primary).erase(msgId);
			}
			if((*secondary).find(msgId) != (*secondary).end()){
				(*secondary).erase(msgId);
			}





			/* send a message to the peer's server to look for queryhits. Also sends messages to all the peer's connections in
			 * order to propagate the query */
			localQuery.push(buildNewMessage(query, qhit, msgId, 0, myId, 1));
			if(TTL > 0){
				set<int>::iterator connection;
				for(connection = connectionlist.begin();connection != connectionlist.end();connection++){
					if(VERBOSE) cout <<msg.time()<<" Connection: "<<*connection<<endl;
					QueryQueue.push(buildNewMessage(query,qhit,msgId,*connection,myId,(TTL-1)));
//					cout << "QUERY MESSAGE" << QueryQueue.front() << endl ;
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

			int qhit = getFifthDigit(msg.value());
			if(VERBOSE) cout << msg.time()<< " qhit: "<<qhit<<endl;

			//whenever the msgIdList reaches a certain size, it copies all it's contents to msgIdList2, then msgIdList clears
			//itself. This way only a certain amount of IDs are stored so that there shouldn't be any conflicts later
			if(msgIdList.size() == 50){
				msgIdList2.clear();
				msgIdList2 = msgIdList;
				msgIdList.clear();
				//if(VERBOSE) cout <<msg.time()<< "      Clearing msgIdList"<<endl;
			}





			/* If message hasn't already been received, then send a message to server to look for queryhits, then send a message
			 * to all connections in order to propagate the query (the TTL is 0) */
			if(msgIdList.find(msgId) == msgIdList.end() && msgIdList2.find(msgId) == msgIdList2.end() && TTL > 0 && qhit != 1){
					localQuery.push(buildNewMessage(query, qhit, msgId, 0, myId, 0)); //build message to be output to the server (local repository)
					set<int>::iterator connection;

					/**
					 * Send to all connections except for the one that sent it to us
					 */
					for(connection = connectionlist.begin();connection != connectionlist.end();connection++){
						if(*connection != fromId){
							if(VERBOSE) cout <<msg.time() <<" Connection: "<<*connection<<endl;
							QueryQueue.push(buildNewMessage(query, qhit, msgId, *connection, myId, (TTL-1))); //build the message to be output to all connections

						}
						else{
							if(VERBOSE)cout << "No new connections"<<endl;
						}
					}
					if(connectionlist.empty()){
						if(VERBOSE)cout << "No new connections"<<endl;
					}

					/* when the primary routingTable is too big, makes it the secondary routingTable and creates a new empty one */
					if((*primary).size() == 500){
						*secondary = *primary;
						(*primary).clear();
						if(VERBOSE) cout <<                      "Clearing routingTables"<<endl;
					}

					/* removes and adds a msgId mapped to the peer which sent the message we received (to rout back queryhits) */
					if((*secondary).find(msgId) != (*secondary).end()){
						(*secondary).erase(msgId);
					}
					if((*primary).find(msgId) != (*primary).end()){
						(*primary).erase(msgId);
					}
					(*primary)[msgId] = fromId;
					msgIdList.insert(msgId);
			}

			/* if the msgId maps to a peer, then rout the message to that peer, otherwise the peer is the one that queried,
			 * therefore sends a message to the RandomSurfer Model*/
			else if(qhit == 1){

				//msgId is in the primary routing table, build a message to be routed to the peer which corresponds with this ID
				if((*primary).find(msgId) != (*primary).end()){
					fromId = (*primary)[msgId];
					QueryQueue.push(buildNewMessage(query,1,msgId,fromId,myId,0));
					if(VERBOSE) cout << "received a queryhit ---->routing in primary" <<endl;
				}

				//msgId is in the secondary routing table, build a message to be routed to the peer which corresponds with this ID
				else if((*secondary).find(msgId) != (*secondary).end()){
					fromId = (*secondary)[msgId];
					QueryQueue.push(buildNewMessage(query,1,msgId,fromId,myId,0));
					if(VERBOSE) cout << "received a queryhit ---->routing in secondary" << endl;
				}

				//msg isn't in the routing tables, it means that these messages were meant for us, therefore send it to the RandomSurfer model
				//(Session Manager)
				else{
					QueryHitQueue.push(buildNewMessage(query, 0, msgId, myId, 0, 0));
					if(VERBOSE) cout << "queryhit being sent to RandomSurfer model"<<endl;
				}

			}
			else{
				if(VERBOSE) cout << "Message already received"<<endl;
			}

		}

	//}
	/* Receiving a message in the active state isn't a problem
	else{
		cout<<"error: message received while in active state ------>Gnutella"<<endl;
	}
	*/

	// we have an instantaneous change back to the passive state (will output the next output values where relevant)
	holdIn( active, Time(0.00f));

	return *this ;
}


/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &Router::internalFunction( const InternalMessage & ){

	//if we're still sending messages
	if( !QueryQueue.empty() || !connectionQueue.empty() || !disconnectionQueue.empty() || !QueryHitQueue.empty()){
		holdIn(active,Time(0,0,0,1)); //wait 1 ms to dequeue
	}
	else{
		passivate(); // we just passivate immediately
	}


	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &Router::outputFunction( const InternalMessage &msg )
{

	/**
	 * Send message to the logger model to output the connection that has been made
	 */
	if (!connectionQueue.empty()){ // if we were or now are in the process of routing messages
		if(VERBOSE) cout << "output coming...\n";
		sendOutput( msg.time(), connect_out, connectionQueue.front());
		connectionQueue.pop();
	}

	/**
	 * Send message to the logger model to output the disconnection that was made
	 */
	if (!disconnectionQueue.empty()){ //if we were or now are in the process of routing messages
		if(VERBOSE) cout << "output coming...\n";
		sendOutput( msg.time(), disconnect_out, disconnectionQueue.front());
		disconnectionQueue.pop();
	}

	/**
	 * Propagates the query to other connections
	 * sends message
	 */
	if (!QueryQueue.empty()) { // if we were or now are in the process of routing messages
		if(VERBOSE) cout<<"output coming...\n";
		long long message = QueryQueue.front();

		sendOutput( msg.time(), q_out, message);
		QueryQueue.pop();
	}

	/**
	 * Send message to the server model (peers local repository) to search for the document requested or any possible links (depends on message)
	 */
	if (!localQuery.empty()){ // if we were or now are in the process of routing messages
		if(VERBOSE) cout << "output coming...\n";
		sendOutput( msg.time(), localOut, localQuery.front());
		localQuery.pop();
	}

	/**
	 * Sends message back to the RandomSurfer Model (Session Manager), this message contains the data needed for the RandomSurfer to publish
	 * or visit a document (again, depends on message)
	 */
	if (!QueryHitQueue.empty()){ // if we were or now are in the process of routing messages
		if(VERBOSE) cout<< "output coming...\n";
		sendOutput( msg.time(), qhit_out, QueryHitQueue.front());
		QueryHitQueue.pop();
	}

	return *this;
}

Router::~Router()
{
	delete primary;
	delete secondary;
	//do nothing
}
