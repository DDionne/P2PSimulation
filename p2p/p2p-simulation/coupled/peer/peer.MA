[top]
components : session@SessionManager connector@ConnectionManager
out : out_online
out : out_offline
out : query
out : publish
out : remove
out : out_connect
out : out_disconnect 
in : queryhit 
in : in_connect
in : in_disconnect 
Link : online@session in_online@connector
Link : out_online@connector out_online
Link : offline@session in_offline@connector
Link : out_offline@connector out_offline
Link : query@session query
Link : publish@session publish
Link : remove@session remove
Link : queryhit queryhit@session
Link : out_connect@connector out_connect
Link : out_disconnect@connector out_disconnect
Link : in_connect in_connect@connector
Link : in_disconnect in_disconnect@connector

[session]
probabilityofactivity : 0.7

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

querylist : 6,2,993,5,8
doclist : 10,94,11,15
whoAmI : 31

[connector]
friendlist : 12,21,8,325,900,545
whoAmI : 31 

 
