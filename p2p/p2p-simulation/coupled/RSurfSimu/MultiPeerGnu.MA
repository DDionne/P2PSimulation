[top]
components : session1@RandomSurfer gnut1@PeerGnutella IdGen@msgIdGen session2@RandomSurfer gnut2@PeerGnutella net@SimpleDemux session3@RandomSurfer gnut3@PeerGnutella LTS@LTSNetwork CM1@ConnectionManager CM2@ConnectionManager CM3@ConnectionManager session777@RandomSurfer gnut777@PeerGnutella CM777@ConnectionManager 
out : publish
out : remove
out : queryhit
out : offline_out
out : online_out
out : connection
out : disconnection
in : online
in : offline
Link : disconnect_out@gnut1 disconnection
Link : disconnect_out@gnut2 disconnection
Link : disconnect_out@gnut3 disconnection
Link : disconnect_out@gnut777 disconnection
Link : connect_out@gnut1 connection
Link : connect_out@gnut2 connection
Link : connect_out@gnut3 connection
Link : connect_out@gnut777 connection
Link : out_online@CM1 online_out
Link : out_online@CM2 online_out
Link : out_online@CM3 online_out
Link : out_online@CM777 online_out
Link : out_offline@CM1 offline_out
Link : out_offline@CM2 offline_out
Link : out_offline@CM3 offline_out
Link : out_offline@CM777 offline_out
Link : online in_online@CM1
Link : online in_online@CM2
Link : online in_online@CM3
Link : online in_online@CM777
Link : offline in_offline@CM1
Link : offline in_offline@CM2
Link : offline in_offline@CM3
Link : offline in_offline@CM777
Link : out_connect@CM1 connect_in@LTS
Link : out_connect@CM2 connect_in@LTS
Link : out_connect@CM3 connect_in@LTS
Link : out_connect@CM777 connect_in@LTS
Link : out_disconnect@CM1 disconnect_in@LTS
Link : out_disconnect@CM2 disconnect_in@LTS
Link : out_disconnect@CM3 disconnect_in@LTS
Link : out_disconnect@CM777 disconnect_in@LTS
Link : c_out1@LTS in_connect@CM1
Link : c_out2@LTS in_connect@CM2
Link : c_out3@LTS in_connect@CM3
Link : c_out777@LTS in_connect@CM777
Link : c_out1@LTS connections@gnut1
Link : c_out2@LTS connections@gnut2
Link : c_out3@LTS connections@gnut3
Link : c_out777@LTS connections@gnut777
Link : q_out@gnut1 inroute@net
Link : q_out@gnut2 inroute@net
Link : q_out@gnut3 inroute@net
Link : q_out@gnut777 inroute@net
Link : online@session1 in_online@CM1
Link : online@session2 in_online@CM2
Link : online@session3 in_online@CM3
Link : online@session777 in_online@CM777
Link : offline@session1 in_offline@CM1
Link : offline@session2 in_offline@CM2
Link : offline@session3 in_offline@CM3
Link : offline@session777 in_offline@CM777
Link : out_online@CM1 peer_online@LTS
Link : out_online@CM2 peer_online@LTS
Link : out_online@CM3 peer_online@LTS
Link : out_online@CM777 peer_online@LTS
Link : out_offline@CM1 peer_offline@LTS
Link : out_offline@CM2 peer_offline@LTS
Link : out_offline@CM3 peer_offline@LTS
Link : out_offline@CM777 peer_offline@LTS
Link : query@session1 msg_in@IdGen
Link : query@session2 msg_in@IdGen
Link : query@session3 msg_in@IdGen
Link : query@session777 msg_in@IdGen
Link : publish@session1 publish
Link : publish@session2 publish
Link : publish@session3 publish
Link : publish@session777 publish
Link : publish@session1 msg_in@IdGen
Link : publish@session2 msg_in@IdGen
Link : publish@session3 msg_in@IdGen
Link : publish@session777 msg_in@IdGen
Link : remove@session1 remove
Link : remove@session2 remove
Link : remove@session3 remove
Link : remove@session777 remove
Link : queryhit@session1 queryhit 
Link : queryhit@session2 queryhit
Link : queryhit@session3 queryhit
Link : queryhit@session777 queryhit
Link : out_1@IdGen query@gnut1
Link : out_2@IdGen query@gnut2
Link : out_3@IdGen query@gnut3
Link : out_777@IdGen query@gnut777
Link : route_out1@net q_in@gnut1
Link : route_out2@net q_in@gnut2
Link : route_out3@net q_in@gnut3
Link : route_out777@net q_in@gnut777

[session1]
probabilityofactivity : 0.7
probabilityToIgnore : 0.15

betweenquerydistribution : lognormal
betweenquerymean : 2.63
betweenquerydeviation : 3.35

beforequerydistribution : lognormal 
beforequerymean : 3
beforequerydeviation : 6

afterquerydistribution : lognormal
afterquerymean : 3
afterquerydeviation : 5.8

offlinetimedistribution : exponential
offlinetimemean : 2000

inactivesessionlengthdistribution : weibull
inactivesessionlengthshape : 0.5
inactivesessionlengthscale : 2000

NumQueries : 965
whoAmI : 1

[session2]
probabilityofactivity : 0.7
probabilityToIgnore : 0.15

betweenquerydistribution : lognormal
betweenquerymean : 2.63
betweenquerydeviation : 3.35

beforequerydistribution : lognormal 
beforequerymean : 3
beforequerydeviation : 6

afterquerydistribution : lognormal
afterquerymean : 3
afterquerydeviation : 5.8

offlinetimedistribution : exponential
offlinetimemean : 2000

inactivesessionlengthdistribution : weibull
inactivesessionlengthshape : 0.5
inactivesessionlengthscale : 2000

NumQueries : 965
whoAmI : 2

[session3]
probabilityofactivity : 0.7
probabilityToIgnore : 0.15

betweenquerydistribution : lognormal
betweenquerymean : 2.63
betweenquerydeviation : 3.35

beforequerydistribution : lognormal 
beforequerymean : 3
beforequerydeviation : 6

afterquerydistribution : lognormal
afterquerymean : 3
afterquerydeviation : 5.8

offlinetimedistribution : exponential
offlinetimemean : 2000

inactivesessionlengthdistribution : weibull
inactivesessionlengthshape : 0.5
inactivesessionlengthscale : 2000

NumQueries : 965
whoAmI : 3

[session777]
probabilityofactivity : 0.7
probabilityToIgnore : 0.15

betweenquerydistribution : lognormal
betweenquerymean : 2.63
betweenquerydeviation : 3.35

beforequerydistribution : lognormal 
beforequerymean : 3
beforequerydeviation : 6

afterquerydistribution : lognormal
afterquerymean : 3
afterquerydeviation : 5.8

offlinetimedistribution : exponential
offlinetimemean : 2000

inactivesessionlengthdistribution : weibull
inactivesessionlengthshape : 0.5
inactivesessionlengthscale : 2000

NumQueries : 965
whoAmI : 777

[CM1]

friendlist : 2,3
whoAmI : 1

[CM2]

friendlist : 1,3
whoAmI : 2

[CM3]

friendlist : 1,2
whoAmI : 3

[CM777]

friendlist : 1
whoAmI : 777

