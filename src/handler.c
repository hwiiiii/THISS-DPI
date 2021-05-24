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
// FILE NAME       : decoder.c
// DEPARTMENT      : Digital System Architecture Lab.
// AUTHOR          : Kim Sung Yeon
// AUTHOR'S EMAIL  : jkjk0411@gmail.com
// -------------------------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION DATE         AUTHOR         DESCRIPTION
// 1.0     2020-03-24   Kim Sung Yeon  decoder source file
// 2.0     2021-05-10   Kim Whi Jin    decoder source file for RV64IM  
// CONTACT EMAIL   : whijin98@gmail.com
// -------------------------------------------------------------------------------------------
// PURPOSE      : RISC-V ISS
//--------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "proc.h"
#include "instruction.h"
#include "decoder.h"
#include "list.h"

#define PA_OP       0xFFFFFFFF


//#define STATUS
//#define PA_STATUS

//extern PA_table *table_head;
//extern PA;

int inst_fetch(proc_t * proc)
{
    return  (   _MEM(_PC+3)   <<  24  |
                _MEM(_PC+2)   <<  16  |
                _MEM(_PC+1)   <<  8   |
                _MEM(_PC));
}

void inst_exe(proc_t * proc, word inst)
{
	int target = 0;
    int load_flag = (_OPCODE == _LOAD)     &&  (_FUNCT3 == 2);
    int store_flag = (_OPCODE == _STORE)    &&  (_FUNCT3 == 2);
    int load_address = _RS1+_I_IMM;
    int store_address = _RS1+_S_IMM;

	switch(_OPCODE)
	{
		case _LOAD			:
			switch(_FUNCT3)
			{
				case 0	:	_lb  	    (proc, inst); break;
				case 1	:	_lh  	    (proc, inst); break;
				case 2	:	_lw  	    (proc, inst); break;
				case 4	:	_lbu 	    (proc, inst); break;
				case 5	:	_lhu 	    (proc, inst); break;
				default	:	printf("unrecognized LOAD instruction\n"); exit(1);
			}break;
		//case _LOAD_FP		:
		//case _custom_0	:
		//case _MISC_MEM		:
		case _OP_IMM		:
			switch(_FUNCT3)
			{
				case 0	:	_addi	    (proc, inst); break;
				case 2	:	_slti	    (proc, inst); break;
				case 3	:	_sltiu	    (proc, inst); break;
				case 4	:	_xori	    (proc, inst); break;
				case 6	:	_ori 	    (proc, inst); break;
				case 7	:	_andi	    (proc, inst); break;

				case 1	:	_slli	    (proc, inst); break;
				case 5	:	
					switch(_FUNCT7)
					{
						case 0	:	_srli	    (proc, inst); break;
						case 32	:	_srai	    (proc, inst); break;
						default	:	printf("unrecognized OP_IMM_ instruction\n"); exit(1);
					}break;
				default	:	printf("unrecognized OP_IMM instruction\n"); exit(1);
			}break;
			
		case _AUIPC			: _auipc	    (proc, inst); break;
		//case _OP_IMM_32		:
		case _OP_IMM_32		:       //I type word instruction
			switch(_FUNCT3)
			{
				case 0	:	_addiw	    (proc, inst); break;
				case 1	:	_slliw	    (proc, inst); break;
				case 5	:	
					switch(_FUNCT7)
					{
						case 0	:	_srliw	    (proc, inst); break;
						case 32	:	_sraiw	    (proc, inst); break;
						default	:	printf("unrecognized OP_IMM_32 instruction\n"); exit(1);
					}break;
				default	:	printf("unrecognized OP_IMM_32 instruction\n"); exit(1);
			}break;
	

		case _STORE			:
			switch(_FUNCT3)
			{
				case 0	:	_sb  	    (proc, inst); break;
				case 1	:	_sh  	    (proc, inst); break;
				case 2	:	_sw  	    (proc, inst); break;
				default	:	printf("unrecognized STORE instruction\n"); exit(1);
			}break;
		//case _STORE_FP		:
		//case _custom_1	:
		//case _AMO			:
		case _OP			:    //R format
			switch(_FUNCT7)
			{
				case 0	:
					switch(_FUNCT3)
					{
						case 0	:	_add 	    (proc, inst); break;
						case 1	:	_sll 	    (proc, inst); break;
						case 2	:	_slt 	    (proc, inst); break;
						case 3	:	_sltu	    (proc, inst); break;
						case 4	:	_xor 	    (proc, inst); break;
						case 5	:	_srl 	    (proc, inst); break;
						case 6	:	_or  	    (proc, inst); break;
						case 7	:	_and 	    (proc, inst); break;
						default	:	printf("unrecognized OP_0 instruction\n"); exit(1);
					}break;
				case 1	:
					switch(_FUNCT3)
					{
						case 0	:	_mul    	(proc, inst); break;
						case 1	:	_mulh   	(proc, inst); break;
						case 2	:	_mulhsu 	(proc, inst); break;
						case 3	:	_mulhu  	(proc, inst); break;
						case 4	:	_div   	    (proc, inst); break;
						case 5	:	_divu   	(proc, inst); break;
						case 6	:	_rem    	(proc, inst); break;
						case 7	:	_remu   	(proc, inst); break;
						default	:	printf("unrecognized OP_1 instruction\n"); exit(1);
					}break;
				case 32	:
					switch(_FUNCT3)
					{
						case 0	:	_sub 	    (proc, inst); break;
						case 5	:	_sra 	    (proc, inst); break;
						default	:	printf("unrecognized OP_2 instruction\n"); exit(1);
					}break;
				default	:	printf("unrecognized OP instruction\n"); exit(1);
			}break;
	
		case _LUI			: _lui		(proc, inst); break;
    	case _OP_32			:    //R type word instruction
			switch(_FUNCT7)
			{
				case 0	:
					switch(_FUNCT3)
					{
						case 0	:	_addw 	    (proc, inst); break;
						case 1	:	_sllw 	    (proc, inst); break;
						case 5	:	_srlw	    (proc, inst); break;
					default	:	printf("unrecognized OP_32_0 instruction\n"); exit(1);
					}break;
				case 1	:
					switch(_FUNCT3)
					{
						case 0	:	_mulw    	(proc, inst); break;
						case 4	:	_divw   	(proc, inst); break;
						case 5	:	_divuw   	(proc, inst); break;
						case 6	:	_remw    	(proc, inst); break;
						case 7	:	_remuw   	(proc, inst); break;
						default	:	printf("unrecognized OP_32_1 instruction\n"); exit(1);
					}break;
				case 32	:
					switch(_FUNCT3)
					{
						case 0	:	_subw 	    (proc, inst); break;
						case 5	:	_sraw 	    (proc, inst); break;
						default	:	printf("unrecognized OP_32_2 instruction\n"); exit(1);
					}break;
				default	:	printf("unrecognized OP_32 instruction\n"); exit(1);
			}break;

		//case _MADD			:
		//case _MSUB			:
		//case _NMSUB			:
		//case _NMADD			:
		//case _OP_FP			:
		//case _reserved_0	:
		//case _custom_2	:

		case _BRANCH		:
			switch(_FUNCT3)
			{
				case 0	:	_beq		(proc, inst); break;
				case 1	:	_bne		(proc, inst); break;
				case 4	:	_blt		(proc, inst); break;
				case 5	:	_bge		(proc, inst); break;
				case 6	:	_bltu	    (proc, inst); break;
				case 7	:	_bgeu	    (proc, inst); break;
				default	:	printf("unrecognized branch instruction\n"); exit(1);
			}break;
		case _JALR			: _jalr	    (proc, inst); break;
		//case _reserved_1	:
		case _JAL			: _jal		(proc, inst); break;
		case _SYSTEM		: 
			#ifdef STATUS
			printf("%x:NoSYS\n", proc->PC); 
			#endif
			break;
		//case _reserved_2	:
		//case _custom_3	:
        default             : printf("unrecognized at all instruction\n"); exit(1); 
	}

	proc->REG_GPR[0] = 0;
}

