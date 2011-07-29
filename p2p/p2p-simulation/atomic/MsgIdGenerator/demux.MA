[top]
components : demux@Demux 
out : out_1
out : out_2
out : out_3
out : out_4
out : out_5
out : out_6
out : out_7
out : out_8
out : out_9
out : out_10
in : msg_in table_in connect_in
Link : msg_in msg_in@demux
Link : connect_in connect_in@demux
Link : table_in table_in@demux
Link : out_1@demux out_1
Link : out_2@demux out_2
Link : out_3@demux out_3
Link : out_4@demux out_4
Link : out_5@demux out_5
Link : out_6@demux out_6
Link : out_7@demux out_7
Link : out_8@demux out_8
Link : out_9@demux out_9
Link : out_10@demux out_10

