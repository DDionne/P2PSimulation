/*******************************************************************
 *
 *  DESCRIPTION: Atomic Model SessionManager : simulate the user's session in the p2p network.
 *  The user goes online, is active or passive until she goes offline. If active, the user outputs a random number of queries at a random time interval, then goes offline.
 *
 *
 *  AUTHOR: Alan
 *
 *
 *  DATE: November 2010 - May 2011
 *
 *******************************************************************/

/** include files **/
#include <math.h>            // fabs( ... )
#include <stdlib.h>
#include "randlib.h"         // Random numbers library
#include "SessionManager.h"  // base header
#include "message.h"       // InternalMessage ....
#include "distri.h"        // class Distribution
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )
#include <sstream>
#include <fstream> // i o streaming from files


/*******************************************************************
 * Function Name: SessionManager
 * Description: constructor
 ********************************************************************/
SessionManager::SessionManager( const string &name )
: Atomic( name )
, queryhit( addInputPort( "queryhit" ) )
, query( addInputPort( "query" ) )
, online( addOutputPort( "online" ) )
, offline( addOutputPort( "offline" ) )
, publish( addOutputPort( "publish") )
, remove( addOutputPort( "remove") )
{

	//get the parameter that specifies the id of this peer
	string Id_str = MainSimulator::Instance().getParameter( description(), "whoAmI" );
	whoAmI = int(floor(str2float(Id_str))); // clumsy way of turning a string into an integer

	//in this version we load the querylist from a file -------------------------------
	string param = MainSimulator::Instance().getParameter( description(), "querylist" );

	istringstream sis;
	sis.str(param); // open stream to string


	// read list of queries
	while(true)
	{
		int w;
		char coma; // find separator character
		sis >> w;
		savequerylist.insert(w);
		if(!sis.eof())
			sis >> coma; //separating from next value (if there is one)
		else
			break; //otherwise we're done

		//if(PVERBOSE) cout<<"Peer"<<whoAmI<<" adding query : "<<w<<endl;
	}


	///----------------------------------------------------
	//list of published documents
	param = MainSimulator::Instance().getParameter( description(), "doclist" );

	istringstream sis2;
	sis2.str(param); // open stream to string


	// read list of queries
	while(true)
	{
		int w;
		char coma;
		sis2 >> w;
		doclist.insert(w);
		if(!sis2.eof())
			sis2 >> coma; //separating from next value (if there is one)
		else
			break; //otherwise we're done
		//if(PVERBOSE) cout<<"Peer"<<whoAmI<<" adding doc : "<<w<<endl;
	}




	try
	{

		/* parameters are : betweenquerydistribution
		 * 					betweenquery[paramname1]
		 * 					betweenquery[paramname(n)]
		 *
		 * 					beforequerydistribution
		 * 					beforequery[param1]
		 *
		 * 					afterquerydistribution
		 *
		 * 					offlinetimedistribution
		 *
		 * 					inactivesessionlengthdistribution
		 *
		 * etc.
		 */
		timeBetweenQueryDist = Distribution::create( MainSimulator::Instance().getParameter( description(), "betweenquerydistribution" ) );
		for ( register int i = 0 ; i < timeBetweenQueryDist->varCount() ; i++ )
		{
			string parameter( MainSimulator::Instance().getParameter( description(), "betweenquery"+timeBetweenQueryDist->getVar(i) ) ) ;
			timeBetweenQueryDist->setVar( i, str2float( parameter ) ) ;

		}

		timeBeforeQueryDist = Distribution::create( MainSimulator::Instance().getParameter( description(), "beforequerydistribution" ) );
		for ( register int i = 0 ; i < timeBeforeQueryDist->varCount() ; i++ )
		{
			string parameter( MainSimulator::Instance().getParameter( description(), "beforequery"+timeBeforeQueryDist->getVar(i) ) ) ;
			timeBeforeQueryDist->setVar( i, str2float( parameter ) ) ;

		}

		timeAfterQueryDist = Distribution::create( MainSimulator::Instance().getParameter( description(), "afterquerydistribution" ) );
		for ( register int i = 0 ; i < timeAfterQueryDist->varCount() ; i++ )
		{
			string parameter( MainSimulator::Instance().getParameter( description(), "afterquery"+timeAfterQueryDist->getVar(i) ) ) ;
			timeAfterQueryDist->setVar( i, str2float( parameter ) ) ;

		}
		offlineTimeDist = Distribution::create( MainSimulator::Instance().getParameter( description(), "offlinetimedistribution" ) );
		for ( register int i = 0 ; i < offlineTimeDist->varCount() ; i++ )
		{
			string parameter( MainSimulator::Instance().getParameter( description(), "offlinetime"+offlineTimeDist->getVar(i) ) ) ;
			offlineTimeDist->setVar( i, str2float( parameter ) ) ;


		}

		inactiveSessionLength = Distribution::create( MainSimulator::Instance().getParameter( description(), "inactivesessionlengthdistribution" )) ;
		for ( register int i = 0 ; i < inactiveSessionLength->varCount() ; i++ )
		{
			string parameter( MainSimulator::Instance().getParameter( description(), "inactivesessionlength"+inactiveSessionLength->getVar(i) ) ) ;
			inactiveSessionLength->setVar( i, str2float( parameter ) ) ;


		}


		unif_01 = Distribution::create("uniform");
		unif_01-> setVar(0,0); //min if first parameter, set to 0
		unif_01-> setVar(1,1); //max is 2nd parameter, set to 1

		activityProb = str2float(MainSimulator::Instance().getParameter( description(), "probabilityofactivity" ) );

		//MASSERT( dist );


	} catch( InvalidDistribution &e )
	{
		e.addText( "The model " + description() + " has distribution problems!" ) ;
		e.print(cerr);
		MTHROW( e ) ;
	} catch( MException &e )
	{
		MTHROW( e ) ;
	}
}

Model &SessionManager::initFunction()
{

	//stay in the active state for some random time, before going online. Use offline distribution
	float t = max(static_cast<float>( distribution(OFFLINETIME).get() ), 0.0f); //low-bound distribution by zero

	mystate = OFFLINETIME;
	officiallyonline=false; //start offline

	if(PVERBOSE) cout<<"Peer"<<whoAmI<<" about to start with waiting time: "<<t<<endl;;
	Time toto = makeTimefromSeconds( t );
	if(PVERBOSE) cout<<"formatted time: "<<toto<<endl;;
	holdIn(active, toto); //input of constructor is in seconds
	return *this;
}

/*******************************************************************
 * Function Name: externalFunction
 * Description: only queryhits come in here.
 * At this point we don't change the model state for them, we just trace them
 ********************************************************************/
Model &SessionManager::externalFunction( const ExternalMessage &msg )
{

	if (this->state() != passive && mystate != OFFLINETIME) //if I'm not offline I'll do something about the queryhit
	{

		if(PVERBOSE) cout<<msg.time()<<"Peer "<<whoAmI<<" got queryhit:"<<msg.value()<<endl;

		//publish the document if I didn't have it
		int thedoc = getPeerId(msg.value());

		if(alldocs.find(thedoc)==alldocs.end()) { // we haven't got this one yet AND we're not about to go offline !
			doclist.insert(thedoc); // list it to be published
		}


		holdIn(active, nextChange());//just go to active state for as long as was remaining before the next scheduled internal change
	}

	return *this ;
}

/*******************************************************************
 * Function Name: internalFunction
 ********************************************************************/
Model &SessionManager::internalFunction( const InternalMessage & )
{

	float t;

	switch(mystate){
	case BEFOREQUERY :
		mystate = INTERQUERY;
		t = max(static_cast<float>( distribution(INTERQUERY).get() ), 0.0f);
		holdIn(active, makeTimefromSeconds(t));
		break;
	case INTERQUERY :
		if (!querylist.empty()) {
			//just wait a random time for the next query.
			t = max(static_cast<float>( distribution(INTERQUERY).get() ), 0.0f);
			holdIn(active, makeTimefromSeconds(t));
		} else { //linger after query activity
			mystate = AFTERQUERY;
			t = max(static_cast<float>( distribution(AFTERQUERY).get() ), 0.0f);
			holdIn(active, makeTimefromSeconds(t));

		}
		break;
	case AFTERQUERY : //go offline for a while, at the end of either active or inactive session
	case INACTIVETIME :
		mystate=OFFLINETIME;
		t = max(static_cast<float>( distribution(OFFLINETIME).get() ), 0.0f);
		holdIn(active, makeTimefromSeconds(t));
		break;
	case OFFLINETIME : //we're going online again


		if(!doclist.empty()){ //publish all docs, if necessary
			holdIn(active, Time(0,0,1,0 )); //wait 1s between publishing docs
		} else { //go online
			float choice = unif_01->get();
			if(PVERBOSE)cout<<"active/inactive choice:"<<choice<<" vs. prob="<<activityProb<<endl;
			if ( choice<activityProb){ // choose between active or inactive session
				mystate =  BEFOREQUERY; //active session
				//reload queries
				set<int>::iterator it;
				for ( it=savequerylist.begin() ; it != savequerylist.end(); it++ )
					querylist.insert(*it);

				t = max(static_cast<float>( distribution(BEFOREQUERY).get() ), 0.0f);
				holdIn(active, makeTimefromSeconds(t));
			}
			else{
				mystate =INACTIVETIME; //inactivesession
				t = max(static_cast<float>( distribution(INACTIVETIME).get() ), 0.0f);
				holdIn(active, makeTimefromSeconds(t));
			}
		}
		break;
	default :
		cout<<"error : undefined state";

	}

	return *this;
}

/*******************************************************************
 * Function Name: outputFunction
 ********************************************************************/
Model &SessionManager::outputFunction( const InternalMessage &msg )
{

	switch(mystate){
	case BEFOREQUERY :
	case INTERQUERY :
		if (!querylist.empty()) //we're querying
		{
			set<int>::iterator it;
			it = querylist.begin();
			if(PVERBOSE) cout<<msg.time()<<" : Peer "<<whoAmI<< " queries:"<<*it<<endl;
			int tosend = buildMessage(*it, whoAmI); // encode query, who am I
			sendOutput( msg.time(), query , tosend ); // we output the value in the query set on the query output port
			querylist.erase(it); // remove the query from list.

		}
		if(!doclist.empty()){ // a document to publish following a queryhit
			set<int>::iterator it;
				it = doclist.begin();
				int tosend = buildMessage(*it, whoAmI); // encode query, who am I
				sendOutput( msg.time(), publish , tosend ); // we output the value in the query set on the query output port
				alldocs.insert(*it); // save it in the collection of already published documents
				doclist.erase(it);
		}
		//the list shouldn't be empty, unless there were no queries to begin with
		break;
	case AFTERQUERY : //go offline for a while, at the end of either active or inactive session
	case INACTIVETIME :
		officiallyonline=false; //going offline next
		sendOutput( msg.time(), offline, whoAmI );
		break;
	case OFFLINETIME : //we're going online again
		if(!officiallyonline) {//if we haven't  gone online yet
			sendOutput( msg.time(), online, whoAmI );
			officiallyonline=true;
		}
		else if (!doclist.empty()) //we're publishing stuff
		{
			set<int>::iterator it;
			it = doclist.begin();
			int tosend = buildMessage(*it, whoAmI); // encode query, who am I
			sendOutput( msg.time(), publish , tosend ); // we output the value in the query set on the query output port
			alldocs.insert(*it); // save it in the collection of already published documents
			doclist.erase(it);
		}
	//	if (doclist.empty()) //once all docs are published we can go online; check again in case we just output the last publish
		//	sendOutput( msg.time(), online, whoAmI );

		break;
	default :
		cout<<"error : undefined state";

	}

	return *this;
}

SessionManager::~SessionManager()
{
	//delete all statistical distribution objects
	delete timeBetweenQueryDist ;
	delete timeBeforeQueryDist ;
	delete timeAfterQueryDist ;
	delete offlineTimeDist ;
	delete inactiveSessionLength ;
	delete unif_01;
}
