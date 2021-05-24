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
// FILE NAME       : decoder.h
// DEPARTMENT      : Digital System Architecture Lab.
// AUTHOR          : Kim Sung Yeon
// AUTHOR'S EMAIL  : jkjk0411@gmail.com
// -------------------------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION DATE         AUTHOR         DESCRIPTION
// 1.0     2020-03-24   Kim Sung Yeon  decoder header file
// 2.0     2021-05-10   Kim Whi Jin    decoder header file for RV64IM  
// CONTACT EMAIL   : whijin98@gmail.com
// -------------------------------------------------------------------------------------------
// PURPOSE      : RISC-V ISS
//--------------------------------------------------------------------------------------------

#ifndef __DECODE__
#define	__DECODE__
#include "proc.h"

//Format
#define _OPCODE			(	(inst)		& 0x7F	)
#define _F_RD			(	(inst)>>7	& 0x1F	)
#define _FUNCT3			(	(inst)>>12	& 0x7	)	
#define _F_RS1			(	(inst)>>15	& 0x1F	)
#define	_F_RS2			(	(inst)>>20	& 0x1F	)
#define _FUNCT7			(	(inst)>>25	& 0x7F	)

#define _RD             ((proc->REG_GPR[(_F_RD)]))
#define _RS1            ((proc->REG_GPR[(_F_RS1)]))
#define _RS2            ((proc->REG_GPR[(_F_RS2)]))
#define _PC             ((proc->PC))
#define _MEM(addr)      ((proc->MEMORY[(addr)]))
#define _IS_B           ((proc->IS_Branch))

//Immediate
#define _I_IMM   (((inst)>>20) & 0x00000fff)

//#define _I_IMM		   (((inst)>>31	& 0x1	)	*	0xFFFFF800	|	\
						((inst)>>25	& 0x3F	)	<<	5			|	\
						((inst)>>21	& 0xF	)	<<	1			|	\
						((inst)>>20 & 0x1	)	<<	0			)
#define _S_IMM		   (((inst)>>31	& 0x1	)	*	0xFFFFF800	|	\
						((inst)>>25	& 0x3F	)	<<	5			|	\
						((inst)>>8	& 0xF	)	<<	1			|	\
						((inst)>>7  & 0x1	)	<<	0			)
#define _B_IMM		   (((inst)>>31	& 0x1	)	*	0xFFFFF000	|	\
						((inst)>>7	& 0x1	)	<<	11			|	\
						((inst)>>25	& 0x3F	)	<<	5			|	\
						((inst)>>8  & 0xF	)	<<	1			)
//#define _U_IMM		   (((inst)>>31	& 0x1	)	<<	19        	|	\
						((inst)>>20 & 0x7FF )	<<	8			|	\
						((inst)>>12	& 0xFF	)				)
#define _U_IMM  (((inst)>>12) & 0x000fffff)


#define _J_IMM		   (((inst)>>31	& 0x1	)	* 	0xFFF00000	|	\
						((inst)>>12 & 0xFF	)	<<	12			|	\
						((inst)>>20	& 0x1 	)	<<	11			|	\
						((inst)>>25 & 0x3F	)	<<	5 			|	\
						((inst)>>21 & 0xF	)	<<	1 			)	

//OPCODE
#define _LOAD			0x03
//#define	_LOAD_FP			0x07
//#define	_custom_0		0x0B
//#define	_MISC_MEM		0x0F    //fence?




#define	_STORE			0x23
//#define	_STORE_FP		0x27
//#define	_custom_1		0x2B
//#define	_AMO				0x2F



#define	_OP				0x33    // 64 bit R type inst
#define	_OP_32			0x3B    // 64 bit R type word inst

#define	_OP_IMM			0x13    // 64 bit I type inst
#define	_OP_IMM_32		0x1B    // 64 bit I type word inst

#define	_LUI		    0x37    
#define	_AUIPC			0x17

//#define	_MADD			0x43    //32 F or D
//#define	_MSUB			0x47    //32 F or D
//#define	_NMSUB			0x4B    //32 F or D
//#define	_NMADD			0x4F    //32 F or D
//#define	_OP_FP			0x53    //32 F or D
//#define	_reserved_0		0x57
//#define	_custom_2		0x5B

#define	_BRANCH			0x63
#define	_JALR			0x67
//#define	_reserved_1		0x6B
#define	_JAL			0x6F
#define	_SYSTEM			0x73    //no ecall and csr
//#define	_reserved_2		0x77
//#define	_custom_3			0x7B

#endif
