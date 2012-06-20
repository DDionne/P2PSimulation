/*******************************************************************
*
*  DESCRIPTION: Atomic Model : The repository maintains the list of documents stored locally by the
*  peer. Its inputs are the publish and remove operations. This model is also used to answer
*  queries; it therefore matches the queries with its own documents. For this purpose, it
*  takes queries as inputs, and has an output for query answers.
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
#include "Repository.h"  // base header
#include "message.h"       // InternalMessage ....
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )
#include <fstream>



/*******************************************************************
* Function Name: Demux
* Description: constructor
********************************************************************/
Repository::Repository( const string &name )
: Atomic( name )
, peer_in( addInputPort( "peer_in" ) )
, publish( addInputPort( "publish"))
, remove( addInputPort( "remove"))
, queryhit( addOutputPort( "queryhit" ) )
, p_out( addOutputPort( "p_out"))
, link( addOutputPort( "link"))


{

	string param = MainSimulator::Instance().getParameter( description(), "capacity" );
	capacity = int(floor(str2float(param)));

}


/*******************************************************************
 * Function Name: externalFunction
 * Description: Checks for queryhits and looks for possible links by sending the message to the LinkFinder Atomic Model
 ********************************************************************/
Model &Repository::externalFunction( const ExternalMessage &msg ){

	/**
	 * First, we check whether or not we have the requested document
	 */
	if(SVERBOSE) cout<<endl<<"external message: "<<msg.value()<<endl;
		int docID = getFirstField(msg.value());

		docFound = false;
		list<int>::iterator ListIt;
		for(ListIt = currentDocs.begin(); ListIt != currentDocs.end(); ListIt++){
			if(*ListIt == docID){
//				cout << "SAME"<<endl;
				docFound = true;
				break;
			}
		}

		if (msg.port() ==  peer_in){//Responds to a query

			//int docID = getFirstField(msg.value());
			int msgId = getSecondField(msg.value());
			int myId = getFourthField(msg.value());
			int qhit = getFifthDigit(msg.value());
			if(SVERBOSE) cout << "qhit: "<<qhit<<endl;
			long long output = msg.value();

//			docFound = false;
//			list<int>::iterator it;
//			for(it = currentDocs.begin(); it != currentDocs.end(); it++){
//				if(*it == docID){
//					docFound = true;
//					break;
//				}
//			}


			/**
			 * if qhit == 0, we need to find all the documents that are related to the one received in the input port, therefore, we send a
			 * message to the LinkFinder Model, which then looks up all available links
			 *
			 * ...encodes a message to be sent to the LinkFinder model
			 */
			if(qhit == 0){
				if(SVERBOSE) cout<< endl <<" document: "<<docID<<endl;


				docCheckQ.push(output);
			}

			/**
			 * If qhit == 1 AND docFound is true, then it means that the current document is a possible link and needs to be routed back to
			 * the peer that is searching... encodes a message of this document
			 */
			else if(qhit == 1){

				if(docFound){
					output = buildNewMessage(docID, 1, msgId, 0, myId, 0);
					if(SVERBOSE) cout << "sending queryhit for doc: "<<docID<<endl;
					QueryhitQ.push(output);
				}
			}

			/**
			 * If qhit == 2 AND docFound is true, then it means that the current document is the one desired, therefore it needs to be routed
			 * back to the peer that is searching... encodes a message of this document
			 */
			else if(qhit == 2){
				if(docFound){
					output = (buildNewMessage(docID, 1, msgId, 0, myId, 0));
					QueryhitQ.push(output);
				}
			}

		}

		/* adds document to list of documents owned by the peer */
		else if (msg.port() == publish){

//			if(docFound){
//				cout << "docFound == true"<<endl;
//			}
//			else{
//				cout << "docFound == false"<<endl;
//			}
			int myId = getThirdField(msg.value());
			int qhit = getFifthDigit(msg.value());
			int msgId = getSecondField(msg.value());

			if(SVERBOSE) cout << "A Peer published a document"<<endl;
			if(qhit != 2){
				if(!docFound){
//					cout<< "MAXDOCS: "<<capacity<<endl;
//					cout<< "MyID: "<< myId <<"Size of List: "<<currentDocs.size()<<endl;
					currentDocs.push_front(docID);
					if(currentDocs.size() > capacity){
						int docremove = currentDocs.back();
						currentDocs.pop_back();
						long long rOutput = buildNewMessage(docremove,0,0,myId,0,0);
						cout << msg.time() << " remove "<< rOutput<<endl;
					}
//					list<int>::iterator p; //start
//					cout << "new docList for Peer "<<myId<<":[ ";
//					for(p = currentDocs.begin(); p != currentDocs.end();p++){
//						cout << *p << " ";
//					}
//					cout <<"]"<< endl; //end

					publishQ.push(buildNewMessage(docID,0,msgId,myId,0,0));
				}
				else{
					currentDocs.remove(docID);
					currentDocs.push_front(docID);
//					list<int>::iterator p;  //start
//					cout << "new docList for Peer "<<myId<<":[ ";
//					for(p = currentDocs.begin(); p != currentDocs.end();p++){
//						cout << *p << " ";
//					}
//					cout <<"]"<< endl;  //end
				}
				currentDocs.unique();
			}

		}
	/* removes the document from the list of documents owned by the peer...currently doesn't actually do anything */
		else if (msg.port() == remove){
			if(SVERBOSE) cout << "A Peer removed a document"<<endl;
			//currentDocs.erase(msg.value());
		}


		//if ( this->state() == passive) {
			holdIn( active, Time(0,0,0,1));//was pause for 20 milliseconds
		//}

	return *this ;
}


/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &Repository::internalFunction( const InternalMessage & ){

	//Any responses to output ?
	if (!QueryhitQ.empty() || !publishQ.empty() || !docCheckQ.empty()) { // if we were or now are in the process of routing messages
		holdIn( active, Time(0,0,0,1)); // we wait 1ms to dequeue (was 2ms)
		} else {
			passivate(); // we just passivate immediately
		}
	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &Repository::outputFunction( const InternalMessage &msg )
{
	if (!docCheckQ.empty()) {
		if(SVERBOSE) cout << "output coming...\n";
		long long message = docCheckQ.front();

		sendOutput( msg.time(), link, message);
		docCheckQ.pop();
	}
	if (!QueryhitQ.empty()) { //if we were or now are in the process of routing messages
		if(SVERBOSE) cout<<"output coming...\n";
		long long message = QueryhitQ.front();
		cout << msg.time() << " queryhit "<<message<<endl;

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

Repository::~Repository()
{
  //do absolutely nothing (nothing needs to be deleted)
}
