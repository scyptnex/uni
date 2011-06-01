void function(int a, int b, int c) {
   char buffer1[5];
   char buffer2[10];
	buffer1[3] = b;
	buffer2[0] = a;
	buffer2[10] = c;
}

void main() {
  function(1,2,3);
}
