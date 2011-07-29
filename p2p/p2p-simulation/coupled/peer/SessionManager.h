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
#define PVERBOSE true

#define BEFOREQUERY 0
#define INTERQUERY 1
#define AFTERQUERY 2
#define OFFLINETIME 3
#define INACTIVETIME 4

/** include files **/
#include "atomic.h"  // class Atomic
#include <set>
#include "..\complexmessages.h" // functions to manage "coded" messages between components

/** forward declarations **/
class Distribution ;

/** declarations **/
class SessionManager: public Atomic
{
public:
	SessionManager( const string &name = "SessionManager" ) ;	 // Default constructor

	~SessionManager();					// Destructor

	virtual string className() const
		{return "SessionManager";}

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

    int whoAmI;
    int mystate; //can be passive, offline, activebeforequery, activebetweenquery, activeafterquery, inactivesession
    bool officiallyonline;//have I output a message announcing myself to be online / offline ?

    set<int> querylist;
    set<int> savequerylist;
    set<int> doclist;
    set<int> alldocs; //docs already published

    //statistical distributions for time-related behavior
    Distribution *timeBetweenQueryDist ;
    Distribution *timeBeforeQueryDist ;
    Distribution *timeAfterQueryDist ;
    Distribution *offlineTimeDist ;
    Distribution *inactiveSessionLength ;

    //Distribution for flipping a coin

    Distribution * unif_01;

    float activityProb; // probability of query activity : configurable

    Distribution &distribution(int which)
    {

    	switch(which){
    	case BEFOREQUERY :
    		return * timeBeforeQueryDist;
    	case INTERQUERY :
    		return *timeBetweenQueryDist ;
    	case AFTERQUERY :
    		return *timeAfterQueryDist ;
    	case OFFLINETIME :
    		return *offlineTimeDist ;
    	case INACTIVETIME :
    		return *inactiveSessionLength ;

    	}
    }
};	// class SessionManager


#endif    //__SESSMANAGER_H
