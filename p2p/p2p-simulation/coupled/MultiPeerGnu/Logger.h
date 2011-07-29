/*******************************************************************
* Atomic Model : Logger, outputs publish, connect, disconnect, online and offline to the Top Model
*
*  AUTHOR: Denis
*
*
*  DATE: July 2011
*
*******************************************************************/
#ifndef __LOGGER_H
#define __LOGGER_H
#define LogVERBOSE true

/** include files **/
#include "atomic.h"  // class Atomic
#include "..\complexmessages2.h" // functions to manage "coded" messages between components
#include<queue>


/** forward declarations **/
class Distribution ;


/** declarations **/
class Logger: public Atomic
{
public:
	Logger( const string &name = "Logger" ) ;	 // Default constructor

	~Logger();					// Destructor

	virtual string className() const
		{return "Logger";}

protected:
	Model &initFunction();

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:
	const Port &in_connect ;
	const Port &in_publish ;
	const Port &in_disconnect ;
	const Port &in_online ;
	const Port &in_offline ;
	const Port &qhit_in;
	const Port &in_query;
	const Port &queryReached;
    Port &online;
    Port &offline;
    Port &publish;
    Port &connect;
    Port &disconnect;



};	// class Logger


#endif    //__Logger_H
