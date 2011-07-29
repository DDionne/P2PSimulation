[top]
components : db@Database demux@Demux disp@Dispatcher gnut@Gnutella netwk@LTSNetwork   
out : out_1
out : out_2
out : out_3
out : out_4
out : out_5
in : publish remove query peer_online peer_offline peer_connect peer_disconnect 
Link : query msg_in@disp
Link : publish publish@db
Link : remove remove@db
Link : peer_online peer_online@netwk
Link : peer_offline peer_offline@netwk
Link : peer_connect peer_connect@netwk
Link : peer_disconnect peer_disconnect@netwk
Link : peer_id@disp table_in@demux
Link : query_id@disp query_in@db
Link : route_out@gnut peer_in@db
Link : queryhit@db msg_in@demux
Link : out_n@gnut inroute@netwk
Link : route_out@netwk in_n@gnut 
Link : peer_id@disp route_in@gnut
Link : out_1@demux out_1
Link : out_2@demux out_2
Link : out_3@demux out_3
Link : out_4@demux out_4
Link : out_5@demux out_5


[db]
datafile : querydoc.dat