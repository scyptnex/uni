EXECUTES=tut1 tut2 tut3 tut4 tut5

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

clean:
	rm -f $(EXECUTES)
	rm -f *.o
