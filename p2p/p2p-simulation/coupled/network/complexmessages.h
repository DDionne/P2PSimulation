#ifndef MESSAGEUTIL
#define MESSAGEUTIL

#define TTLOFFSET 10000
#define	PEERIDOFFSET 100

//some functions to manage the messages passed between different components, where the integer must contain several values coded together.

/*this returns a message (type long) as passed between different components, from the peerid, messageid, and ttl that are given
 * */
static long buildMessage(int id, int peer, int ttl){
	return (long) ttl * TTLOFFSET + peer*PEERIDOFFSET + id;
}
static long buildMessage(int id, int peer){
	return buildMessage(id, peer,0);
}
static int getPeerId(long msg){
	//extract TTL
	int TTL = (int) floor(msg/TTLOFFSET);
	//reuse to extract peerid
	int peerid = (int) floor((msg - (TTL *TTLOFFSET))/PEERIDOFFSET);
	return peerid;
}

static int getTTL(long msg){
	//extract TTL
	int TTL = (int) floor(msg/TTLOFFSET);
	return TTL;
}

static int getMessageId(long msg){
	//extract TTL
	int TTL = (int) floor(msg/TTLOFFSET);
	//reuse to extract peerid
	int peerid = (int) floor((msg - (TTL *TTLOFFSET))/PEERIDOFFSET);
	int msgid = msg - (peerid *PEERIDOFFSET + (TTL*TTLOFFSET));
	return msgid;

}
#endif
