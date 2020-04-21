#include <stdio.h>

#ifdef SYISS
#include "../src/common.h"
#endif

int c;

int main()
{
	int a;
	int b;

	a = 1;
	b = 2;
	c = 1+2;

	printf("hello world! %d\n", 3);

#ifdef SYISS
end_main();
#endif
	return 0;
}
