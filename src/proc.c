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
// FILE NAME       : proc.c
// DEPARTMENT      : Digital System Architecture Lab.
// AUTHOR          : Kim Sung Yeon
// AUTHOR'S EMAIL  : jkjk0411@gmail.com
// -------------------------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION DATE         AUTHOR         DESCRIPTION
// 1.0     2020-03-24   Kim Sung Yeon  processor source file
//                                     This file contains ELF parsing function
// -------------------------------------------------------------------------------------------
// PURPOSE      : RISC-V ISS
//--------------------------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <libelf.h>
#include <gelf.h>

#include "proc.h"
#include "decoder.h"

//#define STATUS
//#define SAVE_INIT_MEM



int init_proc(proc_t* proc, char* filename, word pc_start)
{
    int i;
	int fd;
	Elf *e;
	Elf_Kind ek;
	GElf_Shdr shdr;
    GElf_Ehdr ehdr;
	size_t shstrndx;
	size_t n;
	Elf_Scn *scn = NULL;
	char *name;
	char *p;
	Elf_Data *data;

	long long mem_size = 0x100000000;

	proc->MEMORY = (byte *)malloc(mem_size);

    if(elf_version(EV_CURRENT) == EV_NONE)
    {
        fprintf(stderr, "ELF lib init failed\n");
        return -1;
    }
	if((fd = open(filename, O_RDONLY, 0)) < 0)
	{
		fprintf(stderr, "open elf file failed\n");
		return -1;
	}
    if((e = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
    {
        fprintf(stderr, "elf_begin() failed\n");
		elf_end(e);
        return -1;
    }

	ek = elf_kind(e);

	#ifdef STATUS
	switch (ek) {
		case ELF_K_AR:
			printf("archive\n");
			break;
		case ELF_K_ELF:
			printf("elf obj\n");
			break;
		case ELF_K_NONE:
			printf("data\n");
			break;
		default:
			printf("unrecognized\n");
	}
	#endif

	if(gelf_getehdr(e, &ehdr) == NULL)
	{
		fprintf(stderr, "gelf_getehdr failed\n");
		elf_end(e);
		return -1;
	}
    

	if(elf_getshdrstrndx(e, &shstrndx) != 0)
	{
		fprintf(stderr, "elf_getshdrstrndx failed\n");
		elf_end(e);
		return -1;
	}

	#ifdef STATUS
    printf("set PC\n");
	#endif

    proc->PC = ehdr.e_entry;

	#ifdef STATUS
    printf("start PC : %x\n",proc->PC);
	printf("Scan ELF\n");
	#endif

	while ((scn = elf_nextscn(e, scn)) != NULL)
	{
		data = NULL;
		n = 0;
		// get a section header
		if(gelf_getshdr(scn, &shdr) != &shdr)
		{
			fprintf(stderr, "gelf_getshdr failed\n");
			elf_end(e);
			return -1;
		}

		// get the name of the section
		if((name = elf_strptr(e, shstrndx, shdr.sh_name)) == NULL)
		{
			fprintf(stderr, "elf_strptr failed\n");
			elf_end(e);
			return -1;
		}

	#ifdef STATUS
		printf("\nsection %d\t%s\n",(word)elf_ndxscn(scn), name);
		printf("shdr.sh_addr = %x\n", shdr.sh_addr);
		printf("shdr.sh_size = %x\n", shdr.sh_size);
	#endif

		// .text section
		if(strncmp(name,".text",5) == 0)
		{
            //*program_limit = shdr.sh_addr;
            //*data_limit = shdr.sh_addr;

			// this check is done by linker

			//if((im_size!=0)&&(shdr.sh_size > im_size))
			//{
			//	printf("intruction memory required = %x\n", shdr.sh_addr);
			//	fprintf(stderr, "Not enough instruction memory.\n");
			//	elf_end(e);
			//	return -1;
			//}
            while(n < shdr.sh_size && (data = elf_getdata(scn, data)) != NULL)
            {
                p = (char *) data->d_buf;
                while(p < (char *)data->d_buf + data->d_size)
                {
                    // writing to mem
                    _MEM(shdr.sh_addr + n) = *p;
                    n++; p++;
                }
            }
		}

		// data section
		else if((shdr.sh_flags&0x2) != 0)
		{
			//.bss
			if((strncmp(name, ".bss",4) == 0) || (strncmp(name, ".sbss",5) == 0))
			{
				// this check is done by linker

				//if((dm_size!=0)&&(shdr.sh_size >dm_size))
				//{
				//	printf("data memory required = %x\n", shdr.sh_addr);
				//	fprintf(stderr, "Not enough data memory.\n");
				//	elf_end(e);
				//	return -1;
				//}
				while(n < shdr.sh_size && (data = elf_getdata(scn, data)) != NULL)
				{
					p = (char *) data->d_buf;
					while(p < (char *)data->d_buf + data->d_size)
					{
						// writing to mem
						_MEM(shdr.sh_addr + n) = 0;
						n++; p++;
					}
				}
			}

			//.data
			//else if(strncmp(name, ".data", 5) == 0)
			else
			{
				// this check is done by linker
				//if((dm_size!=0)&&(shdr.sh_size > dm_size))
				//{
				//	printf("data memory required = %x\n", shdr.sh_addr);
				//	fprintf(stderr, "Not enough data memory.\n");
				//	elf_end(e);
				//	return -1;
				//}
				while(n < shdr.sh_size && (data = elf_getdata(scn, data)) != NULL)
				{
					p = (char *) data->d_buf;
					while(p < (char *)data->d_buf + data->d_size)
					{
						// writing to mem
						_MEM(shdr.sh_addr + n) = *p;
						n++; p++;
					}
				}
			}
			//else
			//{
			//	printf("unknown data section\n");
			//}
		}
		//else
		//{
		//	printf("unknown section\n");
		//}
	}
	#ifdef SAVE_INIT_MEM
	printf("\nstart dumping initialized memory\n");
	int div_num = 8;
	char f_name[12] = "INIT_MEM_0";
	FILE *fp;

	for(i=0 ; i<1 ; i++)
	{
		f_name[9] = i + '0';	
		fp = fopen(f_name, "w");
		printf("dumping %d\tfile...\n",i);
		fwrite((proc->MEMORY + ((mem_size/div_num) * i)), sizeof(byte), mem_size/div_num, fp);
		fclose(fp);
	}
	printf("dump initialized memroy finished\n");
	exit(1);
	
	#endif
				
	elf_end(e);

	if(pc_start != 0)
	{
		proc->PC = pc_start;
	}

    //INIT GPR
    for(i = 0 ; i<GPR_SIZE ; i++)
    {
        proc->REG_GPR[i] = 0;
    }

    proc->IS_Branch = 0;
	//temporary SP
	proc->REG_GPR[2] = 0x80000000;
	//temporary FP
	proc->REG_GPR[8] = 0x80000000;

    return 0;
}


