
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
int PA=0;

long int main(long int rd_data_1, long int rd_data_2, longl pc)   //(int argc, char** argv)/////////////////////////////////////////////////////////////////////////////////////
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
	word BP = 0;
    word status = 0;
	
	word break_state = 0;
	word key;
	unsigned long long cnt = 0;

    table_head = INIT_TABLE();

    BP=0;
    init_proc(	proc,
                "./alu_test.out",
		        pc
			 );

    inst = (word)inst_fetch(proc);
    _RS1=rd_data_1;
    _RS2=rd_data_2;


    inst_exe(proc, inst);			
    printf("instnum=\t\t  %d\t       GPR[%d]=%16lx\n",_PC/4+1,  _F_RD, _RD);

return (_RD);
}

