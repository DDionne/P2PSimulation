[top]
components : Network IdGen@msgIdGen log@Logger Linker@LinkFinder Peer0
Link : LinkFind@Peer0 in_q@Linker
Link : q_out0@Linker Link_in@Peer0
Link : queryhit@Peer0 qhit_in@log
Link : disconnect@Peer0 in_disconnect@log
Link : connect@Peer0 in_connect@log
Link : out_0@IdGen in_query@log
Link : publish@Peer0 in_publish@log
Link : online@Peer0 in_online@Network
Link : online@Peer0 in_online@log
Link : offline@Peer0 in_offline@log
Link : offline@Peer0 in_offline@Network
Link : connect_out@Peer0 peer_connect@Network
Link : disconnect_out@Peer0 peer_disconnect@Network
Link : c_out0@Network connect_in@Peer0
Link : query_out@Peer0 msg_in@IdGen
Link : out_0@IdGen query_in@Peer0
Link : q_out@Peer0 inroute@Network
Link : route_out0@Network q_in@Peer0
Link : queryreachespeer@Peer0 queryReached@log
[Linker]
datafile : serverdoc.dat

[Peer0]
components : session0@RandomSurfer connector0@ConnectionManager gnut0@PeerGnutella server0@Server
out : query_out
out : q_out
out : connect_out
out : disconnect_out
out : online
out : offline
out : disconnect
out : connect
out : queryhit
out : publish
out : queryreachespeer
out : LinkFind
in : query_in 
in : q_in
in : connect_in
in : Link_in
Link : online@session0 in_online@connector0
Link : online@session0 online
Link : offline@session0 in_offline@connector0
Link : out_offline@connector0 offline
Link : query@session0 query_out
Link : query@session0 publish@server0
Link : publish@session0 publish@server0
Link : p_out@server0 publish
Link : link@server0 LinkFind
Link : remove@session0 remove@server0
Link : out_connect@connector0 connect_out
Link : out_disconnect@connector0 disconnect_out
Link : connect_in in_connect@connector0
Link : connect_in connections@gnut0
Link : q_in q_in@gnut0
Link : q_out@gnut0 q_out
Link : localOut@gnut0 peer_in@server0
Link : Link_in peer_in@server0
Link : localOut@gnut0 queryreachespeer
Link : queryhit@server0 q_in@gnut0
Link : queryhit@server0 queryhit
Link : query_in query@gnut0
Link : connect_out@gnut0 connect
Link : disconnect_out@gnut0 disconnect
Link : qhit_out@gnut0 queryhit@session0

[session0]
probabilityofactivity : 0.7
probabilityToIgnore : 0.15

betweenquerydistribution : lognormal
betweenquerymean : 2.63
betweenquerydeviation : 3.35

beforequerydistribution : lognormal 
beforequerymean : 8
beforequerydeviation : 0.3

offlinetimedistribution : exponential
offlinetimemean : 2000

NumQueries : 965
doclist : 1,1,1,2,3,3,3,4,4,4,5,5,6,7,8,8,9,10,10,10,11,11,12,13,13,14,14,15,15,15
whoAmI : 0

[connector0]
friendlist : -1
whoAmI : 0 


[Network]
 components : net@SimpleDemux LTS@LTSNetwork
out : c_out0
out : route_out0
in : peer_connect
in : peer_disconnect
in : inroute
in : in_online
in : in_offline
Link : peer_connect connect_in@LTS
Link : peer_disconnect disconnect_in@LTS
Link : in_online peer_online@LTS
Link : in_offline peer_offline@LTS
Link : inroute inroute@net
Link : c_out0@LTS c_out0
Link : route_out0@net route_out0
