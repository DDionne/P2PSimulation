/*******************************************************************
*
*  DESCRIPTION: Atomic Model :
*  Server : maintains a mock-up Server for the whole network and responds to queries
*
*  AUTHOR: Alan
*
*  Edited from Database to Server by: Denis
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

	string datafile = MainSimulator::Instance().getParameter( description(), "datafile" );

	ifstream fis;
	fis.open(datafile.c_str()); // open stream to file


	serverdoc->read2(fis); //reading serverdoc from file!
	fis.close();


	//if(VERBOSE) {
		cout<<"data file read"<<endl;
		//querydoc->write(cout);
	//}


}


/*******************************************************************
 * Function Name: externalFunction
 * Description: the router gets input from either the "outside" (a new messgae to route) or from the router (next step for routing)
 ********************************************************************/
Model &Server::externalFunction( const ExternalMessage &msg ){
	if(VERBOSE) cout<<endl<<"external message: "<<msg.value()<<endl;
	/*if ( this->state() == passive)
	{*/
		if (msg.port() == query_in) //Responding to a query
		{

			int payload = getPeerId(msg.value());// we encode the msg payload instead of the peerid
			int identifier = getMessageId(msg.value());
			if(VERBOSE) cout<< endl <<" query: "<<payload<<endl;

			set<int> docsFromQuery;
			docsFromQuery = serverdoc -> getDocsFromQuery(payload); // set of documents associated with query
			int docsLeft = docsFromQuery.size();

			set<int>::iterator documents;

			if(VERBOSE) cout << "document(s) associated with query: "<<endl; //For each document associated with query, output a queryhit.
			if(docsFromQuery.size() > 0){
				for(documents = docsFromQuery.begin() ; documents != docsFromQuery.end(); documents++){
					if(VERBOSE) cout<< *documents<<"  Identifier: "<<identifier<<endl;
					docsLeft--;
					QueryhitQ.push(buildMessage(*documents,identifier,docsLeft));
				}
			}
			else{
				if(VERBOSE) cout << "none"<<endl; // If there isn't any documents
			}
		}

		else if (msg.port() ==  peer_in){//Responds to a link



			int docID = getPeerId(msg.value());
			int identifier = getMessageId(msg.value());
			if(VERBOSE) cout<< endl <<" document: "<<docID<<endl;

			set<int> docLinks; // set of documents associated with link
			docLinks = serverdoc -> getDocsFromLink(docID);

			set<int>::iterator docs;
			int docsLeft = docLinks.size();

			if(VERBOSE) cout << "This document links to: "<<endl; //For each document associated with the link, output a queryhit
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


		if ( this->state() == passive) {
			holdIn( active, Time(0,0,0,2));//pause for 20 milliseconds
		}

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
	if (!QueryhitQ.empty()) { //if we were or now are in the process of routing messages
		if(VERBOSE) cout<<"output coming...\n";
		long message = QueryhitQ.front();

		sendOutput( msg.time(), queryhit, message);
		QueryhitQ.pop(); // remove latest output message !
	}
	return *this;
}

Server::~Server()
{
	//remove graphs (by pointer)
	delete serverdoc;
	//do nothing
}
