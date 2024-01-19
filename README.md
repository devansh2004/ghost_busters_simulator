# Purpose:

	A simulation of 4 ghost hunters looking for a ghost in a house.
    	The hunters will try to collect 3 peices of unique evidince to identify the type of ghost

# Files:

	main.c : contains the main file that runs
	README.txt : has this discription
	evidence.c : holds all the functions used to modify evidence structures and lists
    	ghost.c : holds all the functions used to modify the ghost structure
    	house.c : holds all the functtions to modify the house structure
    	hunter.c : holds all the functions to modify the hunters structures
    	logger.c :  holds all the functions that are used to print the main events that happen in the simulation
    	main.c : holds the main code for the project
    	room.c : holds all the functions to modify the room structure and lists
    	utils.c : holds all the helper functions useded through the project
    	Makefile :used to complie the code, by creating each object files then linking them
# How to use:
	complie: 
		Use the terminal to go to the folder that has all the code files from above
        then type "make" to complie the code
        then type "make clean" to clear out the object files
		

	run:
		Type "./FP to run the program

	# Instructions:
	    first input the name of the first hunter then what type of equipment they have
        repeat for the other three hunters

        the simulation will then start, and you can see all the of actions each character does

        when the simulation ends you will see the final information
