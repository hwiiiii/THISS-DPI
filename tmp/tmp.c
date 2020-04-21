#include <stdio.h>
#include <stdint.h>


int main(){
	uint8_t a = 0xFF;
	uint8_t c = 0xFF;
	int8_t d = 0xFF;
	int8_t b;

	b = (int8_t)a;

	printf("a = %d\n", a);
	printf("b = %d\n", b);

	printf("a >> 1 = %d\n", a>>1);
	printf("b >> 1 = %d\n", b>>1);
	printf("(unsigned)b >> 1 = %d\n", (uint8_t)b>>1);
	printf("(signed)(a >> 1) = %d\n", (uint8_t)(a>>1));

	a = c >> 1;
	b = c >> 1;

	printf("a = 0xFF >> 1 : %d\n", a);
	printf("b = 0xFF >> 1 : %d\n", b);

	a = d >> 1;
	b = d >> 1;

	printf("a = (signed)0xFF >> 1 : %d\n", a);
	printf("b = (signed)0xFF >> 1 : %d\n", b);

	printf("(unsigned)255 > (unsigned)1 = %d\n", c > (uint8_t)1);
	printf("(signed)255 > (signed)1 = %d\n", d > (int8_t)1);

	printf("3 + -1 = %d\n", (uint8_t)3 + (int8_t)d);
	printf("3 + 0xFF = u %d\n", (uint8_t)((uint8_t)3 + (uint8_t)d));
	printf("3 + 0xFF = s %d\n", (int8_t)((uint8_t)3 + (uint8_t)d));
	printf("(unsigned)(3 + -1) = %d\n", (uint8_t)((uint8_t)3 + (int8_t)d));
	return 0;
}
