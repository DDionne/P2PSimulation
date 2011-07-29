[top]
components : session@SessionManager connector@ConnectionManager
out : online
out : offline
out : query
out : publish
out : remove
out : out_connect
out : out_disconnect 
in : queryhit 
in : in_connect
in : in_disconnect 
Link : online@session online
Link : online@session online@connector
Link : offline@session offline
Link : offline@session offline@connector
Link : query@session query
Link : publish@session publish
Link : remove@session remove
Link : queryhit queryhit@session
Link : out_connect@connector out_connect
Link : out_disconnect@connector out_disconnect
Link : in_connect in_connect@connector
Link : in_disconnect in_disconnect@connector

[session]
distribution : exponential
mean : 90
querylist : qs.dat
whoAmI : 31

[connector]
friendlist : friends.dat
whoAmI : 31 

 
