[top]
components : netwk@LTSNetwork 
out : route_out out_connect out_disconnect
in : peer_online peer_offline peer_connect peer_disconnect inroute
Link : peer_online peer_online@netwk
Link : peer_offline peer_offline@netwk
Link : peer_connect peer_connect@netwk
Link : peer_disconnect peer_disconnect@netwk
Link : inroute inroute@netwk
Link : route_out@netwk route_out
Link : out_connect@netwk out_connect
Link : out_disconnect@netwk out_disconnect

[netwk]
distribution : exponential
mean : 0.12

