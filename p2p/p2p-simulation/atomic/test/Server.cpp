/*******************************************************************
*
*  DESCRIPTION: Atomic Model :
*  Server : maintains a mock-up Server for the whole network and responds to queries
*
*  AUTHOR: Denis
*
*
*  DATE: November 2010
*  Edit: June 2011
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
, publish( addInputPort( "publish"))
, remove( addInputPort( "remove"))
, queryhit( addOutputPort( "queryhit" ) )
, p_out( addOutputPort( "p_out"))


{
	//initialising these values... not indispensable but always safer

	for(int i = 0; i != SIZE ; i++){
		serverdoc = new SGraph();

		string datafile = MainSimulator::Instance().getParameter( description(), "datafile" );

		ifstream fis;
		fis.open(datafile.c_str()); // open stream to file


		serverdoc->read2(fis); //reading serverdoc from file!
		fis.close();
		a[i] = serverdoc;
		cout << "size of the array: "<<i + 1<<endl;
	}


}


/*******************************************************************
 * Function Name: externalFunction
 * Description: the router gets input from either the "outside" (a new message to route) or from the router (next step for routing)
 ********************************************************************/
Model &Server::externalFunction( const ExternalMessage &msg ){
	if(SVERBOSE) cout<<endl<<"external message: "<<msg.value()<<endl;


		if (msg.port() ==  peer_in){//Responds to a query



			int docID = getFirstField(msg.value());
			int msgId = getSecondField(msg.value());
			int myId = getFourthField(msg.value());
			int qhit = getFifthDigit(msg.value());
			long long output = msg.value();


			/* if qhit != 2, encodes messages of all the documents related to the query, if the peer has them.
			 * if qhit == 2, encodes a message of the document queried, if the peer has it */
			if(qhit != 2){
				if(SVERBOSE) cout<< endl <<" document: "<<docID<<endl;

				set<int> docLinks; // set of documents associated with link
				docLinks = serverdoc -> getDocsFromLink(docID);

				set<int>::iterator docs;
				set<int>::iterator CDocs;
				int docsLeft = docLinks.size();

//				if(SVERBOSE) cout << "This document links to: "<<endl; //For each document associated with the link, output a queryhit
				if(docLinks.size() > 0){
					for(docs = docLinks.begin() ; docs != docLinks.end(); docs++){
						for(CDocs = currentDocs.begin() ; CDocs != currentDocs.end(); CDocs++){
							if(*docs == *CDocs){
								if(SVERBOSE) cout << "qhit with "<<*CDocs<<endl;
								output = buildNewMessage(*CDocs, 1, msgId, myId, 0, 0);
								QueryhitQ.push(output);
							}
						}
					}
				}



				else{
					if(SVERBOSE) cout << "none"<<endl;
				}
			}
			else if(qhit == 2){
				if(currentDocs.find(docID) != currentDocs.end()){
					output = (buildNewMessage(docID, 1, msgId, myId, 0, 0));
					QueryhitQ.push(output);
				}
			}

		}

		/* adds document to list of documents owned by the peer */
		else if (msg.port() == publish){


			int doc = getFirstField(msg.value());
			int myId = getThirdField(msg.value());
			int qhit = getFifthDigit(msg.value());
			int msgId = getSecondField(msg.value());

			if(SVERBOSE) cout << "A Peer published a document"<<endl;
			if(qhit != 2){
				if(currentDocs.find(doc) == currentDocs.end()){
					currentDocs.insert(doc);
					publishQ.push(buildNewMessage(doc,0,msgId,myId,0,0));
				}
			}

		}
	/* removes the document from the list of documents owned by the peer */
		else if (msg.port() == remove){
			if(SVERBOSE) cout << "A Peer removed a document"<<endl;
			currentDocs.erase(msg.value());
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
	if (!QueryhitQ.empty() || !publishQ.empty()) { // if we were or now are in the process of routing messages
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
		if(SVERBOSE) cout<<"output coming...\n";
		long long message = QueryhitQ.front();

		sendOutput( msg.time(), queryhit, message);
		QueryhitQ.pop(); // remove latest output message !
	}
	if (!publishQ.empty()){ // if we were or now are in the process of routing messages
		if(SVERBOSE) cout << "output coming...\n";
		long long message = publishQ.front();
		cout << msg.time() << " publish " << message<<endl;

		sendOutput( msg.time(), p_out, message);
		publishQ.pop();
	}
	return *this;
}

Server::~Server()
{
	//remove graphs (by pointer)
	delete serverdoc;
	//do nothing
}
