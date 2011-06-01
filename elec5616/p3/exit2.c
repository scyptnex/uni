#include <stdlib.h>
#include <stdio.h>

void go(){
	__asm__("xorl %ebx,%ebx\n\t"
		"movl %ebx,%eax\n\t"
		"inc %eax\n\t"
		"int $0x80");
	printf("well this shouldnt happen\n");
}

void main(){
	printf("byebye\n");
	go();
}
