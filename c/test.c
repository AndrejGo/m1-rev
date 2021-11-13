#include <stdlib.h>
#include <stdio.h>

int main() {

unsigned long a = 10;
unsigned long b = 6;
unsigned long result = 0;
__asm__ volatile (
	"            mov x1, %1 \n\t"
	"            mov x2, %2 \n\t"
	"            sub x3, x1, x2 \n\t"
	"            mov %0, x3 \n\t"
	: "=r"(result)
	: "r"(a),"r"(b)
	: "x1","x2","x3"
);
printf("IN  | a: %lu\n", a);
printf("IN  | b: %lu\n", b);
printf("OUT | result: %lu\n", result);


    return 0;
}