#ifndef __NE_H
#define __NE_H

struct networkevent { // a structure to enqueue network events : a message reaches a peer
	    	int id;   //message_id
	    	int TTL; // message TTL
	    	int peerid; // a peer receiving the message
	    	float time;	//time this message spends in the network before reaching the peer (? tentative)
	    };

static networkevent makeNetworkEvent(int id, int peerid, float time){
	    	networkevent ne;
	    	ne.id = id;
	    	ne.peerid=peerid;
	    	ne.time=time;
	    	ne.TTL=0; //TODO use TTL
	    	return ne;
};

static networkevent makeNetworkEvent(int id, int peerid, int TTL, float time){
	    	networkevent ne;
	    	ne.id = id;
	    	ne.peerid=peerid;
	    	ne.time=time;
	    	ne.TTL=TTL; //TODO use TTL
	    	return ne;
};
#endif
