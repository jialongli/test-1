#include <stdio.h>

int main() {
	unsigned int i = 0xffffffff;
	printf("i  = %x\n", i);
	printf("!i = %x\n", !i);
	printf("~i = %x\n", ~i);
	printf("!~i= %x\n", !~i);
	printf("~!i= %x\n", ~!i);
	printf("i^1 = %x\n", i^1);
}
