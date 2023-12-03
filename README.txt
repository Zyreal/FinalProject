NAMES: Favour Effiong and Elton Kong
STUDENT IDs: 101228775 and 101169142


COURSE: COMP2401BD
DATE: Wednesday, November 29th, 2023


LIST OF FILES:
	1. defs.h -> Includes all structure and function prototypes/definitions
	2. ghost.c -> Includes ghost data and behaviour
	3. house.c -> House activities
	4. hunter.c -> Contains hunter data and activities
	5. logger.c -> Used for logging activities/behaviour
	6. main.c -> Initializes program, and asks for user input.
	7. room.c -> Contains rooms
	8. utils.c -> Contains random number generator and functions to turn enum types to strings
	9. Makefile -> Used for compilation and cleaning
	10. README.txt
	11. evidence.c -> Contains functions manipulating evidence
	

PROGRAM'S PURPOSE:
	- A communcation simulation
	- Four hunters, each with a single device to take readings of evidence from a room
	- A ghost, wandering around and leaving evidence for hunters to find
	- When a hunter is in a room with a ghost, their fear level goes from 0 to 10
	- If a hunter is not in a room with a ghost, their boredom level goes from 0 to 100
	- If a ghost is not in a room with a hunter, their boredom level goes from 0 to 100
	- When their fear or boredom reaches its max, they run away


HOW TO COMPILE:
	---> Type 'make' into the terminal


HOW TO RUN:
	--> Type './fp'


Use of Generative AI: NONE

