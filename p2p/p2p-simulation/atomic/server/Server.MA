[top]
components : db@Server 
out : queryhit
in : query_in peer_in
Link : query_in query_in@db
Link : peer_in peer_in@db
Link : queryhit@db queryhit


[db]
datafile : serverdoc.dat

