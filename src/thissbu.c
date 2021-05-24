// --=======================================================================================--
// Copyright (c) 2020 DSAL, EWHA Womans University. All rights reserved
//                     DSAL Confidential Proprietary
//  ------------------------------------------------------------------------------------------
//        This confidential and proprietary software may be used only as
//            authorised by a licensing agreement from DSAL.
//
//         The entire notice above must be reproduced on all authorised
//          copies and copies may only be made to the extent permitted
//                by a licensing agreement from DSAL.
//
//      The entire notice above must be reproduced on all authorized copies.
//
// -------------------------------------------------------------------------------------------
// FILE NAME       : thiss.c
// DEPARTMENT      : Digital System Architecture Lab.
// AUTHOR          : Kim Sung Yeon
// AUTHOR'S EMAIL  : jkjk0411@gmail.com
// -------------------------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION DATE         AUTHOR         DESCRIPTION
// 1.0     2020-03-24   Kim Sung Yeon  ISS main source file
// 2.0     2021-05-10   Kim Whi Jin    ISS main source file for RV64IM  
// CONTACT EMAIL   : whijin98@gmail.com
// -------------------------------------------------------------------------------------------
// PURPOSE      : RISC-V ISS
//--------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libelf.h>
#include </home1/wjkim/dsal/CPU_CORE_DES/alu_final/sim/svdpi.h>
 
#include "proc.h"
#include "handler.h"
#include "decoder.h"
#include "instruction.h"

#define STATUS

#define DOT_TERM	1000000 //1,000,000
//if it's defined, it cannot be changed after compile
//make it variable
//below are default?

//#define IMEM_START  0x10000000  
//#define IMEM_SIZE   0x100000      //2**(4+16) = 2**20 = 1MB
//#define DMEM_START  0x20000000
//#define DMEM_SIZE   0x200000      //2**(5+16) = 2**21 = 2MB

	//byte MEMORY[2**32]; //4BG Memory

//0xffff0000 for command-line argument
//0xffffffff for target end & target printf
#define	TARGET_STAT	(proc->MEMORY+0xFFFFFFFF)
#define PRINTF_MASK	0x2
#define END_MASK	0x1

//Command-Line Argument (CLA)
#define	CLA_STAT	(proc->MEMORY+0xFFFF0000)
#define ARGC_MASK	0x1
#define	ARGV_MASK	0x2
#define CLA_ARGC	(proc->MEMORY+0xFFFF0004)
#define CLA_ARGV	(proc->MEMORY+0xFFFF00F0)

//extern int PA_FLAG;
//extern PA_table *table_head;

int PA = 0;

int main(int argc, char** argv)
{
	int i;
	int target_argc_start = 0;
    word dump_addr = 0;
    word dump_size = 0;
    proc_t* proc;

	longl pc_start = 0;
	word  pc_log = 0;
	word  gpr_log = 0;


    proc = (proc_t*)malloc(sizeof(proc_t));

    word inst;
	word BP = -1;
    word status = 0;
	
	word break_state = 0;
	word key;
	unsigned long long cnt = 0;

    table_head = INIT_TABLE();


    if(argc==1)
    {
        printf("no args\n");
        printf("\n\n");
        printf("------------------options------------------\n");
        printf("'-dump_addr=[hex]'\t: Set memory dump start address\n");
        printf("'-dump_size=[KB]'\t: Set memory dump size\n");
        printf("'-start_addr=[hex]'\t: Set start PC address\n");
        printf("'-bp_addr=[hex]'\t: Set break point address\n");
        printf("'-pc_log'\t\t\t: Print PC trace\n");
        printf("'-R'\t\t\t: Print GPR values at the end of simulation\n");
        printf("'-S'\t\t\t: Print ISS Status\t\t\t<-Not Available\n");
        printf("'-PA'\t\t\t: Pointer Authentication Mode\t\t<-Not Available\n");
        printf("'-A [arg1, arg2, ...]'\t: Set command line argument of target program\n");
        printf("\n");
        printf("Caution : '-A' option must be last option\n");
        printf("\n");
        printf("Example : ./bin/syiss ./testcode/app/program.bin -dump_addr=49914 -dump_size=734e -A 1 2 3 4aaaaaa\n");
        printf("\n-------------------------------------------\n");
        exit(1);
    }

    for(i=2;i<argc;i++)
	{
        //dump
		if(!strncmp("-dump_addr=",argv[i],11))
			dump_addr = (word)strtol(argv[i]+11, NULL, 16);		//[0x]
		else if(!strncmp("-dump_size=",argv[i],11))
			dump_size = (word)strtol(argv[i]+11, NULL, 16);		//[0x]
			//dm_size = 1024*atoi(argv[i]+11);					//[KB]

		//start addr
		else if(!strncmp("-start_addr=",argv[i],12))
			pc_start = (longl)strtol(argv[i]+12, NULL, 16);		//[0x]          //word -> longl modified

		//Break Point
		else if(!strncmp("-bp_addr=",argv[i],9))
			BP = (longl)strtol(argv[i]+9, NULL, 16);		    //[0x]          //word -> longl modified

		//PC log
		else if(!strncmp("-pc_log",argv[i],7))
			pc_log = 1;

        //Status
		else if(!strncmp("-S",argv[i],2))
			pc_log = 1;

        //Status
		else if(!strncmp("-R",argv[i],2))
			gpr_log = 1;

		//PA
		else if(!strncmp("-PA",argv[i],3))
			PA = 1;

		else if (!strncmp("-A",argv[i],2))
		{
			target_argc_start = i+1;
			break;
		}
	}

	if(target_argc_start == 0)
	{
		target_argc_start = argc;
	}


	init_proc(	proc,
		//		argv[1],
                "../testcode/app/test0504.out",
				pc_start
				);


	

	////////////////////////////////START ISS///////////////////////////
	#ifdef STATUS
	printf("-----------------------------------\n\nProgram Start\n\n-----------------------------------\n");
	#endif
    while(1)
    {
		if(pc_log)
			printf("PC : %x\n", proc->PC);

        inst = (word)inst_fetch(proc);
        //if(PA_FLAG)
        //{
        //    printf("PA not executed\n");
        //    printf("PA inst : %x\n", inst);
        //    printf("PA addr : %x\n", _PC);
        //}
        inst_exe(proc, inst);

		#ifdef STATUS
		cnt++;
		if((cnt%((unsigned long long)DOT_TERM*100))==0)
			printf(".\n");
		if((cnt%((unsigned long long)DOT_TERM*1000))==0)
			printf("\n%x\n",proc->PC);
		#endif
			
        if((BP != -1)&&(_PC == BP))
        {
			break_state = 1;
		}

		if(break_state)
		{
			BP = -1;
			printf("-----------------------------------\nBreak Point\n-----------------------------------\n");
			printf("PC\t\t\t= %x\n", _PC);
			printf("Instruction [hex]\t= 0x%x\n\n", inst);
            for(i=0 ; i<GPR_SIZE;i++)
            {
                printf("GPR[%d]\t= %16lx\n", i, proc->REG_GPR[i]);
            }
			printf("-----------------------------------\nWrite Command : \n");
			printf("0 : end program\n");
			printf("1 : run (remove break point)\n");
			printf("2 : set new break point\n");
			printf("any other numbers : next pc\n");

			scanf("%d", &key);

			if(key == 1)
			{
				break_state = 0;
				printf("-----------------------------------\nBreak Point Removed\n-----------------------------------\n");
			}
			if(key == 0)
				break;
			if(key == 2)
			{
				printf("Write new bread point address [0x] (lower case) : \n");
				printf("0x");
				scanf("%x", &key);
				break_state = 0;
				BP = key;
			}

        }

		if(!_IS_B)
			_PC += 4;

		_IS_B = 0;

		//////////////////////////////COMMAND-LINE//////////////////////////

        // Why is it in the while loop :
        //  To make it simple and easy to design
        //  The Number of command line argument is known
        //  And, I just wanted to allocate as small memory(proc->memory) as possible for 'CLA_ARGV'
        //  Further Optimization is needed though.

		if((*((byte *)CLA_STAT) & ARGC_MASK) !=0)
		{
		//if argc > 8-bit number, modification required
			//printf("set target argc : %d\n", argc-target_argc_start+1);
			*((byte *)CLA_ARGC) = (byte)(argc - target_argc_start + 1);
			//printf("check argc : %d\n", *((byte *)CLA_ARGC));
			//printf("set target argv[0] : %s\n", argv[1]);
			strcpy((char *)CLA_ARGV, argv[1]);
			//printf("done\n");
			*((byte *)CLA_STAT) = 0;
		}

		if((*((byte *)CLA_STAT) & ARGV_MASK) != 0)
		{
			if(target_argc_start > argc)
			{
				printf("target_argc_start > argc");
				exit(1);
			}

			//printf("start argv\n");
			strcpy((char *)CLA_ARGV, argv[target_argc_start]);
			target_argc_start++;
			//printf("done\n");
			*((byte *)CLA_STAT) = 0;
		}
		/////////////////////////////////////////////////////////////////////

		//////////////////////////FINISH/////////////////////////////////
		if((*((byte *)TARGET_STAT) & END_MASK) != 0)
		{
			#ifdef STATUS
			printf("-----------------------------------\nmain finished\n-----------------------------------\n");
			#endif
			*((byte *)TARGET_STAT) = 0;
			break;
		}
		///////////////////////////////PRINTF////////////////////////////////////
		else if((*((byte *)TARGET_STAT) & PRINTF_MASK) !=0)
		{
			printf("%c",_MEM(0xf0000000));
			*((byte *)TARGET_STAT) = 0;
		}
		else if((*((byte *)TARGET_STAT)) & 0x4 !=0)
		{
			printf("not implemented\n");
			*((byte *)TARGET_STAT) = 0;
		}

    }

	////////////////////////////////////ISS Finished////////////////////////////////

    if(dump_addr != 0)
    {
		FILE *fp = fopen("mem_dump", "w");
		fwrite((proc->MEMORY + dump_addr), sizeof(byte), dump_size, fp); 
		fclose(fp);
	}

    if(gpr_log)
    {
        printf("-----------------------------------\nGPR\n-----------------------------------\n");
        for(i=0 ; i<GPR_SIZE;i++)
        {
            printf("GPR[%d]=%16lx\n", i, proc->REG_GPR[i]);
        }
        printf("Cycle # : %lld\n",cnt);
        printf("-----------------------------------\nISS end\n-----------------------------------\n");
    }

	//endwin();

	return proc->REG_GPR[1];
}


