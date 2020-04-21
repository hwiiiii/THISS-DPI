#include <stdio.h>

#ifdef SYISS
#include "../src/common.h"
#endif

int main(int argc, char** argv)
{
    #ifdef SYISS
    printf("SYISS\n");
    argv = get_argv(&argc);
    #endif

	int i = 0;
	printf("argc = %d\n", argc);

	printf("%d : %s\n",0, argv[0]);
	for (i = 1 ; i < argc ; i++)
	{
		printf("%d : %s\n",i, argv[i]);
	}

	argc = 100;
	printf("modified argc = %d\n", argc);
    #ifdef SYISS
    end_main();
    #endif
	return 0;
}
	
