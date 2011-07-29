[top]
components : Network IdGen@msgIdGen log@Logger Linker@LinkFinder Peer0 Peer1
Link : LinkFind@Peer0 in_q@Linker
Link : LinkFind@Peer1 in_q@Linker
Link : q_out0@Linker Link_in@Peer0
Link : q_out1@Linker Link_in@Peer1
Link : queryhit@Peer0 qhit_in@log
Link : queryhit@Peer1 qhit_in@log
Link : disconnect@Peer0 in_disconnect@log
Link : disconnect@Peer1 in_disconnect@log
Link : connect@Peer0 in_connect@log
Link : connect@Peer1 in_connect@log
Link : out_0@IdGen in_query@log
Link : out_1@IdGen in_query@log
Link : publish@Peer0 in_publish@log
Link : publish@Peer1 in_publish@log
Link : online@Peer0 in_online@Network
Link : online@Peer1 in_online@Network
Link : online@Peer0 in_online@log
Link : online@Peer1 in_online@log
Link : offline@Peer0 in_offline@log
Link : offline@Peer1 in_offline@log
Link : offline@Peer0 in_offline@Network
Link : offline@Peer1 in_offline@Network
Link : connect_out@Peer0 peer_connect@Network
Link : connect_out@Peer1 peer_connect@Network
Link : disconnect_out@Peer0 peer_disconnect@Network
Link : disconnect_out@Peer1 peer_disconnect@Network
Link : c_out0@Network connect_in@Peer0
Link : c_out1@Network connect_in@Peer1
Link : query_out@Peer0 msg_in@IdGen
Link : query_out@Peer1 msg_in@IdGen
Link : out_0@IdGen query_in@Peer0
Link : out_1@IdGen query_in@Peer1
Link : q_out@Peer0 inroute@Network
Link : q_out@Peer1 inroute@Network
Link : route_out0@Network q_in@Peer0
Link : route_out1@Network q_in@Peer1
Link : queryreachespeer@Peer0 queryReached@log
Link : queryreachespeer@Peer1 queryReached@log
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
beforequerymean : 3
beforequerydeviation : 6

offlinetimedistribution : exponential
offlinetimemean : 2000

NumQueries : 965
doclist : 1,2,3,3,5,6,7,8,8,9,9,11,12,15
whoAmI : 0

[connector0]
friendlist : 1
whoAmI : 0 


[Peer1]
components : session1@RandomSurfer connector1@ConnectionManager gnut1@PeerGnutella server1@Server
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
Link : online@session1 in_online@connector1
Link : online@session1 online
Link : offline@session1 in_offline@connector1
Link : out_offline@connector1 offline
Link : query@session1 query_out
Link : query@session1 publish@server1
Link : publish@session1 publish@server1
Link : p_out@server1 publish
Link : link@server1 LinkFind
Link : remove@session1 remove@server1
Link : out_connect@connector1 connect_out
Link : out_disconnect@connector1 disconnect_out
Link : connect_in in_connect@connector1
Link : connect_in connections@gnut1
Link : q_in q_in@gnut1
Link : q_out@gnut1 q_out
Link : localOut@gnut1 peer_in@server1
Link : Link_in peer_in@server1
Link : localOut@gnut1 queryreachespeer
Link : queryhit@server1 q_in@gnut1
Link : queryhit@server1 queryhit
Link : query_in query@gnut1
Link : connect_out@gnut1 connect
Link : disconnect_out@gnut1 disconnect
Link : qhit_out@gnut1 queryhit@session1

[session1]
probabilityofactivity : 0.7
probabilityToIgnore : 0.15

betweenquerydistribution : lognormal
betweenquerymean : 2.63
betweenquerydeviation : 3.35

beforequerydistribution : lognormal 
beforequerymean : 3
beforequerydeviation : 6

offlinetimedistribution : exponential
offlinetimemean : 2000

NumQueries : 965
doclist : 1,2,4,4,7,9,10,11,12,13,14,15
whoAmI : 1

[connector1]
friendlist : 0
whoAmI : 1 


[Network]
 components : net@SimpleDemux LTS@LTSNetwork
out : c_out0
out : c_out1
out : route_out0
out : route_out1
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
Link : c_out1@LTS c_out1
Link : route_out0@net route_out0
Link : route_out1@net route_out1
