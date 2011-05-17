COLLAPSE= collapse2
REDBLUE= redblue
TESTS= test
TARGETS= $(COLLAPSE) $(REDBLUE) $(TESTS)
CFLAGS= -ansi -Wall -g

.PHONY: all clean run

all: $(TARGETS)

$(COLLAPSE): twoa.c
	mpicc $(CFLAGS) twoa.c -o $(COLLAPSE)

$(REDBLUE):
	#do nothing?

test: test.c
	gcc $(CFLAGS) test.c -o test

clean:
	rm -rf *.o $(TARGETS)

