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
// 2.0     2021-05-10   Lee Ji Hye     RISC-V Instruction Function RV64IM Extension  
// CONTACT EMAIL   : jihyelee317@gmail.com
// -------------------------------------------------------------------------------------------
// PURPOSE      : RISC-V ISS
//--------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

#include "proc.h"
#include "instruction.h"
#include "decoder.h"

//64bit -> 32bit -> sign-ext -> 64bit

unsigned long long  sext64(unsigned long long val64) {
    
    unsigned long long value = (0x00000000FFFFFFFF & val64 ) ; //32bit
    unsigned long long mask  =  0x0000000080000000;
    if (mask & val64)   //if sign of 32bit  = 1 (not 0)
    {
        value += 0xFFFFFFFF00000000;
    }
    return value;
}

//32bit -> sign-ext -> 64bit

unsigned long long  sext32(unsigned int val32) {
    
    unsigned long long value = (0x00000000FFFFFFFF & val32 ) ; 
    int mask  =  0x80000000;
    if (mask & val32)   //if sign of 32bit  = 1 (not 0)
    {
        value += 0xFFFFFFFF00000000;
    }
    return value;
}

/////////////////////////////////////////////////////////////



//12bit i_imm -> sign-ext -> 64bit

signed long long  imm_sext12(unsigned int val12) {
    
    signed long long value = (0x0000000000000FFF & val12 ) ; 
    int mask  =  0x800;
    if (mask & val12)   //if sign of 12bit  = 1 (not 0)
    {
        value += 0xFFFFFFFFFFFFF000;
    }
    return value;
}














////////////////////////////////////////////////////////////

uint64_t mulhu_f(uint64_t a, uint64_t b)
{
  uint64_t t;
  uint32_t y1, y2, y3;
  uint64_t a0 = (uint32_t)a, a1 = a >> 32;
  uint64_t b0 = (uint32_t)b, b1 = b >> 32;

  t = a1*b0 + ((a0*b0) >> 32);
  y1 = t;
  y2 = t >> 32;

  t = a0*b1 + y1;
  y1 = t;

  t = a1*b1 + y2 + (t >> 32);
  y2 = t;
  y3 = t >> 32;

  return ((uint64_t)y3 << 32) | y2;
}


int64_t mulh_f(int64_t a, int64_t b)
{
  int negate = (a < 0) != (b < 0);
  uint64_t res = mulhu_f(a < 0 ? -a : a, b < 0 ? -b : b);
  return negate ? ~res + (a * b == 0) : res;
}

int64_t mulhsu_f(int64_t a, uint64_t b)
{
  int negate = a < 0;
  uint64_t res = mulhu_f(a < 0 ? -a : a, b);
  return negate ? ~res + (a * b == 0) : res;
}

//---------------------------------------------------


//RV32I
void _lui	(proc_t* proc, word inst)
{
    //_RD = (s_longl)(int32_t)(_U_IMM << 12);
    _RD = sext32(_U_IMM << 12);
}
void _auipc	(proc_t* proc, word inst)
{
    _RD =   _PC + (s_longl)((int32_t)_U_IMM << 12);
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
    _PC  =   (_RS1 + _I_IMM) & 0xfffffffffffffffe;          //m
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
    _PC  =   ((s_longl)_RS1 < (s_longl)_RS2) ? _PC+_B_IMM : _PC+4;
    _IS_B=   1;
}
void _bge	(proc_t* proc, word inst)
{
    _PC  =   ((s_longl)_RS1 >= (s_longl)_RS2) ? _PC+_B_IMM : _PC+4;
    _IS_B=   1;
}
void _bltu	(proc_t* proc, word inst)
{
    _PC  =   ((longl)_RS1 < (longl)_RS2) ? _PC+_B_IMM : _PC+4;
    _IS_B=   1;
}
void _bgeu	(proc_t* proc, word inst)
{
    _PC  =   ((longl)_RS1 >= (longl)_RS2) ? _PC+_B_IMM : _PC+4;
    _IS_B=   1;
}

void _lb  	(proc_t* proc, word inst)       //???
{
    _RD  =  (_MEM(_RS1 + _I_IMM) >> 7) * 0xffffffffFFFFFFF00  |
			_MEM(_RS1 + _I_IMM);
}
void _lh  	(proc_t* proc, word inst)       //???
{
    _RD  =  (_MEM(_RS1 + _I_IMM+1) >> 7) * 0xffffffffFFFFF0000  |
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
    //printf((signed long long)_I_IMM); -> error
    _RD  = _RS1 + imm_sext12(_I_IMM);            
    //_RD  =(signed long long)_RS1 + (signed long long)_I_IMM;            
    //_RD  = _RS1;                                  
    //_RD  =(signed int)_RS1 + (signed int)_I_IMM;   
    //_RD  =0xffffffffffffffff | _I_IMM;                                  
}
void _slti	(proc_t* proc, word inst)
{
    _RD  =   ((s_longl)_RS1 < (s_longl)(imm_sext12(_I_IMM))) ?   1 : 0;       
}
void _sltiu	(proc_t* proc, word inst)
{
    _RD  =   ((longl)_RS1 < (longl)(imm_sext12(_I_IMM))) ?   1 : 0;///////////////////////////
}
void _xori	(proc_t* proc, word inst)
{
    _RD  =   _RS1 ^ imm_sext12(_I_IMM);
}
void _ori 	(proc_t* proc, word inst)
{
    _RD  =   _RS1 | imm_sext12(_I_IMM);
}
void _andi	(proc_t* proc, word inst)
{
    _RD  =   _RS1 & imm_sext12(_I_IMM);
}

void _slli	(proc_t* proc, word inst)
{
    _RD  =   _RS1 << ((longl)_I_IMM & 0x3F);
}
void _srli	(proc_t* proc, word inst)
{
    _RD  =   _RS1 >> ((longl)_I_IMM & 0x3F);
}
void _srai	(proc_t* proc, word inst)
{
    _RD  =   (s_longl)_RS1 >> ((longl)_I_IMM & 0x3F);
}

//In C, ">>" is arithmatic right shift WHEN LHS(of shift) is singed

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
    _RD  =   _RS1 << (_RS2 & 0x3F);
}
void _slt 	(proc_t* proc, word inst)
{
    _RD  =   ((s_longl)_RS1 < (s_longl)_RS2) ? 1 : 0;
}
void _sltu	(proc_t* proc, word inst)
{
    _RD  =   ((longl)_RS1 < (longl)_RS2) ? 1 : 0;
}
void _xor 	(proc_t* proc, word inst)
{
    _RD  =   _RS1 ^ _RS2;
}
void _srl 	(proc_t* proc, word inst)
{
    _RD  =   _RS1 >> (_RS2 & 0x3F);
}
void _sra 	(proc_t* proc, word inst)
{
    _RD  =   (s_longl)_RS1 >> (longl)(_RS2 & 0x3F);
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
    _RD = mulh_f(_RS1, _RS2);
}
void _mulhsu 	(proc_t* proc, word inst)
{
    _RD = mulhsu_f(_RS1, _RS2); 
}
void _mulhu  	(proc_t* proc, word inst)
{
    _RD = mulhu_f(_RS1, _RS2);
}

void _div    	(proc_t* proc, word inst)
{
    _RD  =   ((s_longl)_RS1 / (s_longl)_RS2);
}
void _divu   	(proc_t* proc, word inst)
{
    _RD  =   ((longl)_RS1 / (longl)_RS2);
}
void _rem    	(proc_t* proc, word inst)
{
    _RD  =   ((s_longl)_RS1 % (s_longl)_RS2);
}
void _remu   	(proc_t* proc, word inst)
{
    _RD  =   ((longl)_RS1 % (longl)_RS2);
}



//for 64bit

void _addw    	(proc_t* proc, word inst)
{
    _RD  = sext64(_RS1 + _RS2);
}
void _subw    	(proc_t* proc, word inst)
{
    _RD = sext64(_RS1 - _RS2);
}
void _sllw    	(proc_t* proc, word inst)
{
    _RD = sext64( _RS1 << (_RS2 & 0x1F) );        //rs2[4:0] = shift amount
}
void _srlw    	(proc_t* proc, word inst)
{
    _RD = sext32((word)_RS1 >> (_RS2 & 0x1F) );           //rs1 to 32
}
void _sraw    	(proc_t* proc, word inst)
{
    _RD = sext32((s_word)_RS1 >> (_RS2 & 0x1F) );       //rs1 to 32
}



void _mulw    	(proc_t* proc, word inst)
{
    _RD  =   sext64( _RS1 * _RS2 );                            
}
void _divw    	(proc_t* proc, word inst)       
{
    _RD  =   sext32( ((s_word)_RS1 / (s_word)_RS2) );
}
void _divuw    	(proc_t* proc, word inst)
{
    _RD  =  sext32( ((word)_RS1 / (word)_RS2) );
}
void _remw    	(proc_t* proc, word inst)
{
    _RD  = sext32(  ((s_word)_RS1 % (s_word)_RS2) );
}
void _remuw    	(proc_t* proc, word inst)
{
    _RD  = sext32(  ((word)_RS1 % (word)_RS2) );
}



void _addiw    	(proc_t* proc, word inst)
{
    _RD  = sext64( _RS1 + imm_sext12(_I_IMM)) ;          
}
void _slliw    	(proc_t* proc, word inst) 
{
    _RD  =  sext32( _RS1 << (_I_IMM & 0x3F) );
}
void _srliw    	(proc_t* proc, word inst)
{
    _RD  =  sext32( (word)_RS1 >> (_I_IMM & 0x3F) );   //rs1 to 32
}
void _sraiw    	(proc_t* proc, word inst)
{
    _RD  =  sext32( (s_word)_RS1 >> (_I_IMM & 0x3F) ); //rs1 to 32
}


