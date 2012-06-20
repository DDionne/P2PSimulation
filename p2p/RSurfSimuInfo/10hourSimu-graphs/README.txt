All of these graphs are based on a 10-hour Simulation made by the RSurfSimu Coupled model in the P2PSimulation workspace.

This model contains the following atomic models:

	-Peer
		-RandomSurfer (SessionManager)
		-ConnectionManager
		-Router
		-Repository

	-Network
		-NetworkGraph
		-PhysicalNetwork

	-Other
		-MsgIdGen
		-LinkFinder
		-Logger (only used for some of the output, not all)

The specifics of the simulation are as follows:
	
	Number of Peers:
				100 (labeled from 0 to 99)

	
	Repository Capacity:
				Each peer had a capacity of 50 documents. After they reached there limit, they would remove the last document in there list
				in order to add a new one


	Documents: 
				There are a total of 964 different documents labeled from 1 to 965


	Time To Live:			
				The Time to Live was set to 4, meaning the queries would propagate up to 4 times


	Queries Per Time Per Peer: 
				Each online peer queries once every 10 seconds


	Chance to ignore links:
				Peers have a 15% change to ignore links and query randomly


	Length of the simulation:
				10:0:0:0 (10 hours)

More details about the simulation:

	When Peers go online, they notify the NetworkGraph that they are online along with who they wish to connect to. If these connections are successful,
		
		both the peer wishing to connect and the peer being connected to get a message saying that the connection has been established. 

		Also, when this peer goes online, he publishes all of his documents for others to see (while he is online).	
	

	
	When a Peer wishes to go offline, he enters the shutdown state in which he sends messages to the NetworkGraph saying he wants to disconnect from

		all his current connections. Once ALL of these disconnections have happened (i.e. hisi connections list is empty), he may then go offline.

	
	
	The First thing a peer does after establishing all his connections is query for a random document (The Peer follows the RandomSurfer model).
		
		This can be any document between 1 and 965. If he receives any queryhits for this document, he publishes it (if he doesn't already have 
	
		a copy). Afterwards, the peer then has a choice to make, either randomly query again OR follow a link. In this case following a link also
	
		means to query. However, instead of querying for a document at random, he queries the document he just published or 
	
		viewed with a message saying something along the lines of "Im looking for any documents linked to by this one". Once he received all the 

		queryhits, he then looks at all the choices of links and picks one at random. He then repeats this process.

	

	Now, how do queries reach other peers? When a peer queries, for each connection a message gets sent to the PhysicalNetwork which sends them 
	
		to the right places. Once a peer receives this query, he takes note of the message ID and the peer he received it from (if he hasn't

		previously received this message). He then sends it to his repository to look for queryhits along with repeating the propagation by

		sending a message to the PhysicalNetwork for each of his connections. Peers only do this if the Time to Live of the message is greater than

		0. If the message has a Time to Live of 0, then it means it has travelled far enough in the network. NOTE: The Time to Live decreases by

		1 everytime it reaches a new peer



	How about Queryhits? When a peer receives a query for a specific document or a link and he has the appropriate document(s), he needs to rout back

		the queryhit to the one which queried. To do this, each peer has a routing table of message IDs corresponding to the peer they received 

		the message from. This allows the peer to rout back the message ID to the peer that sent the query. Once the queryhit reaches a peer that 

		doesn't have the message ID in his routing table, it means that the peer has been reached.
	
	


NOTES ON SPECIFIC GRAPHS:

	on the graphs containing percentile in the name:

		These graphs contain 10 plots:

			100% is the green plot (all the highest points)
			 90% is the blue plot (usually right under the 100% plot)
			 80% is the plot composed of white circles with black contours
			 70% is the magenta colored plot
			 60% is the yellow plot
			 50% is the black plot
			 40% is the cyan colored plot
			 30% is the red colored plot
			 20% is also green (should easily be able to differentiate between the 20% and the 100% since they are incredibly seperated)
			 10% is also blue (should easily be able to differentiate between the 20% and the 90% since they are incredibly seperated)



Plotting info:

	In order to get create these graphs, multiple steps were taken. Firstly, the fileout.txt file, contains the output directly from the simulator, 
	
	was converted into a LogEvents file (LogEvents.txt). The program that does this is a python script named "LogFormatterTemp.py". Afterwards, the 

	file named "LogEvents.txt" is processed by another python script named "Statistical_info.py". This script builds a table containing all the 

	queries made, the amount of peers they reached, the queryhits they received and so on. This script outputs to a file named "Stats-Table.txt" 

	which is directly used to build the graphs. 

	
	To actually create the graphs, there are currently two python scripts. Both use matplotlib to plot. The script named "StatPlotter" allows you

	to choose what you want on each axis and simply plots the information taken from the "Stats-Table". The other script, named "Smooth-StatPlotter.py"

	gives the option of building a percentile graph (which will make 10 plots on the same graph, see above) and it gives the option of building smooth

	graphs (currently takes 1000 values and plots the average).

	