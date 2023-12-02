NAMES: Favour Effiong and Elton Kong
STUDENT ID: 101228775 and 101169142

COURSE: COMP2401BD
DATE: Wednesday, November 29th, 2023


LIST OF FILES:
	1. defs.h -> Includes all structure and function prototypes/definitions
	2. ghost.c -> Includes ghost data and behaviour
	3. house.c -> House activities
	4. hunter.c -> Contains hunter data and activities
	5. logger.c -> Used for logging activities/behaviour
	6. main.c -> Used for testing
	7. room.c -> Contains rooms
	8. utils.c -> Contains ghost representation
	9. Makefile -> Used for compilation and linking
	10. README.txt
	11. evidence.c -> 
	

PROGRAM'S PURPOSE:
	- A communcation simulation
	- Four hunters, each with a single device to take readings of evidence from a room
	- A ghost, wandering around and leaving evidence for hunters to find
	- When a hunter is in a room with a ghost, their fear level goes from 0 to 100
	- When their fear reaches 100, they run away



HOW TO COMPILE:
	---> Type 'make' into the terminal

	    THE LONG WAY: 
		    Type ---> gcc -Wall -Werror -o fp main.c hunter.c logger.c utils.c room.c ghost.c



HOW TO RUN:
	--> Type './fp'



Use of Generative AI: NONE

