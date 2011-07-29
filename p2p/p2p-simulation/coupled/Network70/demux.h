/*******************************************************************
*
*  DESCRIPTION: Atomic Model : Demultiplexer : gets a message on its single input and
*  forwards it to one of its numerous outputs according to a routing table.
*
*  AUTHOR: Alan
*
*
*******************************************************************/
/*handle duplicate inclusion of this header*/
#ifndef __DEMUX_H
#define __DEMUX_H
#define VERBOSE false

/** include files **/
#include "atomic.h"  // class Atomic
#include "..\complexmessages.h" // functions to manage "coded" messages between components
//#include "NetGraph.h"
//#include "IntSet.h"

#include <map>
#include <set>

//#include "EventQueue.h"
//#include <queue> //STL for queue template class
//#include <set>
/** declarations **/
class Demux: public Atomic
{
public:


	Demux( const string &name = "Demux" ) ;	 // Default constructor

	~Demux();					// Destructor

	virtual string className() const
		{return "Demux";};


protected:
	Model &initFunction()
		{
		return *this;
		};

	Model &externalFunction( const ExternalMessage & );

	Model &internalFunction( const InternalMessage & );

	Model &outputFunction( const InternalMessage & );

private:

    const Port &msg_in ; // for getting messages
	const Port &table_in; // for updating the routing table
	const Port &connect_in; // for connection / disconnection messages
	Port &out_0 ;
	Port &out_c0 ;
	Port &out_1 ;
	Port &out_c1 ;
	Port &out_2 ;
	Port &out_c2 ;
	Port &out_3 ;
	Port &out_c3 ;
	Port &out_4 ;
	Port &out_c4 ;
	Port &out_5 ;
	Port &out_c5 ;
	Port &out_6 ;
	Port &out_c6 ;
	Port &out_7 ;
	Port &out_c7 ;
	Port &out_8 ;
	Port &out_c8 ;
	Port &out_9 ;
	Port &out_c9 ;
	Port &out_10 ;
	Port &out_c10 ;
	Port &out_11 ;
	Port &out_c11 ;
	Port &out_12 ;
	Port &out_c12 ;
	Port &out_13 ;
	Port &out_c13 ;
	Port &out_14 ;
	Port &out_c14 ;
	Port &out_15 ;
	Port &out_c15 ;
	Port &out_16 ;
	Port &out_c16 ;
	Port &out_17 ;
	Port &out_c17 ;
	Port &out_18 ;
	Port &out_c18 ;
	Port &out_19 ;
	Port &out_c19 ;
	Port &out_20 ;
	Port &out_c20 ;
	Port &out_21 ;
	Port &out_c21 ;
	Port &out_22 ;
	Port &out_c22 ;
	Port &out_23 ;
	Port &out_c23 ;
	Port &out_24 ;
	Port &out_c24 ;
	Port &out_25 ;
	Port &out_c25 ;
	Port &out_26 ;
	Port &out_c26 ;
	Port &out_27 ;
	Port &out_c27 ;
	Port &out_28 ;
	Port &out_c28 ;
	Port &out_29 ;
	Port &out_c29 ;
	Port &out_30 ;
	Port &out_c30 ;
	Port &out_31 ;
	Port &out_c31 ;
	Port &out_32 ;
	Port &out_c32 ;
	Port &out_33 ;
	Port &out_c33 ;
	Port &out_34 ;
	Port &out_c34 ;
	Port &out_35 ;
	Port &out_c35 ;
	Port &out_36 ;
	Port &out_c36 ;
	Port &out_37 ;
	Port &out_c37 ;
	Port &out_38 ;
	Port &out_c38 ;
	Port &out_39 ;
	Port &out_c39 ;
	Port &out_40 ;
	Port &out_c40 ;
	Port &out_41 ;
	Port &out_c41 ;
	Port &out_42 ;
	Port &out_c42 ;
	Port &out_43 ;
	Port &out_c43 ;
	Port &out_44 ;
	Port &out_c44 ;
	Port &out_45 ;
	Port &out_c45 ;
	Port &out_46 ;
	Port &out_c46 ;
	Port &out_47 ;
	Port &out_c47 ;
	Port &out_48 ;
	Port &out_c48 ;
	Port &out_49 ;
	Port &out_c49 ;
	Port &out_50 ;
	Port &out_c50 ;
	Port &out_51 ;
	Port &out_c51 ;
	Port &out_52 ;
	Port &out_c52 ;
	Port &out_53 ;
	Port &out_c53 ;
	Port &out_54 ;
	Port &out_c54 ;
	Port &out_55 ;
	Port &out_c55 ;
	Port &out_56 ;
	Port &out_c56 ;
	Port &out_57 ;
	Port &out_c57 ;
	Port &out_58 ;
	Port &out_c58 ;
	Port &out_59 ;
	Port &out_c59 ;
	Port &out_60 ;
	Port &out_c60 ;
	Port &out_61 ;
	Port &out_c61 ;
	Port &out_62 ;
	Port &out_c62 ;
	Port &out_63 ;
	Port &out_c63 ;
	Port &out_64 ;
	Port &out_c64 ;
	Port &out_65 ;
	Port &out_c65 ;
	Port &out_66 ;
	Port &out_c66 ;
	Port &out_67 ;
	Port &out_c67 ;
	Port &out_68 ;
	Port &out_c68 ;
	Port &out_69 ;
	Port &out_c69 ;
	// table to associate message Ids to peers (conceptually for routing responses back to the peer that originated the query).
	map<int,int > routingTable;

	bool routing; // if we're routing a queryhit
	bool connecting; // if we're routing a connect/ disconnect
	set<int> to_peers; //the set of peers that the next output is meant for
	int nextOutput; // next output

	//map<float, Time> a time stamp for the last seen message for a given id : the idea is that after a while we're not routing the message anymore and can forget about which nodes the message has visited.

	//Time lastpurge // stores the time of the last purge. If this occured more than <purgeinterval> ago we should re-purge the routingtable of old messages



};	// class Demux


#endif    //__DEMUX_H
