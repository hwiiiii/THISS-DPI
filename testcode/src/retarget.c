#include <stdio.h>
#include <stdint.h>
#include <string.h>
//#include <rt_misc.h>

struct __FILE { int dummy; };

FILE __stdout;
FILE __stdin;

int my_fputc(char ch) {
	*((char *)(0xF0000000)) = (char )ch;
	*((char *)(0xFFFFFFFF)) = 2;
	return ch;
}
//	
//int fgetc(FILE *f) {
//	*((char *)(0xFFFFFFFF)) = 4;
//	return *((uint8_t *)(0xF0000000));
//}


__attribute__ ((used)) int _write (int fd, char *ptr, int len)
{
	size_t i;
	for(i = 0; i<len; i++)
	{
		my_fputc(ptr[i]);
	}
	return len;
}
	
	
