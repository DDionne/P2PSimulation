/*******************************************************************
 *
 *  DESCRIPTION: Atomic Model RandomSurfer2 : The Random Surfer represents the
 *  behavior of the human user of the P2P application. The outputs of this
 *  model are the events initiated by a human user: the peer begins a session
 *  and goes online, creates queries, or manages its local repository,
 *  by publishing or removing documents.
 *
 *  In addition, the Random Surfer has a good chance to publish or visit
 *  documents that are linked to by a previously visited document.
 *
 *  AUTHOR: Alan
 *
 *  Edited from SessionManager to RandomSurfer2 by: Denis
 *
 *  DATE: November 2010 - May 2011
 *  Edited: June 2011 - July 2011
 *
 *******************************************************************/

/** include files **/
#include "RandomSurfer2.h"  // base header
#include <math.h>            // fabs( ... )
#include <stdlib.h>
#include "randlib.h"         // Random numbers library
#include "message.h"       // InternalMessage ....
#include "distri.h"        // class Distribution
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )
#include <sstream>
#include <fstream> // i o streaming from files


/*******************************************************************
 * Function Name: RandomSurfer2
 * Description: constructor
 ********************************************************************/
RandomSurfer2::RandomSurfer2( const string &name )
: Atomic( name )
, queryhit( addInputPort( "queryhit" ) )
, query( addOutputPort( "query" ) )
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

	string TTL_str = MainSimulator::Instance().getParameter( description(), "TimeToLive" );
	TTL = int(floor(str2float(TTL_str)));



	///----------------------------------------------------
	//list of published documents
	param = MainSimulator::Instance().getParameter( description(), "doclist" );

	istringstream sis2;
	sis2.str(param); // open stream to string


	// read list of documents
	while(true)
	{
		int w;
		char coma;
		sis2 >> w;
		if (w==-1)
			break; //-1 stands for an empty list
		doclist.insert(w);
		if(!sis2.eof())
			sis2 >> coma; //separating from next value (if there is one)
		else
			break; //otherwise we're done
	}




	string param2 = MainSimulator::Instance().getParameter( description(), "friendlist" );

	istringstream sis;
		sis.str(param2); // open stream to string

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
		 * 					offlinetimedistribution
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

		offlineTimeDist = Distribution::create( MainSimulator::Instance().getParameter( description(), "offlinetimedistribution" ) );
		for ( register int i = 0 ; i < offlineTimeDist->varCount() ; i++ )
		{
			string parameter( MainSimulator::Instance().getParameter( description(), "offlinetime"+offlineTimeDist->getVar(i) ) ) ;
			offlineTimeDist->setVar( i, str2float( parameter ) ) ;


		}



		unif_01 = Distribution::create("uniform");
		unif_01-> setVar(0,0); //min if first parameter, set to 0
		unif_01-> setVar(1,1); //max is 2nd parameter, set to 1

		activityProb = str2float(MainSimulator::Instance().getParameter( description(), "probabilityofactivity" ) );
		ignoreProb = str2float(MainSimulator::Instance().getParameter(description(), "probabilityToIgnore"));


		nextchange = Time(0,0,0,0);
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

Model &RandomSurfer2::initFunction()
{

	//stay in the active state for some random time, before going online. Use offline distribution
	float t = max(static_cast<float>( distribution(OFFLINE).get() ), 0.0f); //low-bound distribution by zero

	mystate = OFFLINE;
	officiallyonline=false; //start offline



	if(PVERBOSE) cout<<endl<<"Peer"<<whoAmI<<" about to start with waiting time: "<<t<<endl;
	Time toto = makeTimefromSeconds( t );
//	Time toto = Time(0,0,0,1);
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
Model &RandomSurfer2::externalFunction( const ExternalMessage &msg )
{

	if (this->state() != passive && mystate != OFFLINE) //if I'm not offline I'll do something about the queryhit
	{

		if(PVERBOSE) cout<<msg.time()<<" Peer "<<whoAmI<<" got queryhit : "<<msg.value()<<endl;





		//Add document to the set of possible choices
		int thedoc = getFirstField(msg.value()); //Document Id
		int messageID = getSecondField(msg.value()); //Message Identifier
		int peerId = getThirdField(msg.value()); //peer that has the document


		if(PVERBOSE) cout << "		document: " << thedoc << endl;

		set<int>::iterator friends = friendlist.find(peerId);
		if(friends != friendlist.end()){
			friendDocs.insert(thedoc);
			if(PVERBOSE) cout << "		ADDING: " << thedoc << endl;
		}

		linkChoices.push_front(thedoc);

		nextchange = nextChange();
		holdIn(active,nextchange);



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
Model &RandomSurfer2::internalFunction( const InternalMessage &im )
{

	float t;
	int queries;
	switch(mystate){
	case SESSION : //when session runs out, surfer goes offline
		if(PVERBOSE) cout<<im.time()<<"   state: SESSION"<<endl;
		if(PVERBOSE) cout << im.time() << "          "<<stop<<endl;


	//Make surfer stay in the SESSION state for 5 seconds before going to the QUERYHIT state
		if(im.time() > stop){
			mystate = OFFLINE;
			t = max(static_cast<float>( distribution(OFFLINE).get() ), 0.0f);
			holdIn(active, makeTimefromSeconds(t));
		}
		else{
			mystate = QUERYHIT;
			holdIn(active, Time(0,0,5,0));
		}
		break;


	case QUERYHIT :
		if(PVERBOSE) cout<<im.time()<<"   state: QUERYHIT"<<endl;

	//Make surfer wait 5 seconds in the QUERYHIT state, then go to the SESSION state
		if(im.time() > stop){
			mystate = OFFLINE;
			t = max(static_cast<float>( distribution(OFFLINE).get() ), 0.0f);
			holdIn(active, makeTimefromSeconds(t));
		}
		else{
			mystate = SESSION;
			holdIn(active, Time(0,0,5,0));
		}
		break;


	case OFFLINE : //we're going online again
		if(PVERBOSE) cout<<im.time()<<"   state: OFFLINE"<<endl;


		if(!doclist.empty()){ //publish all docs, if necessary
			holdIn(active, Time(0,0,0,2 )); //wait 2ms between publishing docs
		} else { //go online

			mystate =  SESSION; //active session

			t = max(static_cast<float>( distribution(SESSION).get() ), 0.0f);
			start = makeTimefromSeconds(t);
			//cout << "session time: "<<start<<endl;
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
Model &RandomSurfer2::outputFunction( const InternalMessage &msg )
{

	switch(mystate){
	case SESSION :
		if(msg.time() > stop){
			if(PVERBOSE) cout << "Peer "<<whoAmI<<" GOING OFFLINE"<<endl;
			sendOutput( msg.time(), offline, whoAmI );
			officiallyonline=false;
		}

		break;

	case QUERYHIT :
		if(PVERBOSE) cout << "QUERYHIT OUTPUT"<<endl;
		if(!linkChoices.empty() && msg.time() < stop){ // a document to publish following a queryhit


			float randomness = unif_01->get();
			long long tosend;
			if(PVERBOSE)cout <<msg.time()<< " ignore/publish   choice: "<< randomness << " vs. " << ignoreProb << endl;


			if(randomness < ignoreProb){ //we check whether the random surfer is going to publish the file or simply ignore it
				if(PVERBOSE) cout << msg.time()<<" ignoring links"<<endl;
				int queries = (unif_01->get()*maxQuery)+1; //randomly chooses a possible query
				long long tosend2 = buildNewMessage(queries,2,0,whoAmI,0,TTL);
				if(PVERBOSE) cout << msg.time()<<" : Peer" <<whoAmI<< " queries: "<<queries<<endl;
				sendOutput( msg.time(), query , tosend2 ); // we output the value in the query set on the query output port
				alldocs.insert(queries); // save it in the collection of already published documents
			}
			else{



				set<int> temp = MostQhitfromFriends(1,linkChoices, friendDocs, true); //make a set of all documents that are ranked the highest
				int a;
				if(temp.size() != 1)	//if there is more than 1 document in the list
				{

					int nbrOfLinks = temp.size();
					int linkPicker = unif_01->get()*(nbrOfLinks);
					set<int>::iterator link = temp.begin();
					advance(link,linkPicker); //iterate and stop randomly in the set to pick a link
					set<int>::iterator doc = friendDocs.find(*link);
					a = *link;

				}

				else{ //there is only one document in the list (no need to randomly choose)
					set<int>::iterator iter = temp.begin();
					a = (*iter);
				}

				if(PVERBOSE) cout << msg.time()<<" : Peer" <<whoAmI<< " publishes: "<< a <<endl;
				tosend = buildNewMessage(a, 0, 0, whoAmI, 0, TTL);
				sendOutput( msg.time(), query, tosend);
				alldocs.insert(a); // save it in the collection of already published documents








			}
			linkChoices.clear();
			friendDocs.clear();	//empty the list of documents that came from friends (so that we don't overlap
								//on the next set of queryhits

		}
		else if(msg.time() < stop){
			int queries = (unif_01->get()*maxQuery)+1; //randomly chooses a possible query
			long long tosend = buildNewMessage(queries,2,0,whoAmI,0,TTL);
	//		if(PVERBOSE) cout << msg.time()<<" : output message: "<<tosend<<endl;
	//		if(PVERBOSE) cout << msg.time()<<" : Peer" <<whoAmI<< " publishes: "<<queries<<endl;
			sendOutput( msg.time(), query , tosend ); // we output the value in the query set on the query output port
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
	//		if(PVERBOSE) cout <<msg.time()<< " publishing document : "<< *it << endl;
			sendOutput( msg.time(), publish , buildNewMessage(*it,0,0,whoAmI,0,0)); // we output the value in the query set on the query output port
			alldocs.insert(*it); // save it in the collection of already published documents
			doclist.erase(it);
		}

		break;


	default :
		cout<<"error : undefined state";

	}

	return *this;
}

RandomSurfer2::~RandomSurfer2()
{
	//delete all statistical distribution objects
	delete timeBetweenQueryDist ;
	delete timeBeforeQueryDist ;
	delete offlineTimeDist ;
	delete unif_01;
}
