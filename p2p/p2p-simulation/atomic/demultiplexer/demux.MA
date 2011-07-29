[top]
components : demux@Demux 
out : out_1
out : out_2
out : out_3
out : out_4
out : out_5
out : out_c1
out : out_c2
out : out_c3
out : out_c4
out : out_c5
in : msg_in table_in connect_in
Link : msg_in msg_in@demux
Link : connect_in connect_in@demux
Link : table_in table_in@demux
Link : out_1@demux out_1
Link : out_2@demux out_2
Link : out_3@demux out_3
Link : out_4@demux out_4
Link : out_5@demux out_5
Link : out_c1@demux out_c1
Link : out_c2@demux out_c2
Link : out_c3@demux out_c3
Link : out_c4@demux out_c4
Link : out_c5@demux out_c5

