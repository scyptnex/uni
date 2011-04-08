# Sample makefile		

CC = mpicc
CFLAGS = -ansi -Wall -g
OBJECTS = collapse.o
TARGET = collapse

.PHONY: all clean run

all: $(TARGET)

run: all
	./runtests

short: all
	mpirun -np 1 $(TARGET) < 999.tst
	mpirun -np 2 $(TARGET) < 999.tst
	mpirun -np 4 $(TARGET) < 999.tst

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJECTS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o $(TARGET)

