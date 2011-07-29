/*******************************************************************
*
*  DESCRIPTION: Atomic Model Logger
*
*  AUTHOR: Denis
*
*  EMAIL: mailto://hsaadawi@scs.carleton.ca
*
*Description: Outputs to a text file all the outputs (publish, online, offline, connect and disconnect)
*
*  DATE: 14/07/2011
*
*******************************************************************/

/** include files **/
#include <math.h>            // fabs( ... )
#include <stdlib.h>
#include "randlib.h"         // Random numbers library
#include "Logger.h"  // base header
#include "message.h"       // InternalMessage ....
#include "distri.h"        // class Distribution
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )
#include <sstream>


/*******************************************************************
* Function Name: Logger
* Description: constructor
********************************************************************/
Logger::Logger( const string &name )
: Atomic( name )
, in_connect( addInputPort( "in_connect" ) )
, queryReached( addInputPort( "queryReached"))
, in_publish( addInputPort( "in_publish"))
, in_disconnect( addInputPort( "in_disconnect"))
, in_online( addInputPort( "in_online"))
, in_offline( addInputPort( "in_offline"))
, in_query( addInputPort( "in_query"))
, qhit_in( addInputPort( "qhit_in"))
, publish( addOutputPort( "publish"))
, online( addOutputPort( "online"))
, offline( addOutputPort( "offline"))
, connect( addOutputPort( "connect"))
, disconnect( addOutputPort( "disconnect"))

{



}

Model &Logger::initFunction()
{

}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &Logger::externalFunction( const ExternalMessage &msg )
{
	if(this->state() == passive){

	/* prints the queryhit message to the console */
	if(msg.port() == qhit_in){
		long long tosend = msg.value();
		if(LogVERBOSE) cout << msg.time() << " queryhit "<<tosend<<endl;
	}

	/* prints the queryreachespeer message to the console */
	if(msg.port() == queryReached){
		int myId = getFourthField(msg.value());
		int query = getFirstField(msg.value());
		int msgId = getSecondField(msg.value());
		int qhit = getFifthDigit(msg.value());
		int TTL = getFifthField(msg.value());
		long long tosend = buildNewMessage(query, 0, msgId, myId, 0, 0);
		if(TTL != 1){
			if(LogVERBOSE) cout << msg.time() << " queryreachespeer "<<tosend<<endl;
		}
	}

	/*(if (qhit == 2) prints the query message to the console
	 *
	 * otherwise prints the query message and the publish message to the console*/
	if(msg.port() == in_query){
		long long tosend = msg.value();
		if(LogVERBOSE) cout << msg.time() << " query "<<tosend<<endl;

	}

	/* prints the publish message to the console */  //------->currently being output in the Server model
//	if(msg.port() == in_publish){
//		long long tosend;
//		int peer = getThirdField(msg.value());
//		int msgId = getSecondField(msg.value());
//		int doc = getFirstField(msg.value());
//		int qhit = getFifthDigit(msg.value());
//		tosend = buildNewMessage(doc,0, msgId, peer,0,0);
//		if(LogVERBOSE) cout << msg.time() << " publish " << tosend<<endl;
//		cout << msg.time() << " publish " << tosend<<endl;
//	}

	/* prints the online message to the console */
	if(msg.port() == in_online){
		if(LogVERBOSE) cout << msg.time() << " online "<<msg.value()<<endl;
	}

	/* prints the offline message to the console */
	if(msg.port() == in_offline){
		if(LogVERBOSE) cout << msg.time() << " offline "<<msg.value()<<endl;
	}

	/* prints the connect message to the console */
	if(msg.port() == in_connect){
		long long tosend = msg.value();
		if(LogVERBOSE) cout << msg.time() << " connect "<<tosend<<endl;
	}

	/* prints the disconnect message to the console */
	if(msg.port() == in_disconnect){
		long long tosend = msg.value();
		if(LogVERBOSE) cout << msg.time() << " disconnect "<<tosend<<endl;
	}
	holdIn(active, 0.0f);
	}

	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &Logger::internalFunction( const InternalMessage & )
{

	passivate();
	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &Logger::outputFunction( const InternalMessage &msg )
{



	return *this;
}

Logger::~Logger()
{
	//nothing to be done
}
