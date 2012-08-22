The RSurfSimu Model contains the following atomic models:

	-ConnectionManager -> maintains connections between peers

	-LinkFinder -> Finds and outputs the links from a specific
		document

	-msgIdGen -> gives a unique identifier to messages 
		sent between peers

	-PeerGnutella -> routs messages to the right peers

	-RandomSurfer -> the session manager which acts according
		to the RandomSurfer model

	-Server -> Local Repository (stores documents here)

	-SimpleDemux -> takes a message, and sends it to the 
		appropriate peer

	-LTSNetwork -> maintains a graph of all the peer 
		connections

	-Logger -> used to output to a file all the relevent info

Other files are:

	-register -> needed to form the coupled model using all the individual models

	-DBGraph

	-SGraph which extends SGraph (used by Linkfinder)

	-NetGraph  (used by LTSNetwork)

	-serverDoc -> list of all the links between documents
		(used by LinkFinder)

	-MultipeerX.MA (parameters for the simulation)

	-testing1.bat -> chooses which MA file to use for the
		simulation along with the length of the simulation
		and which file to output to.

	-fileout.txt -> output file (contains results of the 
		simulation)

This coupled model is used to simulate a Peer to Peer Network where
each peer follows the RandomSurfer model.

Last MODIFIED ON May 3rd 2012