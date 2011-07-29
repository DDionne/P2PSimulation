/*******************************************************************
 *
 *  DESCRIPTION: Atomic Model RandomSurfer : Simulates the random surfer model
 *  The user goes online for a random interval of time, during which he outputs a query or a link. Then he
 *  waits for all the documents that respond to his query/link to come in. He then either chooses one at
 *  random or ignores them completely. If he chooses to ignore them, he outputs another random query.
 *  When the time interval is over, he goes offline.
 *
 *
 *  AUTHOR: Alan
 *
 *  Edited from SessionManager to RandomSurfer by: Denis
 *
 *  DATE: November 2010 - May 2011
 *
 *******************************************************************/

/** include files **/
#include <math.h>            // fabs( ... )
#include <stdlib.h>
#include "randlib.h"         // Random numbers library
#include "RandomSurfer.h"  // base header
#include "message.h"       // InternalMessage ....
#include "distri.h"        // class Distribution
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )
#include <sstream>
#include <fstream> // i o streaming from files


/*******************************************************************
 * Function Name: RandomSurfer
 * Description: constructor
 ********************************************************************/
RandomSurfer::RandomSurfer( const string &name )
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

	//in this version we load the query range from a file -------------------------------
	string param = MainSimulator::Instance().getParameter( description(), "NumQueries" );
	maxQuery = int(floor(str2float(param)));



	///----------------------------------------------------
	//list of published documents
	param = MainSimulator::Instance().getParameter( description(), "doclist" );

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
//		doclist.insert(w);
//		if(!sis2.eof())
//			sis2 >> coma; //separating from next value (if there is one)
//		else
//			break; //otherwise we're done
//	}







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
		ignoreProb = str2float(MainSimulator::Instance().getParameter(description(), "probabilityToIgnore"));

		identifier = -1; //Id so that only one document from a same query may be published
						 //TODO: Need to change it so that it's not a magic number
		hitsLeft = 0;
		nextchange = Time(0,0,0,0);
		isactive = false;
		start = Time(0,0,0,0);
		stop = Time(0,0,0,0);



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

Model &RandomSurfer::initFunction()
{

	//stay in the active state for some random time, before going online. Use offline distribution
	float t = max(static_cast<float>( distribution(OFFLINE).get() ), 0.0f); //low-bound distribution by zero

	mystate = OFFLINE;
	officiallyonline=false; //start offline


	int initialdoc = (unif_01->get()*maxQuery)+1; // start off with random document
	doclist.insert(initialdoc);



	if(PVERBOSE) cout<<endl<<"Peer"<<whoAmI<<" about to start with waiting time: "<<t<<endl;;
	Time toto = makeTimefromSeconds( t );
	if(PVERBOSE) cout<<"formatted time: "<<toto<<endl;;


	holdIn(active, toto); //input of constructor is in seconds
	return *this;
}

/*******************************************************************
 * Function Name: externalFunction
 * Description: only queryhits come in here.
 * At this point we don't change the model state for them, we just trace them
 *
 * Stores all documents from queryhits into a set. Once all the
 * Queryhits have come in, the random surfer either picks a document
 * at random or ignores them all. If he ignores them, he sends out
 * another random query.
 ********************************************************************/
Model &RandomSurfer::externalFunction( const ExternalMessage &msg )
{

	if (this->state() != passive && mystate != OFFLINE) //if I'm not offline I'll do something about the queryhit
	{

		if(PVERBOSE) cout<<msg.time()<<" Peer "<<whoAmI<<" got queryhit : "<<msg.value()<<endl;





		hitsLeft = getTTL(msg.value());   //Nbr of queryhits incoming
		int thedoc = getMessageId(msg.value()); //Document Id
		int messageID = getPeerId(msg.value()); //Message Identifier


//		if(linkChoices.find(thedoc) == linkChoices.end()) { //Add document to choices if it isn't already there!
			linkChoices.insert(thedoc);
//		}
		nextchange = nextChange();
		holdIn(active,nextchange);








//		if(hitsLeft == 0){ //When no more queryhits are incoming, choose one of the documents at random!
//
//
//			if(nextChange() < Time(0,0,10,0)){
//				nextchange = Time(0,0,0,0);
//			}
//			else{
//				nextchange = nextChange() - Time(0,0,10,0);
//			}
//			float randomness = unif_01->get();
//			if(PVERBOSE)cout <<msg.time()<< " ignore/publish   choice: "<< randomness << " vs. " << ignoreProb << endl;
//
//
//			if(randomness < ignoreProb){ //we check whether the random surfer is going to publish the file or simply ignore it
//										//If ignored, Adds a random query to the querylist
//				if(PVERBOSE) cout << msg.time()<<" ignoring links"<<endl;
//				int queries = (unif_01->get()*maxQuery)+1; //randomly chooses a possible query
//				//querylist.insert(queries);
//				doclist.insert(queries); //no queries for current data
//			}
//			else{
//
//				int nbrOfLinks = linkChoices.size();
//				int linkPicker = unif_01->get()*(nbrOfLinks);
//				set<int>::iterator link = linkChoices.begin();
//				advance(link,linkPicker); //iterate and stop randomly in the set to pick a link
//				doclist.insert(*link);// add the document to be published
//				if(PVERBOSE) cout <<msg.time()<< " Adding document "<<*link<< " to be published"<<endl;
//
//			}
//			linkChoices.clear();
//			holdIn(active, Time(0,0,10,0));//just go to active state for 10 sec
//
//		}
//		if(PVERBOSE) cout <<" Next Change while in the querying: "<<nextChange()<<endl;
//		else{
//			nextchange = nextChange();
//			holdIn(active, nextchange);
//		}
//
//		if(PVERBOSE) cout <<"     TOTAL SESSION LENGTH: "<< start<<endl;
//		if(PVERBOSE) cout <<"              NEXT CHANGE: "<< nextchange<<endl;

	}
	else{
		if(this->state() == passive)
			if(PVERBOSE) cout<<" external function        I'm passive"<<endl;
		if(mystate == OFFLINE)
			if(PVERBOSE) cout<<" external function        I'm offline"<<endl;
	}
	return *this ;
}

/*******************************************************************
 * Function Name: internalFunction
 ********************************************************************/
Model &RandomSurfer::internalFunction( const InternalMessage &im )
{

	float t;
	int queries;
	switch(mystate){
	case SESSION : //when session runs out, surfer goes offline
		if(PVERBOSE) cout<<"state: SESSION"<<endl;
		if(PVERBOSE) cout << im.time() << "          "<<stop<<endl;


	//Make surfer stay in the SESSION state for 30 seconds before going to the QUERYHIT state
		if(im.time() > stop){
			mystate = OFFLINE;
			t = max(static_cast<float>( distribution(OFFLINE).get() ), 0.0f);
			holdIn(active, makeTimefromSeconds(t));
		}
		else{
			mystate = QUERYHIT;
			holdIn(active, Time(0,0,5,0));
		}


/* Need to re-make start-stop time
		mystate = OFFLINE;
		queries = (unif_01->get()*maxQuery)+1; //random number from 0 to MaxQuery
		doclist.insert(queries);
		isactive = false;
		t = max(static_cast<float>( distribution(OFFLINE).get() ), 0.0f);
		holdIn(active, makeTimefromSeconds(t));
*/
		break;


	case QUERYHIT :
		if(PVERBOSE) cout<<"state: QUERYHIT"<<endl;

	//Make surfer wait 30 seconds in the QUERYHIT state, then go to the SESSION state
		if(im.time() > stop){
			mystate = OFFLINE;
			t = max(static_cast<float>( distribution(OFFLINE).get() ), 0.0f);
			holdIn(active, makeTimefromSeconds(t));
		}
		else{
			mystate = SESSION;
			holdIn(active, Time(0,0,5,0));
		}


/*
		if(hitsLeft == 0){
			mystate = SESSION;
			holdIn(active, nextchange);
		}

		else{
			mystate = OFFLINE;
			t = max(static_cast<float>( distribution(OFFLINE).get() ), 0.0f);
			holdIn(active, makeTimefromSeconds(t));
		}
*/
		break;


	case OFFLINE : //we're going online again
		if(PVERBOSE) cout<<"state: OFFLINE"<<endl;


		if(!doclist.empty()){ //publish all docs, if necessary
			holdIn(active, Time(0,0,1,0 )); //wait 1s between publishing docs
		} else { //go online

			mystate =  SESSION; //active session

			t = max(static_cast<float>( distribution(SESSION).get() ), 0.0f);
			start = makeTimefromSeconds(t);
			stop = im.time() + start;

			holdIn(active, Time(0,0,5,0));

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
Model &RandomSurfer::outputFunction( const InternalMessage &msg )
{

	switch(mystate){
	case SESSION :
		if(msg.time() > stop){
			sendOutput( msg.time(), offline, whoAmI );
			officiallyonline=false;
		}
//		sendOutput( msg.time(), offline, whoAmI );
//		officiallyonline=false;




		break;

	case QUERYHIT :
		if(PVERBOSE) cout << "QUERYHIT OUTPUT"<<endl;
/////////////No Queries in this version (for the moment)
//		if (!querylist.empty() && doclist.empty() && hitsLeft == 0) //we're querying
//										//only if there are no documents being published and
//										//there are no more queryhits coming
//		{
//			set<int>::iterator it;
//			it = querylist.begin();
//
//			//if(identifier + 1 > 999) identifier = -1; //identifier can't currently be more than 3 digits
//
//			if(PVERBOSE) cout<<msg.time()<<" : Peer "<<whoAmI<< " queries:"<<*it<<endl;
//			int tosend = buildMessage((identifier+1),*it); // encode query, who am I
//			sendOutput( msg.time(), query , tosend ); // we output the value in the query set on the query output port
//			querylist.erase(it); // remove the query from list.
//			isactive = true;
//				}
		if(!linkChoices.empty() && msg.time() < stop){ // a document to publish following a queryhit


			float randomness = unif_01->get();
			if(PVERBOSE)cout <<msg.time()<< " ignore/publish   choice: "<< randomness << " vs. " << ignoreProb << endl;


			if(randomness < ignoreProb){ //we check whether the random surfer is going to publish the file or simply ignore it
										//If ignored, Adds a random query to the querylist
				if(PVERBOSE) cout << msg.time()<<" ignoring links"<<endl;
				int queries = (unif_01->get()*maxQuery)+1; //randomly chooses a possible query
				//querylist.insert(queries);
				int tosend = buildMessage((identifier + 1),queries);
				if(PVERBOSE) cout << msg.time()<<" : Peer" <<whoAmI<< " publishes: "<<queries<<endl;
				sendOutput( msg.time(), publish , tosend ); // we output the value in the query set on the query output port
				alldocs.insert(queries); // save it in the collection of already published documents
			}
			else{

				int nbrOfLinks = linkChoices.size();
				int linkPicker = unif_01->get()*(nbrOfLinks);
				set<int>::iterator link = linkChoices.begin();
				advance(link,linkPicker); //iterate and stop randomly in the set to pick a link
//				if(PVERBOSE) cout <<msg.time()<< " Adding document "<<*link<< " to be published"<<endl;
				if(PVERBOSE) cout << msg.time()<<" : Peer" <<whoAmI<< " publishes: "<<*link<<endl;
				int tosend = buildMessage((identifier + 1),*link);
				sendOutput( msg.time(), publish , tosend ); // we output the value in the query set on the query output port
				alldocs.insert(*link); // save it in the collection of already published documents
				isactive = true;
			}
			linkChoices.clear();



//			set<int>::iterator it;
//			it = doclist.begin();

			//if(identifier + 1 > 999) identifier = -1; //identifier can't currently be more than 3 digits

//			if(PVERBOSE) cout << msg.time()<<" : Peer" <<whoAmI<< " publishes: "<<*it<<endl;
//			int tosend = buildMessage((identifier + 1),*it); // encode query, who am I
//			sendOutput( msg.time(), publish , tosend ); // we output the value in the query set on the query output port
//			alldocs.insert(*it); // save it in the collection of already published documents
//			doclist.erase(it);
//			isactive = true;
		}
		else if(msg.time() < stop){
			int queries = (unif_01->get()*maxQuery)+1; //randomly chooses a possible query
			//querylist.insert(queries);
			int tosend = buildMessage((identifier + 1),queries);
			if(PVERBOSE) cout << msg.time()<<" : Peer" <<whoAmI<< " publishes: "<<queries<<endl;
			sendOutput( msg.time(), publish , tosend ); // we output the value in the query set on the query output port
			alldocs.insert(queries); // save it in the collection of already published documents
		}
		else{
			sendOutput( msg.time(), offline, whoAmI );
			officiallyonline=false;
		}

		//the list shouldn't be empty, unless there were no queries to begin with
		break;

	case OFFLINE : //we're going online again
		if(!officiallyonline) {//if we haven't  gone online yet
			sendOutput( msg.time(), online, whoAmI );
			officiallyonline=true;
		}
		else if (!doclist.empty()) //we're publishing stuff
		{
			set<int>::iterator it;
			it = doclist.begin();
			if(PVERBOSE) cout <<msg.time()<< " publishing document : "<< *it << endl;

			//if(identifier + 1 > 999) identifier = -1; //identifier can't currently be more than 3 digits

			int tosend = buildMessage((identifier + 1),*it); // encode query, who am I
			sendOutput( msg.time(), publish , tosend ); // we output the value in the query set on the query output port
			alldocs.insert(*it); // save it in the collection of already published documents
			doclist.erase(it);
			isactive = true;
		}

		break;


	default :
		cout<<"error : undefined state";

	}

	return *this;
}

RandomSurfer::~RandomSurfer()
{
	//delete all statistical distribution objects
	delete timeBetweenQueryDist ;
	delete timeBeforeQueryDist ;
	delete timeAfterQueryDist ;
	delete offlineTimeDist ;
	delete inactiveSessionLength ;
	delete unif_01;
}
