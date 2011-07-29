[top]
components : session@SessionManager 
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