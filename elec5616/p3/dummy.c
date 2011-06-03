/*
 * va-vuln-poc.c, Dummy strcpy()/buffer overflow vulnerability
 * - izik@tty64.org
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char buf[256];
	strcpy(buf, argv[1]);
	return 1;
}

