COLLAPSE= collapse2
REDBLUE= redblue
TESTS= test
TARGETS= $(COLLAPSE) $(REDBLUE)
CFLAGS= -ansi -Wall -g

.PHONY: all clean run

all: $(TARGETS)

run: all
	mpirun -np 1 $(COLLAPSE) < 999.tst
	mpirun -np 2 $(COLLAPSE) < 999.tst
	mpirun -np 3 $(COLLAPSE) < 999.tst
	mpirun -np 4 $(COLLAPSE) < 999.tst
	mpirun -np 20 $(COLLAPSE) < 999.tst
	mpirun -np 1 $(COLLAPSE) < 10001.tst
	mpirun -np 2 $(COLLAPSE) < 10001.tst
	mpirun -np 3 $(COLLAPSE) < 10001.tst
	mpirun -np 4 $(COLLAPSE) < 10001.tst
	mpirun -np 20 $(COLLAPSE) < 9999999.tst

$(COLLAPSE): collapse2.c
	mpicc $(CFLAGS) collapse2.c -o $(COLLAPSE)

$(REDBLUE): redblue.c
	gcc $(CFLAGS) -lpthread redblue.c -o $(REDBLUE)

clean:
	rm -rf *.o $(TARGETS)

