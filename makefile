########################################################
CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic
EJS = p2_e1
########################################################
OBJECTSP2E1 = p2_e1.o node.o elestack-node.o stack_elestack.o
########################################################

all : $(EJS)

clean	:
	rm -rf *.o $(EJS)

p2_e1 : $(OBJECTSP2E1)
	$(CC) $(CFLAGS) -o p2_e1 $(OBJECTSP2E1)

p2_e1.o: p2_e1.c stack_elestack.h elestack.h node.h types.h
	$(CC) $(CFLAGS) -c p2_e1.c

node.o: node.c node.h types.h
	$(CC) $(CFLAGS) -c node.c

elestack-node.o: elestack-node.c elestack.h node.h types.h
	$(CC) $(CFLAGS) -c elestack-node.c

stack_elestack.o: stack_elestack.c stack_elestack.h elestack.h types.h
	$(CC) $(CFLAGS) -c stack_elestack.c

runv:
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./p2_e1
