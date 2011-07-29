/*******************************************************************
*
*  DESCRIPTION: Atomic Model ConnectionManager
*
*  AUTHOR: Alan
*
*  EMAIL: mailto://hsaadawi@scs.carleton.ca
*
*
*  DATE: 18/10/2002
*
*******************************************************************/

/** include files **/
#include <math.h>            // fabs( ... )
#include <stdlib.h>
#include "randlib.h"         // Random numbers library
#include "ConnectionManager.h"  // base header
#include "message.h"       // InternalMessage ....
#include "distri.h"        // class Distribution
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )
#include <sstream>


/*******************************************************************
* Function Name: ConnectionManager
* Description: constructor
********************************************************************/
ConnectionManager::ConnectionManager( const string &name )
: Atomic( name )
, in_connect( addInputPort( "in_connect" ) )
, in_disconnect( addInputPort( "in_disconnect" ) )
//, activate( addInputPort( "addfriend" ) ) // we may decide to add friends at some point for some reason (queryhits?)
, in_online( addInputPort( "in_online" ) )
, in_offline( addInputPort( "in_offline" ) )
, out_online( addOutputPort( "out_online" ) )
, out_offline( addOutputPort( "out_offline" ) )
, out_connect( addOutputPort( "out_connect") )
, out_disconnect( addOutputPort( "out_disconnect") )
{
	//get the identifier of this peer from MA file
	string Id_str = MainSimulator::Instance().getParameter( description(), "whoAmI" );
	whoAmI = int(floor(str2float(Id_str))); // clumsy way of turning a string into an integer

	shutdown = false;
	// read the friendlist from MA file :
	//1 - get file :
	string param = MainSimulator::Instance().getParameter( description(), "friendlist" );

	istringstream sis;
		sis.str(param); // open stream to string

		// read list of friends
		while(true)
		{
			int w;
			char coma; // find separator character
			sis >> w;
			friendlist.insert(w);
			if(!sis.eof())
				sis >> coma; //separating from next value (if there is one)
			else
				break; //otherwise we're done

			//if(PVERBOSE) cout<<"Peer"<<whoAmI<<" adding query : "<<w<<endl;
		}

}

Model &ConnectionManager::initFunction()
{
	passivate(); //we start off passive, waiting to the session manager module to wake us
	shutdown=false;
	officiallyonline=false;
    return *this;
}

/*******************************************************************
* Function Name: externalFunction
* Description: Messages coming from the Session Manager (announcing that
* we're online or offline) and from the network (announcing successful
* connections and disconnections)
********************************************************************/
Model &ConnectionManager::externalFunction( const ExternalMessage &msg )
{
	int mval = msg.value();
	if ( this->state() == passive)
	{
		//mval should be my Id on port online
		if ((msg.port() == in_online) && (mval==whoAmI)) {
			set<int>::iterator it;
			for (it = friendlist.begin(); it != friendlist.end(); it++ ) {
				if(connectionlist.find(*it)==connectionlist.end()) // if we don't already have an active connection to this guy
					toconnect.insert(*it); //add to "toconnect" set
			}

			holdIn( active, Time(0,0,0,10) ) ; //go to "active" state for 10ms (then will online (to the outside world) and start outputting connection attempts)"
		}
		/*else{ //a message we don't expect when we're passive
			//cout<<"ConnectionManager :: error : I'm passive so I'm ignoring this message";
			//passivate(); stay passive
		}*/
	}
	else { //state is active
		//we are actively maintaining connections (shutdown=false)
		//OR we are actively destroying them (shutdown=true)

		if ((msg.port() == in_offline) && (mval==whoAmI)){
			shutdown = true; // go to "shutting down" mode (instantly)
			holdIn(active, Time(0.0f)); //prepare for an instant change to passive state; but we need to go through an internal change to be able to output the correct "offline" message
		}
		else if ((msg.port() == in_connect) && (getTTL(mval)==1)){ // this because messages on connect port have both connection and disconnection events, encoded differently (differntiated through TTL)

			//we just got a message to the effect that the connection with another peer "mval" is successfully established
			//we assume that this peer is a friend and we accept the connection //TODO maybe another behavior is possible
			int whichfriend = getPeerId(mval); // using encoding from complexMessages.h

			if (whichfriend==whoAmI) whichfriend=getMessageId(mval); // then it's the other one
			connectionlist.insert(whichfriend);
			cout<<"got connect msg from"<<whichfriend<<endl;
			toconnect.erase(whichfriend); // if this friend was in our prospective connections, he no longer needs to be there
			//holdIn(active, 0.0f); // just go back to what we were doing immediately
			//change state to "active" for whatever time was remaining
			holdIn(active, nextChange()); // nextChange is the time remaining until the next internal transition: we reschedule the same time
		}
		else if((msg.port() == in_connect)&& (getTTL(mval)==0)){// using encoding from complexMessages.h
			//we just got a message to the effect that the another peer has disconnected from us
			int whichfriend = getPeerId(mval);
			if (whichfriend==whoAmI) whichfriend=getMessageId(mval);
			cout<<"got disconnect msg from"<<whichfriend<<endl;
			connectionlist.erase(whichfriend); // record disconnection
			//holdIn(active, 0.0f); // just go back to what we were doing immediately
			holdIn(active, nextChange()); // nextChange is the time remaining until the next internal transition: we reschedule the same time
		}

		//(shutdown may be true: we're busy shutting down, but let's consider the case where we want to reactivate the connection)
		else if ((msg.port() == in_online) && (mval==whoAmI)){
			shutdown = false; // exit the "shutting down" mode (instantly)
			holdIn(active, Time(0.0f)); //prepare for an instant change to passive state; but we need to go through an internal change to be able to output the correct "offline" message
		}

	}
	return *this ;
}

/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &ConnectionManager::internalFunction( const InternalMessage & )
{
	if (shutdown){
		if(!connectionlist.empty() || officiallyonline) //we're in the process of emptying the connectionlist
			holdIn(active, Time(0,0,0,10)); //output all disconnects with 10ms interval
		else{ //else we're ready to passivate
			shutdown=false; //so we're moving out of the shutdown mode
			toconnect.clear();
			passivate();
		}
	}
	else {
		if (toconnect.empty()) {//if we've finished iterating through the list of friends to connect to
			//rebuild toconnect list

			set<int>::iterator it;
			for (it = friendlist.begin(); it != friendlist.end(); it++ ) {
				if(connectionlist.find(*it)==connectionlist.end()) // if we don't already have an active connection to this guy
					toconnect.insert(*it); //add to "toconnect" set
			}
			//ready to start again
			//cout<<"waiting 2 min! \n";
			holdIn(active, Time(0,2,0,0)); // wait 2 min before starting again
		} else { // we currently have a list of peers that we're busy connecting to
			holdIn(active, Time(0,0,1,0)); // we'll just wait one second before outputting a connect message
		}
	}

	return *this;
}

/*******************************************************************
* Function Name: outputFunction
********************************************************************/
Model &ConnectionManager::outputFunction( const InternalMessage &msg )
{
	if (shutdown ) {
		if (!connectionlist.empty()) { // if we haven't finished emptying our connectionlist
			set<int>::iterator it;
			it = connectionlist.begin(); // get a first element from this list
			int tosend = *it;
			tosend = buildMessage(tosend, whoAmI); // encode other peer id + who we are
			connectionlist.erase(it); //remove the connection
			sendOutput( msg.time(), out_disconnect, tosend ); //output on "disconnect" port
		}
		//
		else if (connectionlist.empty() && officiallyonline){
			officiallyonline=false;
			sendOutput(msg.time(), out_offline, whoAmI); //output the offline message
		}
	}
	else{
		if(!officiallyonline){
			officiallyonline=true;
			sendOutput(msg.time(), out_online, whoAmI); //output the "online" message
		} else if (!toconnect.empty()) { // if we have nobody to connect to then we just stay quiet
			set<int>::iterator it;
			it = toconnect.begin(); // get a first element from this list and output a connection message for this friend
			int tosend = *it;
			tosend = buildMessage(tosend, whoAmI); // encode who we want to connect from + who we are
			toconnect.erase(it); //remove the friend from the list of prospective contacts
			sendOutput( msg.time(), out_connect, tosend );
		}
	}

	return *this;
}

ConnectionManager::~ConnectionManager()
{
	//nothing to be done
}
