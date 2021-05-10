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
// FILE NAME       : instruction.h
// DEPARTMENT      : Digital System Architecture Lab.
// AUTHOR          : Kim Sung Yeon
// AUTHOR'S EMAIL  : jkjk0411@gmail.com
// -------------------------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION DATE         AUTHOR         DESCRIPTION
// 1.0     2020-03-24   Kim Sung Yeon  RISC-V Instruction Function Header File
// 2.0     2021-05-10   Lee Ji Hye     RISC-V Instruction Function Header File for RV64IM Extension  
// CONTACT EMAIL   : jihyelee317@gmail.com
// -------------------------------------------------------------------------------------------
// PURPOSE      : RISC-V ISS
//--------------------------------------------------------------------------------------------

#ifndef __INSTR__
#define	__INSTR__

#include "proc.h"

//RV32I
void _lui		(proc_t* proc, word inst);
void _auipc	    (proc_t* proc, word inst);
void _jal		(proc_t* proc, word inst);
void _jalr	    (proc_t* proc, word inst);

void _beq		(proc_t* proc, word inst);
void _bne		(proc_t* proc, word inst);
void _blt		(proc_t* proc, word inst);
void _bge		(proc_t* proc, word inst);
void _bltu	    (proc_t* proc, word inst);
void _bgeu	    (proc_t* proc, word inst);

void _lb  	    (proc_t* proc, word inst);
void _lh  	    (proc_t* proc, word inst);
void _lw  	    (proc_t* proc, word inst);
void _lbu 	    (proc_t* proc, word inst);
void _lhu 	    (proc_t* proc, word inst);

void _sb  	    (proc_t* proc, word inst);
void _sh  	    (proc_t* proc, word inst);
void _sw  	    (proc_t* proc, word inst);

void _addi	    (proc_t* proc, word inst);
void _slti	    (proc_t* proc, word inst);
void _sltiu	    (proc_t* proc, word inst);
void _xori	    (proc_t* proc, word inst);
void _ori 	    (proc_t* proc, word inst);
void _andi	    (proc_t* proc, word inst);

void _slli	    (proc_t* proc, word inst);
void _srli	    (proc_t* proc, word inst);
void _srai	    (proc_t* proc, word inst);

void _add 	    (proc_t* proc, word inst);
void _sub 	    (proc_t* proc, word inst);
void _sll 	    (proc_t* proc, word inst);
void _slt 	    (proc_t* proc, word inst);
void _sltu	    (proc_t* proc, word inst);
void _xor 	    (proc_t* proc, word inst);
void _srl 	    (proc_t* proc, word inst);
void _sra 	    (proc_t* proc, word inst);
void _or  	    (proc_t* proc, word inst);
void _and 	    (proc_t* proc, word inst);

void _fence	    (proc_t* proc, word inst);
void _fence_i	(proc_t* proc, word inst);

void _ecall  	(proc_t* proc, word inst);
void _ebreak 	(proc_t* proc, word inst);

void _csrrw  	(proc_t* proc, word inst);
void _csrrs  	(proc_t* proc, word inst);
void _csrrc  	(proc_t* proc, word inst);
void _csrrwi 	(proc_t* proc, word inst);
void _csrrsi 	(proc_t* proc, word inst);
void _csrrci 	(proc_t* proc, word inst);

//rv32m
void _mul    	(proc_t* proc, word inst);
void _mulh   	(proc_t* proc, word inst);
void _mulhsu 	(proc_t* proc, word inst);
void _mulhu  	(proc_t* proc, word inst);

void _div   	(proc_t* proc, word inst);
void _divu   	(proc_t* proc, word inst);
void _rem    	(proc_t* proc, word inst);
void _remu   	(proc_t* proc, word inst);

//rv32a
//later..
void _lr_w		(proc_t* proc, word inst);
void _lr_w		(proc_t* proc, word inst);

void _amoswap_w	(proc_t* proc, word inst);
void _amoadd_w	(proc_t* proc, word inst);
void _amoxor_w	(proc_t* proc, word inst);
void _amoand_w	(proc_t* proc, word inst);
void _amoor_w	(proc_t* proc, word inst);
void _amomin_w	(proc_t* proc, word inst);
void _amomax_w	(proc_t* proc, word inst);
void _amominu_w	(proc_t* proc, word inst);
void _amomaxu_w	(proc_t* proc, word inst);

//for RV64IM
void _addw    	(proc_t* proc, word inst);
void _subw    	(proc_t* proc, word inst);
void _sllw    	(proc_t* proc, word inst);
void _srlw    	(proc_t* proc, word inst);
void _sraw    	(proc_t* proc, word inst);

void _mulw    	(proc_t* proc, word inst);
void _divw    	(proc_t* proc, word inst);
void _divuw    	(proc_t* proc, word inst);
void _remw    	(proc_t* proc, word inst);
void _remuw    	(proc_t* proc, word inst);

void _addiw    	(proc_t* proc, word inst);
void _slliw    	(proc_t* proc, word inst);
void _srliw    	(proc_t* proc, word inst);
void _sraiw    	(proc_t* proc, word inst);


#endif
