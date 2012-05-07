/*******************************************************************
*
*  DESCRIPTION: Atomic Model :
*  Repository : maintains a mock-up Repository for the whole network and responds to queries
*
*  AUTHOR: Denis
*
*
*******************************************************************/
/*handle duplicate inclusion of this header*/
#ifndef __DB_H
#define __DB_H
#define SVERBOSE false
#define MAXDOCS 10

/** include files **/
#include "atomic.h"  // class Atomic
#include "..\complexmessages.h" // functions to manage "coded" messages between components
#include "..\complexmessages2.h"
#include "..\..\lib\graph-c++.h" // classes to handle the graph (network)
#include "DBGraph.h"
#include "SGraph.h"
#include <queue>
#include<map>
#include <set>
#include <list>
/** declarations **/
class Repository: public Atomic
{
public:


	Repository( const string &name = "Repository" ) ;	 // Default constructor

	~Repository();					// Destructor

	virtual string className() const
		{return "Repository";};


protected:
	Model &initFunction()
		{
		return *this;
		};

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:

	/**************** PORTS *********************************************/
    const Port &peer_in ;


	const Port &publish;
	const Port &remove;




	//outputs
	Port &queryhit ;
	Port &p_out;
	Port &link;
	/*******************************************************************/

	int capacity;

	queue<long long> QueryhitQ; //queue of queryhits that need to be output
	queue<long long> publishQ; //queue of all the documents being published
	list<int> currentDocs; //list of all documents published by a peer
	queue<long long> docCheckQ;
	bool docFound;



};	// class Repository


#endif    //__DB_H
