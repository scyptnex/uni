EXECUTES=tut1 tut2 tut3 tut4 tut5 tut6 tut8 tut9

all: $(EXECUTES)

tut1: tutorial01.c
	mpicc tutorial01.c -o tut1
	
tut2: tutorial02.c
	mpicc tutorial02.c -o tut2
	
tut3: tutorial03.c
	mpicc tutorial03.c -o tut3

tut4: tutorial04.c
	mpicc tutorial04.c -o tut4

tut5: tutorial05.c
	mpicc tutorial05.c -o tut5

tut6: tutorial06.c
	mpicc tutorial06.c -o tut6

tut8: tutorial08.c
	gcc tutorial08.c -o tut8 -lpthread

tut9: tutorial09.c
	gcc tutorial09.c -g -o tut9 -lpthread

clean:
	rm -f $(EXECUTES)
	rm -f *.o
