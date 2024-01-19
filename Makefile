# Default target
default: myprogram

# Compilation of source files
evidence.o: evidence.c
	gcc -c -Wextra -Wall -g -pthread evidence.c

ghost.o: ghost.c
	gcc -c -Wextra -Wall -g -pthread ghost.c

house.o: house.c
	gcc -c -Wextra -Wall -g -pthread house.c

hunter.o: hunter.c
	gcc -c -Wextra -Wall -g -pthread hunter.c

logger.o: logger.c
	gcc -c -Wextra -Wall -g -pthread logger.c

main.o: main.c
	gcc -c -Wextra -Wall -g -pthread main.c

room.o: room.c
	gcc -c -Wextra -Wall -g -pthread room.c

utils.o: utils.c
	gcc -c -Wextra -Wall -g -pthread utils.c

# Linking of object files
myprogram: evidence.o ghost.o house.o hunter.o logger.o main.o room.o utils.o
	gcc -o FP evidence.o ghost.o house.o hunter.o logger.o main.o room.o utils.o -lm

# Clean target
clean:
	rm -f *.o