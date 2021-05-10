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
// FILE NAME       : proc.h
// DEPARTMENT      : Digital System Architecture Lab.
// AUTHOR          : Kim Sung Yeon
// AUTHOR'S EMAIL  : jkjk0411@gmail.com
// -------------------------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION DATE         AUTHOR         DESCRIPTION
// 1.0     2020-03-24   Kim Sung Yeon  processor header file
// 2.0     2021-05-10   Lee Ji Hye     processor header file for RV64IM  
// CONTACT EMAIL   : jihyelee317@gmail.com
// -------------------------------------------------------------------------------------------
// PURPOSE      : RISC-V ISS
//--------------------------------------------------------------------------------------------

#ifndef __PROC__
#define	__PROC__

#include <stdint.h>
#include "decoder.h"


#define	GPR_SIZE		32                              //modified
#define MEM_SIZE        4*1024*1024*1024


typedef uint8_t		byte;
typedef uint16_t	half;
typedef uint32_t	word; 
typedef uint64_t	longl;                              //modified
typedef int8_t		s_byte;
typedef int16_t	    s_half;
typedef int32_t	    s_word; 
typedef int64_t	    s_longl;                            //modified



typedef struct 
{
	longl	PC;							//Program Counter                                           //m
	longl	REG_GPR[GPR_SIZE];			//General Purpose Registers                                 //m 
    longl   IS_Branch;                                                                             //m

	//byte	*IM;						//Instruction Memory
	//byte	*DM;						//Data Memory

	//byte MEMORY[MEM_SIZE];            //4GB Memory
    byte *MEMORY;

	unsigned long long exec_cycle;
} proc_t;

//functions
int init_proc(proc_t* proc, char* filename, longl pc_start);                    //pc_start datatype : word -> longl modified

#endif
