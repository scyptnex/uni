#include <stdio.h>

void go(char* wr, char** arr, void* nu){
	execve(wr, arr, nu);
}

void main(){
	char* wrds[2];
	wrds[0] = "/bin/sh";
	wrds[1] = NULL;
	go(wrds[0], wrds, NULL);
}
