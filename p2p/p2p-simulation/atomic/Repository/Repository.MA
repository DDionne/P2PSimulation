[top]
components : db@Repository
out : queryhit p_out link
in : peer_in publish remove
Link : peer_in peer_in@db
Link : publish publish@db
Link : remove remove@db
Link : queryhit@db queryhit
Link : p_out@db p_out
Link : link@db link

[db]

capacity : 5



