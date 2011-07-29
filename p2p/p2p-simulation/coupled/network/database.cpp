/*******************************************************************
*
*  DESCRIPTION: Atomic Model :
*  Database : maintains a mock-up database for the whole network and responds to queries
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
#include "database.h"  // base header
#include "message.h"       // InternalMessage ....
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )
#include <ifstream>



/*******************************************************************
* Function Name: Demux
* Description: constructor
********************************************************************/
Database::Database( const string &name )
: Atomic( name )
, query_in( addInputPort( "query_in" ) )
, peer_in( addInputPort( "peer_in" ) )
, publish( addInputPort( "publish" ) )
, remove( addInputPort( "remove" ) )
, queryhit( addOutputPort( "queryhit" ) )

{
	//initialising these values... not indispensable but always safer
	querydoc = new GraphInt(); // graph of query - document associations (what query matches what document)
	peerdoc = new GraphInt(); // graph of peer-document associations (who stores what)
	// load querydoc graph from file :

	string datafile = MainSimulator::Instance().getParameter( description(), "datafile" );

	ifstream fis;
	fis.open(datafile.c_str()); // open stream to file

	querydoc->read(fis); // read graph from file
	fis.close();

	cout<<"data file read"<<endl;
	querydoc->write(cout);

}


/*******************************************************************
 * Function Name: externalFunction
 * Description: the router gets input from either the "outside" (a new messgae to route) or from the router (next step for routing)
 ********************************************************************/
Model &Database::externalFunction( const ExternalMessage &msg ){
	if(VERBOSE) cout<<"DB : external message: "<<msg.value()<<endl;
	if ( this->state() == passive)
	{
		if (msg.port() == query_in) //putting something in the routing table
		{
			//expecting a message including the peerid and a messageId encoded according to the spec of "complexmessages.h"

			int payload = getPeerId(msg.value());// we encode the msg payload instead of the peerid
			//if(VERBOSE)cout<<" query:"<<payload<<endl;
			int id = getMessageId(msg.value()); // get message id
			//if(VERBOSE)cout<<"  message id:"<<id<<endl;

			//create "mapping entry" list. The message ids are float values (that way we can generate them using a random function)
			// to create an empty list I use the default constructor, throught the shortcut of calling the [] operator.
			routingTable[id]=payload; //associates peerid to id in routing table.
			if(VERBOSE)cout<<"  RoutingTable id:"<<id<<"associated with query:"<<routingTable[id]<<endl;

		}

		else if (msg.port() ==  peer_in){

			//expecting value = TTL * 100 + peerid + id, where id is the decimal part (<1)
			//if(VERBOSE) cout<<"Message: a query reaches a peer... "<<msg.value()<<"\n";

			long inval = msg.value();

			//extract our 3 values using static functions from complexmessages.h
			//HACK : the msg payload will be encoded in the space meant for the peerId
			int inpeer = getPeerId(inval);
			//if(VERBOSE)cout<<" peer :"<<inpeer<<endl;
			int id = getMessageId(inval);
			//if(VERBOSE)cout<<"  message id:"<<id<<endl;

			//check for already visited peer : search for "peerid" in set mapped to id in routing table
			int query = routingTable[id]; // get the query associated wityh this id
			if(VERBOSE)cout<<"  query:"<<query<<"; peer :"<<inpeer<<endl;
			//TODO : what does this return if the msgid is not in the table ?

			//TODO : query the peerdoc table, the querydoc table, enqueue all results for output
			set<int> docsFromPeer, docsFromQuery;
			docsFromPeer=peerdoc->getConnectedNodes(inpeer);
			docsFromQuery=querydoc->getConnectedNodes(query);

			if(VERBOSE) cout<<"DB : intersection of document sets :";
			set<int>::iterator doc;
			for ( doc=docsFromPeer.begin() ; doc != docsFromPeer.end(); doc++ ){ // loop through docs stored by peer
				if(VERBOSE) cout<<"peer has : "<<*doc;
				if(docsFromQuery.count(*doc)==1) {// if the considered doc matches the query
					if(VERBOSE) cout<<" (hit!); ";
					QueryhitQ.push(buildMessage(id,*doc)); //enqueue a message made from the queryId and the queryhit document

				}else {
					if(VERBOSE) cout<<" (nomatch); ";
				}
			}

		}
		////////////////DATA OPERATIONS : publish, remove //////////////////
		else if (msg.port() == publish){

			//extract message content using static functions from complexmessages.h
			int peer = getPeerId(msg.value());
			//if(VERBOSE)cout<<" peer :"<<peer<<endl;
			int doc = getMessageId(msg.value());
			//if(VERBOSE)cout<<"  message id:"<<doc<<endl;
			if(VERBOSE) cout<<"peer "<<peer<<" publishes document "<<doc <<"\n";

			//if the peer or the document are not in the matrix, we need to add the peer / doc to the graph
			if(peerdoc->find(peer)==peerdoc->end()){ // peer not found in graph
				if(VERBOSE) cout<<"new peer"<<endl;
				peerdoc->insert(peer);
			} else
				if(VERBOSE) cout<<"found peer:"<<*(peerdoc->find(peer))<<endl;;

			if(peerdoc->find(doc)==peerdoc->end()){ // doc not found in graph
				if(VERBOSE) cout<<"new doc"<<endl;
				peerdoc->insert(doc);
			} else
				if(VERBOSE) cout<<"found doc:"<<*(peerdoc->find(peer))<<endl;;

			peerdoc->connect(peer,doc);
			//if(VERBOSE) peerdoc->write(cout);
		}
		else if (msg.port() == remove){
			//extract message content using static functions from complexmessages.h
			int peer = getPeerId(msg.value());
			if(VERBOSE)cout<<" peer :"<<peer<<endl;
			int doc = getMessageId(msg.value());
			if(VERBOSE)cout<<"  message id:"<<doc<<endl;
			if(VERBOSE) cout<<"peer "<<peer<<" removes document "<<doc <<"\n";

			peerdoc->disconnect(peer,doc);
		}


	} //end if state is passive
	else{
		cout<<"error: message received while in active state"<<endl;
	}

	//Any responses to output ?
	if (!QueryhitQ.empty()) { // if we were or now are in the process of routing messages
		holdIn( active, Time(0.01f)); // we wait 0.01s to dequeue
	} else {
		holdIn( active, Time(0.00f)); // we just passivate immediately
	}

	return *this ;
}


/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &Database::internalFunction( const InternalMessage & ){

	//Any responses to output ?
	if (!QueryhitQ.empty()) { // if we were or now are in the process of routing messages
		holdIn( active, Time(0.01f)); // we wait 0.01s to dequeue
		} else {
			passivate(); // we just passivate immediately
		}
	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &Database::outputFunction( const InternalMessage &msg )
{
	if (!QueryhitQ.empty()) { // if we were or now are in the process of routing messages
		if(VERBOSE) cout<<"output coming...\n";
		long message = QueryhitQ.front();

		sendOutput( msg.time(), queryhit, message);
		QueryhitQ.pop(); // remove latest output message !
	}
	return *this;
}

Database::~Database()
{
	//remove graphs (by pointer)
	delete peerdoc;
	delete querydoc;
	//do nothing
}
