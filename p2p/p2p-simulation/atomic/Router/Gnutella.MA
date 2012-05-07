[top]
components : gnutella@Router 
out : q_out
out : connect_out
out : disconnect_out
in : query q_in connections
Link : query query@gnutella
Link : q_in q_in@gnutella
Link : connections connections@gnutella
Link : q_out@gnutella q_out
Link : connect_out@gnutella connect_out
Link : disconnect_out@gnutella disconnect_out

[gnutella]

connection :



