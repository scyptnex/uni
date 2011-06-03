/*
 * got_jmpesp.c, scanning for JMP %ESP
 * - izik@tty64.org
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int i, jmps;
	char *ptr = (char *) 0xffffe000;

	jmps = 0;

	for (i = 0; i < 4095; i++) {

		if (ptr[i] == '\xff' && ptr[i+1] == '\xe4') {

			printf("* 0x%08x : jmp *%%esp\n", ptr+i);
			jmps++;
		}
	}

	if (!jmps) {

		printf("* No JMP %%ESP were found\n");
	}

	return 1;
}

