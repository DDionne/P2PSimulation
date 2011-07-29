#ifndef __NE_H
#define __NE_H
//TODO : define different event types, then field1, field 2, field3.

struct networkevent { // a structure to enqueue network events : a message reaches a peer
			bool connectEvent; // a boolean indicating whether this event is a "connection" event
	    	int id;   //message_id
	    	int TTL; // message TTL
	    	int peerid; // a peer receiving the message
	    	float time;	//time this message spends in the network before reaching the peer (? tentative)
	    };

static networkevent makeNetworkEvent(int id, int peerid, float time){
	    	networkevent ne;
	    	ne.connectEvent=false;
	    	ne.id = id;
	    	ne.peerid=peerid;
	    	ne.time=time;
	    	ne.TTL=0; //TODO use TTL
	    	return ne;
};

static networkevent makeNetworkEvent(int id, int peerid, int TTL, float time){
	    	networkevent ne;
	    	ne.connectEvent=false;
	    	ne.id = id;
	    	ne.peerid=peerid;
	    	ne.time=time;
	    	ne.TTL=TTL; //TODO use TTL
	    	return ne;
};

//make a "connect" event (notifies of a connection / disconnection to the relevant peers)
static networkevent makeConnectEvent(int peer1, int peer2, float time){
	networkevent ne;
	ne.connectEvent=true;
	ne.TTL = 1;
	ne.peerid=peer2;
	ne.time=time;
	ne.id = peer1;
	return ne;
};

//make a "connect" event (notifies of a connection / disconnection to the relevant peers)
static networkevent makeDisConnectEvent(int peer1, int peer2, float time){
	networkevent ne;
	ne.connectEvent=true;
	ne.TTL = 0;
	ne.peerid=peer2;
	ne.time=time;
	ne.id = peer1;
	return ne;
};

//comparator for network events : compares the time, gives the reverse order of the time value
class NECompare {
	typedef networkevent value_type;
public :
  bool operator()( const networkevent& m1,
    const networkevent& m2 ) const
    {
	  if( m1.time > m2.time ) //higher priority for the earlier output time
		  return true;
	  else
		  return false;

    }
};


#endif
