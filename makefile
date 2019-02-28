########################################################
CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic
EJS = p2_e1 p2_e2
########################################################
OBJECTSP2E1 = p2_e1.o node.o elestack-node.o stack_elestack.o
OBJECTSP2E2 = p2_e2.o elestack-int.o stack_elestack.o
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

p2_e2 : $(OBJECTSP2E2)
	$(CC) $(CFLAGS) -o p2_e2 $(OBJECTSP2E2)

p2_e2.o: p2_e2.c stack_elestack.h elestack.h types.h
	$(CC) $(CFLAGS) -c p2_e2.c

elestack-int.o: elestack-int.c elestack.h types.h
	$(CC) $(CFLAGS) -c elestack-int.c

stack_elestack.o: stack_elestack.c stack_elestack.h elestack.h types.h
	$(CC) $(CFLAGS) -c stack_elestack.c

runv1:
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./p2_e1
runv2:
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./p2_e2 5
