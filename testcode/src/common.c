#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

char** get_argv(int* argc)
{
    int i = 1;
    char **argv;

    //set argc
    *((uint8_t *)CLA_STAT) = ARGC_MASK;
    //printf("CLA_ARGC : %d\n", *((uint8_t *)CLA_ARGC));
    *argc = *((uint8_t *)CLA_ARGC);

	//printf("func_argc = %d\n",*argc);

    //set argv[0]
    argv = (char **)malloc(*argc*sizeof(uint32_t));
    //printf("argv[0] len : %d\n",strlen((char *)CLA_ARGV)+1);
    argv[0] = (char *)malloc(strlen((char *)CLA_ARGV)+1);
    //printf("strcpy argv[0]\n");
    strcpy(argv[0], (uint8_t *)CLA_ARGV);
    //printf("check argv[0] : %s\n", argv[0]);

    for(i = 1 ; i<(*argc); i++)
    {
        //printf("strcpy argv[%d]\n", i);
        *((uint8_t *)CLA_STAT) = ARGV_MASK;
        argv[i] = (uint8_t *)malloc(strlen((char *)CLA_ARGV)+1);
        strcpy(argv[i], (char *)CLA_ARGV);
        //printf("check argv[%d] : %s\n", i, argv[i]);
    }

    return argv;
}

void end_main(){
	*(char*)(0xffffffff) = 1;
}
