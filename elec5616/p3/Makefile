CC= gcc
CFLAGS= -Wall -W -pedantic -fno-stack-protector

COM= commanche
WRAP= wrapper

.PHONY: clean all

all: $(COM) $(WRAP)

$(COM): $(COM).c
	$(CC) $(CFLAGS) $(COM).c -o $(COM)

$(WRAP): $(WRAP).c
	$(CC) $(CFLAGS) $(WRAP).c -o $(WRAP)

clean:
	rm -rf $(COM) $(WRAP)
