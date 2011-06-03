#include<stdio.h>
#include<string.h>

int
printURL(char *myURL) {
	int version = 2;
	char url[256];
	
	strcpy(url, myURL);
	printf("Loading URL: %s\n", url);
	
	return 0;
}

void
main(int argc, char **argv) {
	printURL(argv[1]);
}
