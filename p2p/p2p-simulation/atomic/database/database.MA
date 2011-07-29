[top]
components : db@Database 
out : queryhit
in : query_in peer_in publish remove
Link : query_in query_in@db
Link : peer_in peer_in@db
Link : publish publish@db
Link : remove remove@db
Link : queryhit@db queryhit

[db]
datafile : querydoc.dat

