#ifndef __NE_H
#define __NE_H

struct netevent { // a structure to enqueue network events : a message reaches a peer
	    	int id;   //message_id
	    	int TTL; // message TTL
	    	int peerid; // a peer receiving the message
	    	float time;	//time this message spends in the network before reaching the peer (? tentative)
	    	int toId;
	    	int query;
	    	int qhit;
	    };

static netevent makeNetEvent(int id, int peerid, float time){
	    	netevent ne;
	    	ne.id = id;
	    	ne.peerid=peerid;
	    	ne.time=time;
	    	ne.TTL=0; //TODO use TTL
	    	ne.query=0;
	    	ne.toId=0;
	    	ne.qhit=0;
	    	return ne;
};

static netevent makeNetEvent(int id, int peerid, int TTL, float time){
	    	netevent ne;
	    	ne.id = id;
	    	ne.peerid=peerid;
	    	ne.time=time;
	    	ne.TTL=TTL; //TODO use TTL
	    	ne.query=0;
	    	ne.toId=0;
	    	ne.qhit=0;
	    	return ne;
};

static netevent makeNetEvent(int id, int peerid, int TTL, float time, int query){
	    	netevent ne;
	    	ne.id = id;
	    	ne.peerid=peerid;
	    	ne.time=time;
	    	ne.TTL=TTL; //TODO use TTL
	    	ne.query=query;
	    	ne.toId=0;
	    	ne.qhit=0;
	    	return ne;
};
static netevent makeNetEvent(int id, int peerid, int TTL, float time, int query, int toId){
	    	netevent ne;
	    	ne.id = id;
	    	ne.peerid=peerid;
	    	ne.time=time;
	    	ne.TTL=TTL; //TODO use TTL
	    	ne.query=query;
	    	ne.toId=toId;
	    	ne.qhit=0;
	    	return ne;
};
static netevent makeNetEvent(int id, int peerid, int TTL, float time, int query, int toId, int qhit){
	    	netevent ne;
	    	ne.id = id;
	    	ne.peerid=peerid;
	    	ne.time=time;
	    	ne.TTL=TTL; //TODO use TTL
	    	ne.query=query;
	    	ne.toId=toId;
	    	ne.qhit=qhit;
	    	return ne;
};
#endif
