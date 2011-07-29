/*******************************************************************
*
*  DESCRIPTION: Atomic Model : Demultiplexer : gets a message on its single input and
*  forwards it to one of its numerous outputs according to a routing table.
*
*  AUTHOR: Alan
*
*
*  DATE: November 2010
*
*******************************************************************/

/** include files **/
#include <math.h>            // fabs( ... )
#include <stdlib.h>
#include "randlib.h"         // Random numbers library
#include "demux.h"  // base header
#include "message.h"       // InternalMessage ....
//#include "..\..\lib\graph-c++.h"        // class graph
//#include "NetGraph.h"        // basic undirected graph with integers
#include "mainsimu.h"      // class MainSimulator
#include "strutil.h"       // str2float( ... )



/*******************************************************************
* Function Name: Demux
* Description: constructor
********************************************************************/
Demux::Demux( const string &name )
: Atomic( name )
, msg_in( addInputPort( "msg_in" ) )
, table_in( addInputPort( "table_in" ) )
, connect_in( addInputPort( "connect_in" ) ), out_0( addOutputPort( "out_0" ) )
, out_c0( addOutputPort( "out_c0" ) )
, out_1( addOutputPort( "out_1" ) )
, out_c1( addOutputPort( "out_c1" ) )
, out_2( addOutputPort( "out_2" ) )
, out_c2( addOutputPort( "out_c2" ) )
, out_3( addOutputPort( "out_3" ) )
, out_c3( addOutputPort( "out_c3" ) )
, out_4( addOutputPort( "out_4" ) )
, out_c4( addOutputPort( "out_c4" ) )
, out_5( addOutputPort( "out_5" ) )
, out_c5( addOutputPort( "out_c5" ) )
, out_6( addOutputPort( "out_6" ) )
, out_c6( addOutputPort( "out_c6" ) )
, out_7( addOutputPort( "out_7" ) )
, out_c7( addOutputPort( "out_c7" ) )
, out_8( addOutputPort( "out_8" ) )
, out_c8( addOutputPort( "out_c8" ) )
, out_9( addOutputPort( "out_9" ) )
, out_c9( addOutputPort( "out_c9" ) )
, out_10( addOutputPort( "out_10" ) )
, out_c10( addOutputPort( "out_c10" ) )
, out_11( addOutputPort( "out_11" ) )
, out_c11( addOutputPort( "out_c11" ) )
, out_12( addOutputPort( "out_12" ) )
, out_c12( addOutputPort( "out_c12" ) )
, out_13( addOutputPort( "out_13" ) )
, out_c13( addOutputPort( "out_c13" ) )
, out_14( addOutputPort( "out_14" ) )
, out_c14( addOutputPort( "out_c14" ) )
, out_15( addOutputPort( "out_15" ) )
, out_c15( addOutputPort( "out_c15" ) )
, out_16( addOutputPort( "out_16" ) )
, out_c16( addOutputPort( "out_c16" ) )
, out_17( addOutputPort( "out_17" ) )
, out_c17( addOutputPort( "out_c17" ) )
, out_18( addOutputPort( "out_18" ) )
, out_c18( addOutputPort( "out_c18" ) )
, out_19( addOutputPort( "out_19" ) )
, out_c19( addOutputPort( "out_c19" ) )
, out_20( addOutputPort( "out_20" ) )
, out_c20( addOutputPort( "out_c20" ) )
, out_21( addOutputPort( "out_21" ) )
, out_c21( addOutputPort( "out_c21" ) )
, out_22( addOutputPort( "out_22" ) )
, out_c22( addOutputPort( "out_c22" ) )
, out_23( addOutputPort( "out_23" ) )
, out_c23( addOutputPort( "out_c23" ) )
, out_24( addOutputPort( "out_24" ) )
, out_c24( addOutputPort( "out_c24" ) )
, out_25( addOutputPort( "out_25" ) )
, out_c25( addOutputPort( "out_c25" ) )
, out_26( addOutputPort( "out_26" ) )
, out_c26( addOutputPort( "out_c26" ) )
, out_27( addOutputPort( "out_27" ) )
, out_c27( addOutputPort( "out_c27" ) )
, out_28( addOutputPort( "out_28" ) )
, out_c28( addOutputPort( "out_c28" ) )
, out_29( addOutputPort( "out_29" ) )
, out_c29( addOutputPort( "out_c29" ) )
, out_30( addOutputPort( "out_30" ) )
, out_c30( addOutputPort( "out_c30" ) )
, out_31( addOutputPort( "out_31" ) )
, out_c31( addOutputPort( "out_c31" ) )
, out_32( addOutputPort( "out_32" ) )
, out_c32( addOutputPort( "out_c32" ) )
, out_33( addOutputPort( "out_33" ) )
, out_c33( addOutputPort( "out_c33" ) )
, out_34( addOutputPort( "out_34" ) )
, out_c34( addOutputPort( "out_c34" ) )
, out_35( addOutputPort( "out_35" ) )
, out_c35( addOutputPort( "out_c35" ) )
, out_36( addOutputPort( "out_36" ) )
, out_c36( addOutputPort( "out_c36" ) )
, out_37( addOutputPort( "out_37" ) )
, out_c37( addOutputPort( "out_c37" ) )
, out_38( addOutputPort( "out_38" ) )
, out_c38( addOutputPort( "out_c38" ) )
, out_39( addOutputPort( "out_39" ) )
, out_c39( addOutputPort( "out_c39" ) )
, out_40( addOutputPort( "out_40" ) )
, out_c40( addOutputPort( "out_c40" ) )
, out_41( addOutputPort( "out_41" ) )
, out_c41( addOutputPort( "out_c41" ) )
, out_42( addOutputPort( "out_42" ) )
, out_c42( addOutputPort( "out_c42" ) )
, out_43( addOutputPort( "out_43" ) )
, out_c43( addOutputPort( "out_c43" ) )
, out_44( addOutputPort( "out_44" ) )
, out_c44( addOutputPort( "out_c44" ) )
, out_45( addOutputPort( "out_45" ) )
, out_c45( addOutputPort( "out_c45" ) )
, out_46( addOutputPort( "out_46" ) )
, out_c46( addOutputPort( "out_c46" ) )
, out_47( addOutputPort( "out_47" ) )
, out_c47( addOutputPort( "out_c47" ) )
, out_48( addOutputPort( "out_48" ) )
, out_c48( addOutputPort( "out_c48" ) )
, out_49( addOutputPort( "out_49" ) )
, out_c49( addOutputPort( "out_c49" ) )
, out_50( addOutputPort( "out_50" ) )
, out_c50( addOutputPort( "out_c50" ) )
, out_51( addOutputPort( "out_51" ) )
, out_c51( addOutputPort( "out_c51" ) )
, out_52( addOutputPort( "out_52" ) )
, out_c52( addOutputPort( "out_c52" ) )
, out_53( addOutputPort( "out_53" ) )
, out_c53( addOutputPort( "out_c53" ) )
, out_54( addOutputPort( "out_54" ) )
, out_c54( addOutputPort( "out_c54" ) )
, out_55( addOutputPort( "out_55" ) )
, out_c55( addOutputPort( "out_c55" ) )
, out_56( addOutputPort( "out_56" ) )
, out_c56( addOutputPort( "out_c56" ) )
, out_57( addOutputPort( "out_57" ) )
, out_c57( addOutputPort( "out_c57" ) )
, out_58( addOutputPort( "out_58" ) )
, out_c58( addOutputPort( "out_c58" ) )
, out_59( addOutputPort( "out_59" ) )
, out_c59( addOutputPort( "out_c59" ) )
, out_60( addOutputPort( "out_60" ) )
, out_c60( addOutputPort( "out_c60" ) )
, out_61( addOutputPort( "out_61" ) )
, out_c61( addOutputPort( "out_c61" ) )
, out_62( addOutputPort( "out_62" ) )
, out_c62( addOutputPort( "out_c62" ) )
, out_63( addOutputPort( "out_63" ) )
, out_c63( addOutputPort( "out_c63" ) )
, out_64( addOutputPort( "out_64" ) )
, out_c64( addOutputPort( "out_c64" ) )
, out_65( addOutputPort( "out_65" ) )
, out_c65( addOutputPort( "out_c65" ) )
, out_66( addOutputPort( "out_66" ) )
, out_c66( addOutputPort( "out_c66" ) )
, out_67( addOutputPort( "out_67" ) )
, out_c67( addOutputPort( "out_c67" ) )
, out_68( addOutputPort( "out_68" ) )
, out_c68( addOutputPort( "out_c68" ) )
, out_69( addOutputPort( "out_69" ) )
, out_c69( addOutputPort( "out_c69" ) )
{
	//initialising these values... not indispensable but always safer
	routing = false;
	nextOutput= 0;
}



/*******************************************************************
* Function Name: externalFunction
* Description: the router gets input from either the "outside" (a new messgae to route) or from the router (next step for routing)
********************************************************************/
Model &Demux::externalFunction( const ExternalMessage &msg ){
	if(VERBOSE) cout<<"external message: "<<msg.value()<<endl;
	if ( this->state() == passive)
	{
		if (msg.port() == table_in) //putting something in the routing table
		{
			//expecting a message including the peerid and a messageId encoded according to the spec of "complexmessages.h"

			int peerid = getPeerId(msg.value());// get originating peer (from value of external msg)
			if(VERBOSE)cout<<" peerid:"<<peerid<<endl;
			int id = getMessageId(msg.value()); // get message id
			if(VERBOSE)cout<<"  message id:"<<id<<endl;

			//create "mapping entry" list. The message ids are float values (that way we can generate them using a random function)
			// to create an empty list I use the default constructor, throught the shortcut of calling the [] operator.
			routingTable[id]=peerid; //associates peerid to id in routing table.
			if(VERBOSE)cout<<"  RoutingTable id:"<<id<<"associated with peer:"<<routingTable[id]<<endl;

		}


		else if (msg.port() ==  msg_in){
			//expecting value = TTL * 100 + peerid + id, where id is the decimal part (<1)
			if(VERBOSE) cout<<"message to be demultiplexed... "<<msg.value()<<"\n";

			long inval = msg.value();

			//extract our 3 values using static functions from complexmessages.h
			//HACK : the msg payload will be encoded in the space meant for the peerId
			int payload = getPeerId(inval);
			if(VERBOSE)cout<<" message payload:"<<payload<<endl;
			int id = getMessageId(inval);
			if(VERBOSE)cout<<"  message id:"<<id<<endl;

			//search for "peerid" in set mapped to id in routing table
			map<int,int>::iterator finder =routingTable.find(id);
			if (!(finder==routingTable.end())){
			routing = true;
			to_peers.clear();
			to_peers.insert(finder->second); //finder accesses both key and value
			} else{
				cout<<"DEMUX::Error : msgid -> peer not set";
			}
			nextOutput = inval;//nextOutput = buildMessage(id, payload); // put the payload back in the message in place of the peerid
		}

		else if (msg.port() ==  connect_in){
			//expecting value = TTL ;  peer1id + peer2id
			if(VERBOSE) cout<<"connection message "<<msg.value()<<"\n";

			long inval = msg.value();

			//extract our 3 values using static functions from complexmessages.h
			//HACK : the msg payload will be encoded in the space meant for the peerId
			int p1 = getPeerId(inval);

			int p2 = getMessageId(inval);
			int ttl = getTTL(inval); //=1 if this is a connect message, 0 for disconnect
			//if(VERBOSE)cout<<"  message id:"<<id<<endl;

			//check for already visited peer : search for "peerid" in set mapped to id in routing table
			to_peers.clear();
			to_peers.insert(p1);
			to_peers.insert(p2);
			//TODO : what does this return if the msgid is not in the table ?
			connecting = true;
			nextOutput = buildMessage(p1, p2,ttl); // put the payload back in the message in place of the peerid
		}

	} //end if state is passive
	else{
		cout<<"error: message received while in active state"<<endl;
	}
	// we have an instantaneous change back to the passive state (will output the next output values where relevant)
	holdIn( active, Time(0.00f));

	return *this ;
}


/*******************************************************************
* Function Name: internalFunction
********************************************************************/
Model &Demux::internalFunction( const InternalMessage & ){

	//set these back
	routing = false;
	connecting = false;
	passivate(); // we just passivate immediately

	return *this;
}

/*******************************************************************
 * Function Name: outputFunction
 ********************************************************************/
Model &Demux::outputFunction( const InternalMessage &msg )
{
	if(VERBOSE) cout<<"output coming...\n";
	if (routing) // if we have anything to output
	{
		set<int>::iterator it= to_peers.begin(); // there's just one
		//NextOutput contains the next message to be output, to_peer the integer identifying the peer that the message is for
		switch ( *it ) {
			case 0:
				 sendOutput( msg.time(), out_0, nextOutput);
				break;
			case 1:
				 sendOutput( msg.time(), out_1, nextOutput);
				break;
			case 2:
				 sendOutput( msg.time(), out_2, nextOutput);
				break;
			case 3:
				 sendOutput( msg.time(), out_3, nextOutput);
				break;
			case 4:
				 sendOutput( msg.time(), out_4, nextOutput);
				break;
			case 5:
				 sendOutput( msg.time(), out_5, nextOutput);
				break;
			case 6:
				 sendOutput( msg.time(), out_6, nextOutput);
				break;
			case 7:
				 sendOutput( msg.time(), out_7, nextOutput);
				break;
			case 8:
				 sendOutput( msg.time(), out_8, nextOutput);
				break;
			case 9:
				 sendOutput( msg.time(), out_9, nextOutput);
				break;
			case 10:
				 sendOutput( msg.time(), out_10, nextOutput);
				break;
			case 11:
				 sendOutput( msg.time(), out_11, nextOutput);
				break;
			case 12:
				 sendOutput( msg.time(), out_12, nextOutput);
				break;
			case 13:
				 sendOutput( msg.time(), out_13, nextOutput);
				break;
			case 14:
				 sendOutput( msg.time(), out_14, nextOutput);
				break;
			case 15:
				 sendOutput( msg.time(), out_15, nextOutput);
				break;
			case 16:
				 sendOutput( msg.time(), out_16, nextOutput);
				break;
			case 17:
				 sendOutput( msg.time(), out_17, nextOutput);
				break;
			case 18:
				 sendOutput( msg.time(), out_18, nextOutput);
				break;
			case 19:
				 sendOutput( msg.time(), out_19, nextOutput);
				break;
			case 20:
				 sendOutput( msg.time(), out_20, nextOutput);
				break;
			case 21:
				 sendOutput( msg.time(), out_21, nextOutput);
				break;
			case 22:
				 sendOutput( msg.time(), out_22, nextOutput);
				break;
			case 23:
				 sendOutput( msg.time(), out_23, nextOutput);
				break;
			case 24:
				 sendOutput( msg.time(), out_24, nextOutput);
				break;
			case 25:
				 sendOutput( msg.time(), out_25, nextOutput);
				break;
			case 26:
				 sendOutput( msg.time(), out_26, nextOutput);
				break;
			case 27:
				 sendOutput( msg.time(), out_27, nextOutput);
				break;
			case 28:
				 sendOutput( msg.time(), out_28, nextOutput);
				break;
			case 29:
				 sendOutput( msg.time(), out_29, nextOutput);
				break;
			case 30:
				 sendOutput( msg.time(), out_30, nextOutput);
				break;
			case 31:
				 sendOutput( msg.time(), out_31, nextOutput);
				break;
			case 32:
				 sendOutput( msg.time(), out_32, nextOutput);
				break;
			case 33:
				 sendOutput( msg.time(), out_33, nextOutput);
				break;
			case 34:
				 sendOutput( msg.time(), out_34, nextOutput);
				break;
			case 35:
				 sendOutput( msg.time(), out_35, nextOutput);
				break;
			case 36:
				 sendOutput( msg.time(), out_36, nextOutput);
				break;
			case 37:
				 sendOutput( msg.time(), out_37, nextOutput);
				break;
			case 38:
				 sendOutput( msg.time(), out_38, nextOutput);
				break;
			case 39:
				 sendOutput( msg.time(), out_39, nextOutput);
				break;
			case 40:
				 sendOutput( msg.time(), out_40, nextOutput);
				break;
			case 41:
				 sendOutput( msg.time(), out_41, nextOutput);
				break;
			case 42:
				 sendOutput( msg.time(), out_42, nextOutput);
				break;
			case 43:
				 sendOutput( msg.time(), out_43, nextOutput);
				break;
			case 44:
				 sendOutput( msg.time(), out_44, nextOutput);
				break;
			case 45:
				 sendOutput( msg.time(), out_45, nextOutput);
				break;
			case 46:
				 sendOutput( msg.time(), out_46, nextOutput);
				break;
			case 47:
				 sendOutput( msg.time(), out_47, nextOutput);
				break;
			case 48:
				 sendOutput( msg.time(), out_48, nextOutput);
				break;
			case 49:
				 sendOutput( msg.time(), out_49, nextOutput);
				break;
			case 50:
				 sendOutput( msg.time(), out_50, nextOutput);
				break;
			case 51:
				 sendOutput( msg.time(), out_51, nextOutput);
				break;
			case 52:
				 sendOutput( msg.time(), out_52, nextOutput);
				break;
			case 53:
				 sendOutput( msg.time(), out_53, nextOutput);
				break;
			case 54:
				 sendOutput( msg.time(), out_54, nextOutput);
				break;
			case 55:
				 sendOutput( msg.time(), out_55, nextOutput);
				break;
			case 56:
				 sendOutput( msg.time(), out_56, nextOutput);
				break;
			case 57:
				 sendOutput( msg.time(), out_57, nextOutput);
				break;
			case 58:
				 sendOutput( msg.time(), out_58, nextOutput);
				break;
			case 59:
				 sendOutput( msg.time(), out_59, nextOutput);
				break;
			case 60:
				 sendOutput( msg.time(), out_60, nextOutput);
				break;
			case 61:
				 sendOutput( msg.time(), out_61, nextOutput);
				break;
			case 62:
				 sendOutput( msg.time(), out_62, nextOutput);
				break;
			case 63:
				 sendOutput( msg.time(), out_63, nextOutput);
				break;
			case 64:
				 sendOutput( msg.time(), out_64, nextOutput);
				break;
			case 65:
				 sendOutput( msg.time(), out_65, nextOutput);
				break;
			case 66:
				 sendOutput( msg.time(), out_66, nextOutput);
				break;
			case 67:
				 sendOutput( msg.time(), out_67, nextOutput);
				break;
			case 68:
				 sendOutput( msg.time(), out_68, nextOutput);
				break;
			case 69:
				 sendOutput( msg.time(), out_69, nextOutput);
				break;
		default :

			cout<<"error : output port `to_peer' not set"<<endl;
			//ERROR
		}


	} else if(connecting) {
		for (set<int>::iterator it= to_peers.begin();it!=to_peers.end();it++){
			//NextOutput contains the next message to be output, to_peer the integer identifying the peer that the message is for
			switch ( *it ) {			case 0:
				 sendOutput( msg.time(), out_c0, nextOutput);
				break;
			case 1:
				 sendOutput( msg.time(), out_c1, nextOutput);
				break;
			case 2:
				 sendOutput( msg.time(), out_c2, nextOutput);
				break;
			case 3:
				 sendOutput( msg.time(), out_c3, nextOutput);
				break;
			case 4:
				 sendOutput( msg.time(), out_c4, nextOutput);
				break;
			case 5:
				 sendOutput( msg.time(), out_c5, nextOutput);
				break;
			case 6:
				 sendOutput( msg.time(), out_c6, nextOutput);
				break;
			case 7:
				 sendOutput( msg.time(), out_c7, nextOutput);
				break;
			case 8:
				 sendOutput( msg.time(), out_c8, nextOutput);
				break;
			case 9:
				 sendOutput( msg.time(), out_c9, nextOutput);
				break;
			case 10:
				 sendOutput( msg.time(), out_c10, nextOutput);
				break;
			case 11:
				 sendOutput( msg.time(), out_c11, nextOutput);
				break;
			case 12:
				 sendOutput( msg.time(), out_c12, nextOutput);
				break;
			case 13:
				 sendOutput( msg.time(), out_c13, nextOutput);
				break;
			case 14:
				 sendOutput( msg.time(), out_c14, nextOutput);
				break;
			case 15:
				 sendOutput( msg.time(), out_c15, nextOutput);
				break;
			case 16:
				 sendOutput( msg.time(), out_c16, nextOutput);
				break;
			case 17:
				 sendOutput( msg.time(), out_c17, nextOutput);
				break;
			case 18:
				 sendOutput( msg.time(), out_c18, nextOutput);
				break;
			case 19:
				 sendOutput( msg.time(), out_c19, nextOutput);
				break;
			case 20:
				 sendOutput( msg.time(), out_c20, nextOutput);
				break;
			case 21:
				 sendOutput( msg.time(), out_c21, nextOutput);
				break;
			case 22:
				 sendOutput( msg.time(), out_c22, nextOutput);
				break;
			case 23:
				 sendOutput( msg.time(), out_c23, nextOutput);
				break;
			case 24:
				 sendOutput( msg.time(), out_c24, nextOutput);
				break;
			case 25:
				 sendOutput( msg.time(), out_c25, nextOutput);
				break;
			case 26:
				 sendOutput( msg.time(), out_c26, nextOutput);
				break;
			case 27:
				 sendOutput( msg.time(), out_c27, nextOutput);
				break;
			case 28:
				 sendOutput( msg.time(), out_c28, nextOutput);
				break;
			case 29:
				 sendOutput( msg.time(), out_c29, nextOutput);
				break;
			case 30:
				 sendOutput( msg.time(), out_c30, nextOutput);
				break;
			case 31:
				 sendOutput( msg.time(), out_c31, nextOutput);
				break;
			case 32:
				 sendOutput( msg.time(), out_c32, nextOutput);
				break;
			case 33:
				 sendOutput( msg.time(), out_c33, nextOutput);
				break;
			case 34:
				 sendOutput( msg.time(), out_c34, nextOutput);
				break;
			case 35:
				 sendOutput( msg.time(), out_c35, nextOutput);
				break;
			case 36:
				 sendOutput( msg.time(), out_c36, nextOutput);
				break;
			case 37:
				 sendOutput( msg.time(), out_c37, nextOutput);
				break;
			case 38:
				 sendOutput( msg.time(), out_c38, nextOutput);
				break;
			case 39:
				 sendOutput( msg.time(), out_c39, nextOutput);
				break;
			case 40:
				 sendOutput( msg.time(), out_c40, nextOutput);
				break;
			case 41:
				 sendOutput( msg.time(), out_c41, nextOutput);
				break;
			case 42:
				 sendOutput( msg.time(), out_c42, nextOutput);
				break;
			case 43:
				 sendOutput( msg.time(), out_c43, nextOutput);
				break;
			case 44:
				 sendOutput( msg.time(), out_c44, nextOutput);
				break;
			case 45:
				 sendOutput( msg.time(), out_c45, nextOutput);
				break;
			case 46:
				 sendOutput( msg.time(), out_c46, nextOutput);
				break;
			case 47:
				 sendOutput( msg.time(), out_c47, nextOutput);
				break;
			case 48:
				 sendOutput( msg.time(), out_c48, nextOutput);
				break;
			case 49:
				 sendOutput( msg.time(), out_c49, nextOutput);
				break;
			case 50:
				 sendOutput( msg.time(), out_c50, nextOutput);
				break;
			case 51:
				 sendOutput( msg.time(), out_c51, nextOutput);
				break;
			case 52:
				 sendOutput( msg.time(), out_c52, nextOutput);
				break;
			case 53:
				 sendOutput( msg.time(), out_c53, nextOutput);
				break;
			case 54:
				 sendOutput( msg.time(), out_c54, nextOutput);
				break;
			case 55:
				 sendOutput( msg.time(), out_c55, nextOutput);
				break;
			case 56:
				 sendOutput( msg.time(), out_c56, nextOutput);
				break;
			case 57:
				 sendOutput( msg.time(), out_c57, nextOutput);
				break;
			case 58:
				 sendOutput( msg.time(), out_c58, nextOutput);
				break;
			case 59:
				 sendOutput( msg.time(), out_c59, nextOutput);
				break;
			case 60:
				 sendOutput( msg.time(), out_c60, nextOutput);
				break;
			case 61:
				 sendOutput( msg.time(), out_c61, nextOutput);
				break;
			case 62:
				 sendOutput( msg.time(), out_c62, nextOutput);
				break;
			case 63:
				 sendOutput( msg.time(), out_c63, nextOutput);
				break;
			case 64:
				 sendOutput( msg.time(), out_c64, nextOutput);
				break;
			case 65:
				 sendOutput( msg.time(), out_c65, nextOutput);
				break;
			case 66:
				 sendOutput( msg.time(), out_c66, nextOutput);
				break;
			case 67:
				 sendOutput( msg.time(), out_c67, nextOutput);
				break;
			case 68:
				 sendOutput( msg.time(), out_c68, nextOutput);
				break;
			case 69:
				 sendOutput( msg.time(), out_c69, nextOutput);
				break;
			default :

				cout<<"error : output port `to_peer' not set"<<endl;
			}//end switch
		}//end for
	}//end if
	return *this;
}

Demux::~Demux()
{
	//do nothing
}
