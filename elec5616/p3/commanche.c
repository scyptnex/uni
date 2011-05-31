#include <stdio.h>
#include <string.h>

int printURL(char *myURL)
{
	int version = 2;
	char url[16];
	strcpy(url, myURL);
	printf("Commanche version %d\n", version);
	printf("Loading URL: \"%s\"\n", url);
	fflush(stdout);
	return 0;
}

int main(void)
{
	char line[2048];
	fgets(line, 2048, stdin);
	printURL(line);
	return 0;
}
