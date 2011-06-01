#include <stdio.h>

void function(int a, int b, int c) {
   char buffer1[5];
   char buffer2[10];
   int *ret;

buffer1[0] = '\0';
buffer1[1] = '\0';
buffer1[2] = '\0';
buffer1[3] = '\0';
   buffer1[4] = 'a';

   ret = (int*)(buffer1 + 13);

	printf("%x\n", *ret);
	printf("%x\n", ret);
   (*ret) += 7;
	printf("%x\n", ret);
}

void main() {
  int x;

  x = 0;
  function(1,2,3);
  x = 1;
  printf("%d\n",x);
  printf("lol\n");
}
