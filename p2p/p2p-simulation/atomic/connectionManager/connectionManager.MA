[top]
components : connector@ConnectionManager 
in : in_online
in : in_offline
in : in_connect
in : in_disconnect 
out : out_connect
out : out_disconnect
out : out_online
out : out_offline 
Link : out_connect@connector out_connect
Link : out_disconnect@connector out_disconnect
Link : out_online@connector out_online
Link : out_offline@connector out_offline
Link : in_online in_online@connector 
Link : in_offline in_offline@connector
Link : in_connect in_connect@connector
Link : in_disconnect in_disconnect@connector


[connector]
friendlist : 12,21,8,325,900,545
whoAmI : 31 

 
