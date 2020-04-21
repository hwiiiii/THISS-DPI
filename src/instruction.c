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
// FILE NAME       : instruction.c
// DEPARTMENT      : Digital System Architecture Lab.
// AUTHOR          : Kim Sung Yeon
// AUTHOR'S EMAIL  : jkjk0411@gmail.com
// -------------------------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION DATE         AUTHOR         DESCRIPTION
// 1.0     2020-03-24   Kim Sung Yeon  RISC-V Instruction Function 
// -------------------------------------------------------------------------------------------
// PURPOSE      : RISC-V ISS
//--------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

#include "proc.h"
#include "instruction.h"
#include "decoder.h"


//RV32I
void _lui	(proc_t* proc, word inst)
{
    _RD  =   _U_IMM;
}
void _auipc	(proc_t* proc, word inst)
{
    _RD =   _PC + _U_IMM;
}
void _jal	(proc_t* proc, word inst)
{
    _RD  =_PC  + 4;
    _PC  =_PC  + _J_IMM;
    _IS_B=   1;
}
void _jalr	(proc_t* proc, word inst)
{
    _RD  =   _PC  + 4;
    _PC  =   (_RS1 + _I_IMM) & 0xfffffffe;
    _IS_B=   1;
}

void _beq	(proc_t* proc, word inst)
{
    _PC  =   (_RS1 == _RS2) ? _PC+_B_IMM : _PC+4;
    _IS_B=   1;
}
void _bne	(proc_t* proc, word inst)
{
    _PC  =   (_RS1 != _RS2) ? _PC+_B_IMM : _PC+4;
    _IS_B=   1;
}
void _blt	(proc_t* proc, word inst)
{
    _PC  =   ((s_word)_RS1 < (s_word)_RS2) ? _PC+_B_IMM : _PC+4;
    _IS_B=   1;
}
void _bge	(proc_t* proc, word inst)
{
    _PC  =   ((s_word)_RS1 >= (s_word)_RS2) ? _PC+_B_IMM : _PC+4;
    _IS_B=   1;
}
void _bltu	(proc_t* proc, word inst)
{
    _PC  =   ((word)_RS1 < (word)_RS2) ? _PC+_B_IMM : _PC+4;
    _IS_B=   1;
}
void _bgeu	(proc_t* proc, word inst)
{
    _PC  =   ((word)_RS1 >= (word)_RS2) ? _PC+_B_IMM : _PC+4;
    _IS_B=   1;
}

void _lb  	(proc_t* proc, word inst)
{
    _RD  =  (_MEM(_RS1 + _I_IMM) >> 7) * 0xFFFFFFF00  |
			_MEM(_RS1 + _I_IMM);
}
void _lh  	(proc_t* proc, word inst)
{
    _RD  =  (_MEM(_RS1 + _I_IMM+1) >> 7) * 0xFFFFF0000  |
			_MEM(_RS1 + _I_IMM+1)   <<  8   |
            _MEM(_RS1 + _I_IMM);
}
void _lw  	(proc_t* proc, word inst)
{
    _RD  =  _MEM(_RS1 + _I_IMM+3)   <<  24  |
			_MEM(_RS1 + _I_IMM+2)   <<  16  |
            _MEM(_RS1 + _I_IMM+1)   <<  8   |
            _MEM(_RS1 + _I_IMM);
}
void _lbu 	(proc_t* proc, word inst)
{
    _RD  =   _MEM(_RS1 + _I_IMM);
}
void _lhu 	(proc_t* proc, word inst)
{
    _RD  =   _MEM(_RS1 + _I_IMM+1)   <<  8   |
            _MEM(_RS1 + _I_IMM);
}

void _sb  	(proc_t* proc, word inst)
{
    _MEM(_RS1 + _S_IMM)     =	(_RS2 & 0xFF);
}
void _sh  	(proc_t* proc, word inst)
{
    _MEM(_RS1 + _S_IMM)     =   (_RS2 & 0xFF);
    _MEM(_RS1 + _S_IMM+1)   =   (_RS2 & 0xFF00)    >>  8;
}
void _sw  	(proc_t* proc, word inst)
{
    _MEM(_RS1 + _S_IMM)     =   (_RS2 & 0xFF);
    _MEM(_RS1 + _S_IMM+1)   =   (_RS2 & 0xFF00)    >>  8;
    _MEM(_RS1 + _S_IMM+2)   =   (_RS2 & 0xFF0000)   >>  16;
    _MEM(_RS1 + _S_IMM+3)   =   (_RS2 & 0xFF000000)  >>  24;
}

void _addi	(proc_t* proc, word inst)
{
    _RD  =_RS1 + _I_IMM;
}
void _slti	(proc_t* proc, word inst)
{
    _RD  =   ((s_word)_RS1 < (s_word)_I_IMM) ?   1 : 0;
}
void _sltiu	(proc_t* proc, word inst)
{
    _RD  =   ((word)_RS1 < (word)_I_IMM) ?   1 : 0;
}
void _xori	(proc_t* proc, word inst)
{
    _RD  =   _RS1 ^ _I_IMM;
}
void _ori 	(proc_t* proc, word inst)
{
    _RD  =   _RS1 | _I_IMM;
}
void _andi	(proc_t* proc, word inst)
{
    _RD  =   _RS1 & _I_IMM;
}

void _slli	(proc_t* proc, word inst)
{
    _RD  =   _RS1 << ((word)_I_IMM & 0x1F);
}
void _srli	(proc_t* proc, word inst)
{
    _RD  =   _RS1 >> ((word)_I_IMM & 0x1F);
}
void _srai	(proc_t* proc, word inst)
{
    //In C, ">>" is arithmatic right shift WHEN LHS(of shift) is singed
    _RD  =   (s_word)_RS1 >> ((word)_I_IMM & 0x1F);
}

void _add 	(proc_t* proc, word inst)
{
    _RD  =   _RS1 + _RS2;
}
void _sub 	(proc_t* proc, word inst)
{
    _RD  =   _RS1 - _RS2;
    //_RD  =   _RS1 - _RS2;
}
void _sll 	(proc_t* proc, word inst)
{
    _RD  =   _RS1 << (_RS2 & 0x1F);
}
void _slt 	(proc_t* proc, word inst)
{
    _RD  =   ((s_word)_RS1 < (s_word)_RS2) ? 1 : 0;
}
void _sltu	(proc_t* proc, word inst)
{
    _RD  =   ((word)_RS1 < (word)_RS2) ? 1 : 0;
}
void _xor 	(proc_t* proc, word inst)
{
    _RD  =   _RS1 ^ _RS2;
}
void _srl 	(proc_t* proc, word inst)
{
    _RD  =   _RS1 >> (_RS2 & 0x1F);
}
void _sra 	(proc_t* proc, word inst)
{
    _RD  =   (s_word)_RS1 >> (word)(_RS2 & 0x1F);
}
void _or  	(proc_t* proc, word inst)
{
    _RD  =   _RS1 | _RS2;
}
void _and 	(proc_t* proc, word inst)
{
    _RD  =   _RS1 & _RS2;
}

//no fence
void _fence	    (proc_t* proc, word inst)
{
	printf("no fence\n");
	exit(1);
}
void _fence_i	(proc_t* proc, word inst)
{
	printf("no fence\n");
	exit(1);
}

//no e?
void _ecall  	(proc_t* proc, word inst)
{
	printf("no ecall\n");
	exit(1);
}
void _ebreak 	(proc_t* proc, word inst)
{
	printf("no ecall\n");
	exit(1);
}

//no csr
void _csrrw  	(proc_t* proc, word inst)
{
	printf("no csr\n");
	exit(1);
}
void _csrrs  	(proc_t* proc, word inst)
{
	printf("no csr\n");
	exit(1);
}
void _csrrc  	(proc_t* proc, word inst)
{
	printf("no csr\n");
	exit(1);
}
void _csrrwi 	(proc_t* proc, word inst)
{
	printf("no csr\n");
	exit(1);
}
void _csrrsi 	(proc_t* proc, word inst)
{
	printf("no csr\n");
	exit(1);
}
void _csrrci 	(proc_t* proc, word inst)
{
	printf("no csr\n");
	exit(1);
}

//rv32m
void _mul    	(proc_t* proc, word inst)
{
    _RD  =   _RS1 * _RS2;
}
void _mulh   	(proc_t* proc, word inst)
{
    _RD  =   ((int64_t)(s_word)_RS1 * (int64_t)(s_word)_RS2) >>  32;
}
void _mulhsu 	(proc_t* proc, word inst)
{
    _RD  =   ((int64_t)(s_word)_RS1 * (uint64_t)_RS2) >>  32;
}
void _mulhu  	(proc_t* proc, word inst)
{
    _RD  =   ((uint64_t)_RS1 * (uint64_t)_RS2) >>  32;
}

void _div    	(proc_t* proc, word inst)
{
    _RD  =   ((s_word)_RS1 / (s_word)_RS2);
}
void _divu   	(proc_t* proc, word inst)
{
    _RD  =   ((word)_RS1 / (word)_RS2);
}
void _rem    	(proc_t* proc, word inst)
{
    _RD  =   ((s_word)_RS1 % (s_word)_RS2);
}
void _remu   	(proc_t* proc, word inst)
{
    _RD  =   (_RS1 % _RS2);
}
