[top]
components : demux@Demux 
out : out_1
out : out_2
out : out_3
out : out_4
out : out_5
in : msg_in table_in
Link : msg_in msg_in@demux
Link : table_in table_in@demux
Link : out_1@demux out_1
Link : out_2@demux out_2
Link : out_3@demux out_3
Link : out_4@demux out_4
Link : out_5@demux out_5

