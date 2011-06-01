#include <stdlib.h>
#include <stdio.h>

char inject[] = "\x31\xdb\x89\xd8\x40\xcd\x80";

void mung(){
	int* ret;
	ret = (int *)&ret + 2;
	(*ret) = (int)inject;
}

void main(){
	mung();
	printf("this shouldnt happen");
}
