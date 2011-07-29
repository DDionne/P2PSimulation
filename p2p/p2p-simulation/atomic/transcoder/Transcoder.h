/*******************************************************************
*
*  DESCRIPTION: Atomic Model : Transcoder : encodes messages going in and out of peers
*  to/from a generic format understood by the network. Mostly this requires adding /removing the peerId
*
*  AUTHOR: Alan
*
*
*******************************************************************/
/*handle duplicate inclusion of this header*/
#ifndef __TRANS_H
#define __TRANS_H
#define VERBOSE true

/** include files **/
#include "atomic.h"  // class Atomic
#include "..\complexmessages.h" // functions to manage "coded" messages between components
//#include "NetGraph.h"
//#include "IntSet.h"

#include<map>

//#include "EventQueue.h"
//#include <queue> //STL for queue template class
//#include <set>
/** declarations **/
class Demux: public Atomic
{
public:


	Transcoder( const string &name = "Transcoder" ) ;	 // Default constructor

	~Transcoder();					// Destructor

	virtual string className() const
		{return "Transcoder";};


protected:
	Model &initFunction()
		{
		return *this;
		};

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:

	//all the incoming ports
	const Port &query1 ;
	const Port &publish1;
	const Port &remove1 ;
	const Port &connect1 ;
	const Port &disconnect1 ;
	const Port &online1 ;
	const Port &offline1 ;
	const Port &inconnect1;
	const Port &indisconnect1;

    //outgoing
    Port &query2 ;
    Port &publish2;
    Port &connect2 ;
    Port &remove2 ;
    Port &disconnect1 ;
    Port &online2 ;
    Port &offline2 ;
	Port &inconnect2;
	Port &indisconnect2;

//..Port &out_k

	Port &next_port_out; // the next output port, one of the above
	int nextOutput; // next output

};	// class Transcoder


#endif    //__TRANS_H
