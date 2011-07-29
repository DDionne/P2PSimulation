[top]
components : Network IdGen@msgIdGen log@Logger Linker@LinkFinder Peer0 Peer1 Peer2 Peer3 Peer4
Link : LinkFind@Peer0 in_q@Linker
Link : LinkFind@Peer1 in_q@Linker
Link : LinkFind@Peer2 in_q@Linker
Link : LinkFind@Peer3 in_q@Linker
Link : LinkFind@Peer4 in_q@Linker
Link : q_out0@Linker Link_in@Peer0
Link : q_out1@Linker Link_in@Peer1
Link : q_out2@Linker Link_in@Peer2
Link : q_out3@Linker Link_in@Peer3
Link : q_out4@Linker Link_in@Peer4
Link : queryhit@Peer0 qhit_in@log
Link : queryhit@Peer1 qhit_in@log
Link : queryhit@Peer2 qhit_in@log
Link : queryhit@Peer3 qhit_in@log
Link : queryhit@Peer4 qhit_in@log
Link : disconnect@Peer0 in_disconnect@log
Link : disconnect@Peer1 in_disconnect@log
Link : disconnect@Peer2 in_disconnect@log
Link : disconnect@Peer3 in_disconnect@log
Link : disconnect@Peer4 in_disconnect@log
Link : connect@Peer0 in_connect@log
Link : connect@Peer1 in_connect@log
Link : connect@Peer2 in_connect@log
Link : connect@Peer3 in_connect@log
Link : connect@Peer4 in_connect@log
Link : out_0@IdGen in_query@log
Link : out_1@IdGen in_query@log
Link : out_2@IdGen in_query@log
Link : out_3@IdGen in_query@log
Link : out_4@IdGen in_query@log
Link : publish@Peer0 in_publish@log
Link : publish@Peer1 in_publish@log
Link : publish@Peer2 in_publish@log
Link : publish@Peer3 in_publish@log
Link : publish@Peer4 in_publish@log
Link : online@Peer0 in_online@Network
Link : online@Peer1 in_online@Network
Link : online@Peer2 in_online@Network
Link : online@Peer3 in_online@Network
Link : online@Peer4 in_online@Network
Link : online@Peer0 in_online@log
Link : online@Peer1 in_online@log
Link : online@Peer2 in_online@log
Link : online@Peer3 in_online@log
Link : online@Peer4 in_online@log
Link : offline@Peer0 in_offline@log
Link : offline@Peer1 in_offline@log
Link : offline@Peer2 in_offline@log
Link : offline@Peer3 in_offline@log
Link : offline@Peer4 in_offline@log
Link : offline@Peer0 in_offline@Network
Link : offline@Peer1 in_offline@Network
Link : offline@Peer2 in_offline@Network
Link : offline@Peer3 in_offline@Network
Link : offline@Peer4 in_offline@Network
Link : connect_out@Peer0 peer_connect@Network
Link : connect_out@Peer1 peer_connect@Network
Link : connect_out@Peer2 peer_connect@Network
Link : connect_out@Peer3 peer_connect@Network
Link : connect_out@Peer4 peer_connect@Network
Link : disconnect_out@Peer0 peer_disconnect@Network
Link : disconnect_out@Peer1 peer_disconnect@Network
Link : disconnect_out@Peer2 peer_disconnect@Network
Link : disconnect_out@Peer3 peer_disconnect@Network
Link : disconnect_out@Peer4 peer_disconnect@Network
Link : c_out0@Network connect_in@Peer0
Link : c_out1@Network connect_in@Peer1
Link : c_out2@Network connect_in@Peer2
Link : c_out3@Network connect_in@Peer3
Link : c_out4@Network connect_in@Peer4
Link : query_out@Peer0 msg_in@IdGen
Link : query_out@Peer1 msg_in@IdGen
Link : query_out@Peer2 msg_in@IdGen
Link : query_out@Peer3 msg_in@IdGen
Link : query_out@Peer4 msg_in@IdGen
Link : out_0@IdGen query_in@Peer0
Link : out_1@IdGen query_in@Peer1
Link : out_2@IdGen query_in@Peer2
Link : out_3@IdGen query_in@Peer3
Link : out_4@IdGen query_in@Peer4
Link : q_out@Peer0 inroute@Network
Link : q_out@Peer1 inroute@Network
Link : q_out@Peer2 inroute@Network
Link : q_out@Peer3 inroute@Network
Link : q_out@Peer4 inroute@Network
Link : route_out0@Network q_in@Peer0
Link : route_out1@Network q_in@Peer1
Link : route_out2@Network q_in@Peer2
Link : route_out3@Network q_in@Peer3
Link : route_out4@Network q_in@Peer4
Link : queryreachespeer@Peer0 queryReached@log
Link : queryreachespeer@Peer1 queryReached@log
Link : queryreachespeer@Peer2 queryReached@log
Link : queryreachespeer@Peer3 queryReached@log
Link : queryreachespeer@Peer4 queryReached@log
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

NumQueries : 15
doclist : 1,2,3
whoAmI : 0

[connector0]
friendlist : 1,2,3,4
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
beforequerymean : 8
beforequerydeviation : 0.3

offlinetimedistribution : exponential
offlinetimemean : 2000

NumQueries : 15
doclist : 3,4,5
whoAmI : 1

[connector1]
friendlist : 0,2,3
whoAmI : 1 


[Peer2]
components : session2@RandomSurfer connector2@ConnectionManager gnut2@PeerGnutella server2@Server
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
Link : online@session2 in_online@connector2
Link : online@session2 online
Link : offline@session2 in_offline@connector2
Link : out_offline@connector2 offline
Link : query@session2 query_out
Link : query@session2 publish@server2
Link : publish@session2 publish@server2
Link : p_out@server2 publish
Link : link@server2 LinkFind
Link : remove@session2 remove@server2
Link : out_connect@connector2 connect_out
Link : out_disconnect@connector2 disconnect_out
Link : connect_in in_connect@connector2
Link : connect_in connections@gnut2
Link : q_in q_in@gnut2
Link : q_out@gnut2 q_out
Link : localOut@gnut2 peer_in@server2
Link : Link_in peer_in@server2
Link : localOut@gnut2 queryreachespeer
Link : queryhit@server2 q_in@gnut2
Link : queryhit@server2 queryhit
Link : query_in query@gnut2
Link : connect_out@gnut2 connect
Link : disconnect_out@gnut2 disconnect
Link : qhit_out@gnut2 queryhit@session2

[session2]
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

NumQueries : 15
doclist : 6,7,8
whoAmI : 2

[connector2]
friendlist : 0,1,3
whoAmI : 2 


[Peer3]
components : session3@RandomSurfer connector3@ConnectionManager gnut3@PeerGnutella server3@Server
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
Link : online@session3 in_online@connector3
Link : online@session3 online
Link : offline@session3 in_offline@connector3
Link : out_offline@connector3 offline
Link : query@session3 query_out
Link : query@session3 publish@server3
Link : publish@session3 publish@server3
Link : p_out@server3 publish
Link : link@server3 LinkFind
Link : remove@session3 remove@server3
Link : out_connect@connector3 connect_out
Link : out_disconnect@connector3 disconnect_out
Link : connect_in in_connect@connector3
Link : connect_in connections@gnut3
Link : q_in q_in@gnut3
Link : q_out@gnut3 q_out
Link : localOut@gnut3 peer_in@server3
Link : Link_in peer_in@server3
Link : localOut@gnut3 queryreachespeer
Link : queryhit@server3 q_in@gnut3
Link : queryhit@server3 queryhit
Link : query_in query@gnut3
Link : connect_out@gnut3 connect
Link : disconnect_out@gnut3 disconnect
Link : qhit_out@gnut3 queryhit@session3

[session3]
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

NumQueries : 15
doclist : 9,10,11
whoAmI : 3

[connector3]
friendlist : 0,2,1
whoAmI : 3 


[Peer4]
components : session4@RandomSurfer connector4@ConnectionManager gnut4@PeerGnutella server4@Server
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
Link : online@session4 in_online@connector4
Link : online@session4 online
Link : offline@session4 in_offline@connector4
Link : out_offline@connector4 offline
Link : query@session4 query_out
Link : query@session4 publish@server4
Link : publish@session4 publish@server4
Link : p_out@server4 publish
Link : link@server4 LinkFind
Link : remove@session4 remove@server4
Link : out_connect@connector4 connect_out
Link : out_disconnect@connector4 disconnect_out
Link : connect_in in_connect@connector4
Link : connect_in connections@gnut4
Link : q_in q_in@gnut4
Link : q_out@gnut4 q_out
Link : localOut@gnut4 peer_in@server4
Link : Link_in peer_in@server4
Link : localOut@gnut4 queryreachespeer
Link : queryhit@server4 q_in@gnut4
Link : queryhit@server4 queryhit
Link : query_in query@gnut4
Link : connect_out@gnut4 connect
Link : disconnect_out@gnut4 disconnect
Link : qhit_out@gnut4 queryhit@session4

[session4]
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

NumQueries : 15
doclist : 12,13,14,15
whoAmI : 4

[connector4]
friendlist : 0
whoAmI : 4 


[Network]
 components : net@SimpleDemux LTS@LTSNetwork
out : c_out0
out : c_out1
out : c_out2
out : c_out3
out : c_out4
out : route_out0
out : route_out1
out : route_out2
out : route_out3
out : route_out4
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
Link : c_out2@LTS c_out2
Link : c_out3@LTS c_out3
Link : c_out4@LTS c_out4
Link : route_out0@net route_out0
Link : route_out1@net route_out1
Link : route_out2@net route_out2
Link : route_out3@net route_out3
Link : route_out4@net route_out4
