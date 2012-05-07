Here are the instructions on how to run the simulation containing
Random Surfers as peers.

1- Download and place the folder P2PSimulation into 

	C:\eclipse\workspace

2- Now, go to the P2PSimulation folder and select 

	p2p->ressources->Multi_MA_scripter.py

3- Now insert the amount of peers desired, there local repositories
	capacity, the Time to Live (number of times a query will propagate)
	and the number of documents (the one already there contains 965)

4- Now, back from the P2PSimulation Folder, select

	p2p->p2p-simulation->coupled->RSurfSimu

5- Now, from this directory, right click on testing1.bat and 
	open it as a text file.


6- Next, change the line which says MultiPeerX.MA, change the X, to the 
	number of peers you decided when running the Multi_MA_scripter.

7- Now, in this .bat file, there is a segment which looks like this

		e.g. -t"02:00:00:000"

	This can be changed to decide the amount of time the simulation
	will simulate.

	do not change the -t, however, to change the time, we need
	to modify the rest

	the first section is for the number of hours, the second
	is minutes, the third is secondes and the last section
	is millisecondes. The example above would simulate 
	2 hours.

8- Now, to run the simulation, right click once again on testing1.bat and
	open it with the system editor. When the simulation is
	complete it should say 

	(testing1.bat's directory)>pause
	Press any key to continue . . .

9- To view the results of the simulation, you can open fileout.txt

10- To view the results on the simulator follow the next set of instructions
	Otherwise, you are done

11- In fileout.txt, delete all the line up to and including

	Starting simulation. Stop at time ...


	At the end of the file, delete the line saying 

	Simulation ended!

	Make sure there are no empty line at the end or start of the file
	now copy and paste this file inside the ressources folder

12- In the ressources folder, run LogEventFormatter.py
	this converts the output to the format required by the 
	visualisation tool

13- Open logEvents.txt and at the very start of the file, on the first line,
	write the amount of lines that are in the file

	For example, if my file contains

	
	12345:publish:31 (and nothing more)

	then I would make my file contain

	1    <-------- number of lines
	12345:publish:31

14- You should now be ready to use you results in the Visualisation tool
 