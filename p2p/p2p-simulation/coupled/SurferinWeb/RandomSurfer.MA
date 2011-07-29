[top]
components : session@RandomSurfer server@Server 
out : online
out : offline
out : publish
out : remove
Link : online@session online
Link : offline@session offline
Link : query@session query_in@server
Link : publish@session publish
Link : publish@session peer_in@server
Link : remove@session remove
Link : queryhit@server queryhit@session

[session]
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
doclist : 845
whoAmI : 31

[server]
datafile : serverdoc.dat