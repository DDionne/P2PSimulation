/*******************************************************************
*
*  DESCRIPTION: Atomic Model :
*  Server : maintains a mock-up Server for the whole network and responds to queries
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
#include "Server.h"  // base header
#include "message.h"       // InternalMessage ....
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )
#include <fstream>



/*******************************************************************
* Function Name: Demux
* Description: constructor
********************************************************************/
Server::Server( const string &name )
: Atomic( name )
, query_in( addInputPort( "query_in" ) )
, peer_in( addInputPort( "peer_in" ) )
//, publish( addInputPort( "publish"))
, queryhit( addOutputPort( "queryhit" ) )


{
	//initialising these values... not indispensable but always safer


	serverdoc = new SGraph();
	datadoc = new SGraph();

	string datafile = MainSimulator::Instance().getParameter( description(), "datafile" );

	ifstream fis;
	fis.open(datafile.c_str()); // open stream to file


	serverdoc->read(fis); //reading serverdoc from file!
	fis.close();


	if(VERBOSE) {
		cout<<"data file read"<<endl;
		//querydoc->write(cout);
	}





}


/*******************************************************************
 * Function Name: externalFunction
 * Description: the router gets input from either the "outside" (a new messgae to route) or from the router (next step for routing)
 ********************************************************************/
Model &Server::externalFunction( const ExternalMessage &msg ){
	if(VERBOSE) cout<<endl<<"external message: "<<msg.value()<<endl;
	/*if ( this->state() == passive)
	{*/
		if (msg.port() == query_in) //putting something in the routing table
		{

			//expecting a message including the peerid and a messageId encoded according to the spec of "complexmessages.h"

/*			int payload = getPeerId(msg.value());// we encode the msg payload instead of the peerid
			if(VERBOSE)cout<<" query: "<<payload<<endl;
			int id = getMessageId(msg.value()); // get message id
			if(VERBOSE)cout<<"  message id: "<<id<<endl;

			//create "mapping entry" list. The message ids are float values (that way we can generate them using a random function)
			// to create an empty list I use the default constructor, throught the shortcut of calling the [] operator.
			routingTable[id]=payload; //associates peerid to id in routing table.
			if(VERBOSE)cout<<"  RoutingTable id: "<<id<<endl << endl;//" associated with query: "<<routingTable[id]<<endl;
*/


			int payload = getPeerId(msg.value());// we encode the msg payload instead of the peerid
			int identifier = getMessageId(msg.value());
			if(VERBOSE) cout<< endl <<" query: "<<payload<<endl;

			set<int> docsFromQuery;
			docsFromQuery = serverdoc -> getDocsFromQuery(payload); // set of documents associated with query
			int docsLeft = docsFromQuery.size();

			set<int>::iterator documents;

			if(VERBOSE) cout << "document(s) associated with query: "<<endl;
			if(docsFromQuery.size() > 0){
				for(documents = docsFromQuery.begin() ; documents != docsFromQuery.end(); documents++){
					if(VERBOSE) cout<< *documents<<"  Identifier: "<<identifier<<endl;
					docsLeft--;
					QueryhitQ.push(buildMessage(*documents,identifier,docsLeft));
				}
			}
			else{
				if(VERBOSE) cout << "none"<<endl;
			}
		}

		else if (msg.port() ==  peer_in){

			//expecting value = TTL * 100 + peerid + id, where id is the decimal part (<1)
			//if(VERBOSE) cout<<"Message: a query reaches a peer... "<<msg.value()<<"\n";
/*
			long inval = msg.value();

			//extract our 3 values using static functions from complexmessages.h
			//HACK : the msg payload will be encoded in the space meant for the peerId
			int inpeer = getPeerId(inval);
			//if(VERBOSE)cout<<" peer :"<<inpeer<<endl;
			int id = getMessageId(inval);
			if(VERBOSE)cout<<"  message id: "<<id<<endl;

			//check for already visited peer : search for "peerid" in set mapped to id in routing table
			int query = routingTable[id]; // get the query associated wityh this id
			if(VERBOSE)cout<<"  query: "<<query<<endl;
			//TODO : what does this return if the msgid is not in the table ?







			//TODO : query the peerdoc table, the querydoc table, enqueue all results for output
			//set<int> docsFromPeer;       //documents associated with peer
			set<int> docsFromQuery;        //documents associated with the query
			//docsFromPeer=peerdoc->getsDocsOfPeer(inpeer);
			//docsFromQuery=querydoc->getConnectedNodes(query);
			docsFromQuery = serverdoc -> getDocsFromQuery(query);

			//if(VERBOSE) cout<<"intersection of document sets: ";
			set<int>::iterator doc;
			//if(docsFromPeer.size() > 0){
			//	if(VERBOSE) cout<<"peer has: ";
			//}


			if(VERBOSE) cout << "document(s) associated with query: ";
			for(doc = docsFromQuery.begin() ; doc != docsFromQuery.end(); doc++){
				if(VERBOSE) cout <<endl<< *doc << " which also links to: ";

				set<int> linkeddocs;
				linkeddocs = serverdoc->getDocsFromLink(*doc);   //set of all the documents connected to by *doc (doesn't work atm)
				set<int>::iterator docs;
				if(linkeddocs.size() == 0){
					if(VERBOSE) cout << "none";
				}
				for(docs = linkeddocs.begin(); docs != linkeddocs.end();docs++){
					if(VERBOSE) cout << *docs;
				}
			}
			if(VERBOSE) cout << endl << endl;


		}

*/

/* Original stuff

			for ( doc=docsFromPeer.begin() ; doc != docsFromPeer.end(); doc++ ){ // loop through docs stored by peer
				if(VERBOSE) cout<<"peer has : "<<*doc;
				if(docsFromQuery.count(*doc)==1) {// if the considered doc matches the query
					if(VERBOSE) cout<<" (hit!); ";
					QueryhitQ.push(buildMessage(id,*doc, inpeer)); //enqueue a message made from the queryId and the queryhit document

				}else {
					if(VERBOSE) cout<<" (no match); ";
				}
			}

		}
*/

			int docID = getPeerId(msg.value());
			int identifier = getMessageId(msg.value());
			if(VERBOSE) cout<< endl <<" document: "<<docID<<endl;

			set<int> docLinks; // set of documents associated with query
			docLinks = serverdoc -> getDocsFromLink(docID);

			set<int>::iterator docs;
			int docsLeft = docLinks.size();

			if(VERBOSE) cout << "This document links to: "<<endl;
			if(docLinks.size() > 0){
				for(docs = docLinks.begin() ; docs != docLinks.end(); docs++){
					if(VERBOSE) cout << *docs << "  identifier: " << identifier <<endl;
					docsLeft--;
					QueryhitQ.push(buildMessage(*docs,identifier,docsLeft));
				}
			}
			else{
				if(VERBOSE) cout << "none"<<endl;
			}
		}

		////////////////DATA OPERATIONS : publish, remove //////////////////
	//	else if (msg.port() == publish){

			//extract message content using static functions from complexmessages.h

////add query doc-link
//			int query = getPeerId(msg.value());
			//if(VERBOSE)cout<<" peer :"<<peer<<endl;
//			int doc = getMessageId(msg.value());
			//if(VERBOSE)cout<<"  message id:"<<doc<<endl;
//			if(VERBOSE) cout<<"Query "<<query<<" associated with document "<<doc <<"\n";

//			serverdoc->querydoclink(query,doc); //store in the graph








//Peer publishing documents
/*

			int peer = getPeerId(msg.value());
			//if(VERBOSE)cout<<" peer :"<<peer<<endl;
			int doc = getMessageId(msg.value());
			//if(VERBOSE)cout<<"  message id:"<<doc<<endl;
			if(VERBOSE) cout<<"peer "<<peer<<" publishes document "<<doc <<"\n";

			peerdoc->publish(peer,doc); //store in the graph


*/







			// all below is out of date
			//if the peer or the document are not in the matrix, we need to add the peer / doc to the graph
			/*if(peerdoc->find(peer)==peerdoc->end()){ // peer not found in graph
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
			if(VERBOSE) peerdoc->write(cout);*/






//}
/*		else if (msg.port() == remove){
			//extract message content using static functions from complexmessages.h
			int peer = getPeerId(msg.value());
			if(VERBOSE)cout<<" peer : "<<peer<<endl;
			int doc = getMessageId(msg.value());
			if(VERBOSE)cout<<"  message id: "<<doc<<endl;
			if(VERBOSE) cout<<"peer "<<peer<<" removes document "<<doc <<"\n";

			//peerdoc->disconnect(peer,doc);
			peerdoc->remove(peer,doc); // this is the appropriate disconnection, not removal of nodes

		}
*/

	/*} //end if state is passive
	else{
		cout<<"DB error: message received while in active state"<<endl;
	}*/

		if ( this->state() == passive) {
			holdIn( active, Time(0,0,0,2));//pause for 20 milliseconds
		}
	/*/Any responses to output ?
	if (!QueryhitQ.empty()) { // if we were or now are in the process of routing messages
		holdIn( active, Time(0.01f)); // we wait 0.01s to dequeue
	} else {
		holdIn( active, Time(0.00f)); // we just passivate immediately
	}*/

	return *this ;
}


/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &Server::internalFunction( const InternalMessage & ){

	//Any responses to output ?
	if (!QueryhitQ.empty()) { // if we were or now are in the process of routing messages
		holdIn( active, Time(0,0,0,2)); // we wait 2ms to dequeue
		} else {
			passivate(); // we just passivate immediately
		}
	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &Server::outputFunction( const InternalMessage &msg )
{
	if (!QueryhitQ.empty()) { // if we were or now are in the process of routing messages
		if(VERBOSE) cout<<"output coming...\n";
		long message = QueryhitQ.front();

		sendOutput( msg.time(), queryhit, message);
		QueryhitQ.pop(); // remove latest output message !
		sendOutput(msg.time(), queryhit, message);
	}
	return *this;
}

Server::~Server()
{
	//remove graphs (by pointer)
	delete serverdoc;
	delete datadoc;
	//do nothing
}
