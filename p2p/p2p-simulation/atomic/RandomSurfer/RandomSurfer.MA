[top]
components : session@RandomSurfer 
out : online
out : offline
out : query
out : publish
out : remove
in : queryhit 
Link : online@session online
Link : offline@session offline
Link : query@session query
Link : publish@session publish
Link : remove@session remove
Link : queryhit queryhit@session

[session]
probabilityofactivity : 0.99
probabilityToIgnore : 0.5

betweenquerydistribution : lognormal
betweenquerymean : 9.63
betweenquerydeviation : 3.35

beforequerydistribution : lognormal 
beforequerymean : 3
beforequerydeviation : 6

afterquerydistribution : lognormal
afterquerymean : 9
afterquerydeviation : 5.8

offlinetimedistribution : exponential
offlinetimemean : 10

inactivesessionlengthdistribution : weibull
inactivesessionlengthshape : 0.5
inactivesessionlengthscale : 2000

NumQueries : 6
doclist : 10
whoAmI : 31
TimeToLive : 7