/*******************************************************************
* Atomic Model : Connection Manager : part of a peer that manages the connections to other peers
*
*  AUTHOR: Alan
*
*
*  DATE: November 2010
*
*******************************************************************/
#ifndef __SESSMANAGER_H
#define __SESSMANAGER_H
#define PVERBOSE false
#define OFFLINE 0
#define SESSION 1
#define QUERYHIT 2

/** include files **/
#include "atomic.h"  // class Atomic
#include "Ranking.h"
#include <set>
#include <map>
#include <list>
#include "..\complexmessages.h" // functions to manage "coded" messages between components
#include "..\complexmessages2.h"


/** forward declarations **/
class Distribution ;

/** declarations **/
class RandomSurfer2: public Atomic
{
public:
	RandomSurfer2( const string &name = "RandomSurfer2" ) ;	 // Default constructor

	~RandomSurfer2();					// Destructor

	virtual string className() const
		{return "RandomSurfer2";}

protected:
	Model &initFunction();

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:
	const Port &queryhit ;
	Port & online ;
    Port & offline;
    Port & query;
    Port & publish;
    Port & remove;

    int TTL;
    int whoAmI;
    Time nextchange;
    int mystate; //can be passive, offline, session or queryhit
    bool officiallyonline;//have I output a message announcing myself to be online / offline ?

    int maxQuery;
    Time start;
    Time stop;
    set<int> friendlist;
    set<int> doclist;
    set<int> alldocs; //docs already published
    map<int,int> peerDocMap;
    list<int> linkChoices; //links which the surfer chooses from
    set<int> friendDocs;



    //statistical distributions for time-related behavior
    Distribution *timeBetweenQueryDist ;
    Distribution *timeBeforeQueryDist ;
    Distribution *offlineTimeDist ;

    //Distribution for flipping a coin

    Distribution * unif_01;

    float activityProb; // probability of query activity : configurable
    float ignoreProb; // probability of ignoring query : configurable  -> currently 50/50 chance probability

    Distribution &distribution(int which)
    {

    	switch(which){

    	case OFFLINE :
    		return *offlineTimeDist ;
    	case SESSION :
    		return *timeBeforeQueryDist;
    	case QUERYHIT :
    		return *timeBetweenQueryDist;

    	}
    }
};	// class RandomSurfer2


#endif    //__SESSMANAGER_H
