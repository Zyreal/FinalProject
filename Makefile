TARGETS = fp ghosts.o main.o room.o logger.o utils.o hunter.o house.o evidence.o

all:	ghost.c hunter.c logger.c main.c room.c utils.c house.c evidence.c defs.h
	gcc -Wall -Wextra -c ghost.c
	gcc -Wall -Wextra -c hunter.c
	gcc -Wall -Wextra -c logger.c
	gcc -Wall -Wextra -c main.c
	gcc -Wall -Wextra -c room.c
	gcc -Wall -Wextra -c utils.c
	gcc -Wall -Wextra -c house.c
	gcc -Wall -Wextra -c evidence.c
	gcc -o fp ghost.c hunter.c logger.c main.c room.c utils.c house.c evidence.c
clean:
	rm -f ${TARGETS}