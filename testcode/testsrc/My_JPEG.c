/*
 *-----------------------------------------------------------------------------
 * The confidential and proprietary information contained in this file may
 * only be used by a person authorised under and to the extent permitted
 * by a subsisting licensing agreement from ARM Limited or its affiliates.
 *
 *            (C) COPYRIGHT 2010-2013 ARM Limited or its affiliates.
 *                ALL RIGHTS RESERVED
 *
 * This entire notice must be reproduced on all copies of this file
 * and copies of this file may only be made by a person if such person is
 * permitted to do so under the terms of a subsisting license agreement
 * from ARM Limited or its affiliates.
 *
 *      SVN Information
 *
 *      Checked In          : $Date: 2017-07-25 15:10:13 +0100 (Tue, 25 Jul 2017) $
 *
 *      Revision            : $Revision: 368444 $
 *
 *      Release Information : Cortex-M0 DesignStart-r2p0-00rel0
 *-----------------------------------------------------------------------------
 */

#ifdef CORTEX_M0
#include "CMSDK_CM0.h"
#include "core_cm0.h"
#endif

#ifdef CORTEX_M0PLUS
#include "CMSDK_CM0plus.h"
#include "core_cm0plus.h"
#endif

//#include "uart_stdout.h"

#include	<stdio.h>
#include	<stdlib.h>

#define	RRST
#define	CDSTY_VSC
//#define	use
#define	pic3
#define	MAX_IMAGE_HEIGHT	1024		
#define	MAX_IMAGE_WIDTH		1024	
#define	MAX_IMAGE_COMPS		3
#define	TILE_WIDTH			64	
#define	TILE_HEIGHT			64	
/*
#define	IMAGEW				640	
#define	IMAGEH				448	
#define	COLR				1
*/
#ifdef	pic0
#define		OUTFILE		"./out_grad128mono.j2k"
#define		FILENAME	"../img_src/grad128mono.raw"
#define		IMAGEW		128	
#define		IMAGEH		128	
#define		COLR		1
#endif
#ifdef	pic1
#include	"parrot512.h"
#define		OUTFILE		"./out_parrot512.j2k"
#define		FILENAME	"../img_src/parrot512.raw"
#define		IMAGEW		512	
#define		IMAGEH		512	
#define		COLR		3
#endif
#ifdef	pic2
#include	"lenna512.h"
#define		OUTFILE		"./out_lenna512.j2k"
#define		FILENAME	"../img_src/lenna512.raw"
#define		IMAGEW		512	
#define		IMAGEH		512	
#define		COLR		1
#endif
#ifdef	pic3
#include	"lenna256.h"
#define		OUTFILE		"./rv_lenna256.j2k"
#define		FILENAME	"../img_src/lenna.raw"
#define		IMAGEW		256	
#define		IMAGEH		256	
#define		COLR		1
#endif

#define	CBLK_HEIGHT			32	
#define	CBLK_WIDTH			32	
#define CTRL_INIT		0
#define CTRL_WRITE_BIT	1
#define CTRL_WRITE		2
#define CTRL_PUT		3
#define CTRL_READ_BIT_PT	4
#define CTRL_READ_PT	5
#define CTRL_READ		6
#define CTRL_SKIP		7

#define CTRL_ENCODE		10
#define CTRL_FLUSH		11
#define	CTRL_RESTART	12

#define CBLK_LEN			0
#define CBLK_NUM_BITPLANE	2
#define CBLK_TOT_PASS		3
#define	CBLK_PASS_LEN		4

#define MQC_NUMCTXS 19

#define MQC_STATE_QE		0
#define MQC_STATE_NMPS		1
#define MQC_STATE_NLPS		2
#define MQC_STATE_SWITCH	3

#define T1_SIG 0x10
#define T1_REFINE 0x20
#define T1_VISIT 0x40

#define MAX_TGT_LEVEL	1	

#define JP2_MS_SOC 0xff4f	/**< SOC marker value */
#define JP2_MS_SOT 0xff90	/**< SOT marker value */
#define JP2_MS_SOD 0xff93	/**< SOD marker value */
#define JP2_MS_EOC 0xffd9	/**< EOC marker value */
#define JP2_MS_SIZ 0xff51	/**< SIZ marker value */
#define JP2_MS_COD 0xff52	/**< COD marker value */
#define JP2_MS_COC 0xff53	/**< COC marker value */
#define JP2_MS_QCD 0xff5c	/**< QCD marker value */
#define JP2_MS_QCC 0xff5d	/**< QCC marker value */
#define JP2_MS_SOP 0xff91	/**< SOP marker value */
#define JP2_MS_EPH 0xff92	/**< EPH marker value */


#ifdef SYISS
#include "../src/common.h"
#endif

/* -------------------------------------------------------------------------- */
//Internal SRAM
//unsigned	char	tile_buffer[TILE_WIDTH * TILE_HEIGHT];

//Register & Internal Memory in DWT
unsigned	int		ENDWT;
unsigned	int		DONEDWT;
unsigned	int	tile_in[TILE_WIDTH*(TILE_WIDTH)];

int len, total_len;

//Flash memory			
unsigned	char	image_dst_buf[MAX_IMAGE_WIDTH * MAX_IMAGE_HEIGHT * MAX_IMAGE_COMPS];
//Image Source (Maybe External SDRAM)
//unsigned	char	image_src_buf[MAX_IMAGE_WIDTH * MAX_IMAGE_HEIGHT * MAX_IMAGE_COMPS];

/* -------------------------------------------------------------------------------------------- */

int	fix_mul(int a, int b)
{
	long long	temp = (long long)a * (long long)b;
//	int	temp = a * b;

	temp	+= temp & 4096;
	return	(int)(temp >>13);
}

int	fix_mul12(int a, int b)
{
	long long	temp = (long long)a * (long long)b;
//	int	temp = a * b;
	temp	+= temp & 2048;
	return	(int)(temp >>12);
}
int	fix_mul10(int a, int b)
{
	long long	temp = (long long)a * (long long)b;
//	int	temp = a * b;
	temp	+= temp &512 ;
	return	(int)(temp >>10);
}

int	fix_mul8(int a, int b)
{
	long long	temp = (long long)a * (long long)b;
//	int	temp = a * b;
	temp	+= temp &128 ;
	return	(int)(temp >>8);
}

int int_max(int a, int b) {
	return (a > b) ? a : b;
}

int int_abs(int a) {
	return a < 0 ? -a : a;
}

int int_floorlog2(int a) {
	int l;
	for (l = 0; a > 1; l++) {
		a >>= 1;
	}
	return l;
}

int int_ceildivpow2(int a, int b) {
	return (a + (1 << b) - 1) >> b;
}

int int_min(int a, int b) {
	return a < b ? a : b;
}



/* ------------------------------------------------------------------------ */

int	tile_in_buffer (int mode, int clr, int x, int y, int v, int tile_w)
{
	int	ret	= 0;
	static	short	tile_out_dwt[TILE_WIDTH*TILE_HEIGHT];	

	switch(mode)
	{
		case	CTRL_WRITE :
			switch(clr)
			{
				case	0 :
					tile_out_dwt[y * TILE_WIDTH+ x]	= v;
					break;
				case	1 : 
					tile_out_dwt[y * TILE_WIDTH+ x]	= v;
					break;
				case	2 :
					tile_out_dwt[y * TILE_WIDTH+ x]	= v;
					break;
			}
			break;
			
		case	CTRL_READ :
			switch(clr)
			{
				case	0 :
					ret	= tile_out_dwt[y * TILE_WIDTH+ x];
					break;
				case	1 :
					ret	= tile_out_dwt[y * TILE_WIDTH+ x];
					break;
				case	2 :
					ret	= tile_out_dwt[y * TILE_WIDTH+ x];
					break;
			}
			break;
	}

	return ret;
}

/* ------------------------------------------------------------------------------------------------ */

int	tile_out_buffer (int mode, int v, int n)
{
	static	int	byte_pt = 0;
	static	int	bit_pt	= 7;
	static	int	max_len	= TILE_WIDTH * TILE_HEIGHT;
	static	unsigned char	tile_buffer[TILE_WIDTH * TILE_HEIGHT];
	
	int	i, 
		pt,
		value;
	value	= 0;

	switch(mode)
	{
		/* initiate */
		case	CTRL_INIT :
			byte_pt	= 0;
			bit_pt	= 7;
			for(i =0; i <max_len; i++)
				tile_buffer[i]	= 0x0000;
			break;
		/* write bit */
		case	CTRL_WRITE_BIT :
			value = tile_buffer[byte_pt];

			for (i =n-1; i >=0; i--)
			{
		
				value |= (((v >> i) & 0x01) << bit_pt);
				bit_pt--;

				if (bit_pt <0)
				{
					tile_buffer[byte_pt]	= value;

					if (value ==0xFF)
						bit_pt	= 6;
					else
						bit_pt	= 7;

					byte_pt++;
					value = tile_buffer[byte_pt];
				}
			}
			tile_buffer[byte_pt] = value;
			break;
		
		case	CTRL_WRITE :

			if(bit_pt <7)
			{
				byte_pt++;
				bit_pt	=7;
			}

			for(i =0; i <n; i++)
			{
				tile_buffer[byte_pt]	= (v >> (n -1 -i) *8) & 0xFF;
				byte_pt++;
				if(byte_pt >=max_len)
					return -1;
			}
			break;

		case	CTRL_PUT :
			pt	= n& 0xFFFFFF;
			n	= (n>>24)	& 0xFF;

			for(i =0; i <n; i++)
			{
				tile_buffer[pt++]	= (v >> (n -1 -i) *8) & 0xFF;
			}

			break;

		case	CTRL_READ_BIT_PT :
			return	bit_pt;
			break;

		case	CTRL_READ_PT :
			return	byte_pt;
			break;

		case	CTRL_READ :
			return tile_buffer[n];

		case	CTRL_SKIP :
			byte_pt += n;
			bit_pt	= 7;
			break;

	}

	return 0;
}



#if 1
/* -------------------------------------------------------------------------- */


int	ctrl_cblk_flags(int mode, int x, int y, int flag, int cblk_w)
{
	static	char cblk_flags[CBLK_HEIGHT * CBLK_WIDTH];
	int	ret;
	int i;
	if(mode ==CTRL_INIT)
	{
		for(i=0;i<CBLK_HEIGHT * CBLK_WIDTH; i++)
			cblk_flags[i] = 0;
	}
	if(mode ==CTRL_WRITE)
	{
		cblk_flags[y*cblk_w+x]	=flag;
		ret	=0;
	}
	else
	{
		ret	= cblk_flags[y*cblk_w+x];
	}
	return	ret;
	
}
/* -------------------------------------------------------------------------------------------- */

unsigned char	cblk_in_buffer (int mode, int clr, int cur_cblk, int cur_pt, unsigned char v)
{
	static	unsigned char	cblk_buffer0[128*2];
	static	unsigned char	cblk_buffer1[128*2];
	static	unsigned char	cblk_buffer2[128*2];
	static	unsigned char	cblk_buffer3[128*2];
	static	unsigned char	cblk_buffer4[512*2];
	static	unsigned char	cblk_buffer5[512*2];
	static	unsigned char	cblk_buffer6[512*2];
	static	unsigned char	cblk_buffer7[2048*2];
	static	unsigned char	cblk_buffer8[2048*2];
	static	unsigned char	cblk_buffer9[2048*2];
	
	unsigned char	ret;

	switch(mode)
	{
		case	CTRL_WRITE :
			switch(cur_cblk)
			{
				case	0:
					cblk_buffer0[cur_pt]	= v;
					break;
				case	1:
					cblk_buffer1[cur_pt]	= v;
					break;
				case	2:
					cblk_buffer2[cur_pt]	= v;
					break;
				case	3:
					cblk_buffer3[cur_pt]	= v;
					break;
				case	4:
					cblk_buffer4[cur_pt]	= v;
					break;
				case	5:
					cblk_buffer5[cur_pt]	= v;
					break;
				case	6:
					cblk_buffer6[cur_pt]	= v;
					break;
				case	7:
					cblk_buffer7[cur_pt]	= v;
					break;
				case	8:
					cblk_buffer8[cur_pt]	= v;
					break;
				case	9:
					cblk_buffer9[cur_pt]	= v;
					break;

			}
			
			break;
			
		case	CTRL_READ :
			switch(cur_cblk)
			{
				case	0:
					ret	= cblk_buffer0[cur_pt];
					break;
				case	1:
					ret	= cblk_buffer1[cur_pt];
					break;
				case	2:
					ret	= cblk_buffer2[cur_pt];
					break;
				case	3:
					ret	= cblk_buffer3[cur_pt];
					break;
				case	4:
					ret	= cblk_buffer4[cur_pt];
					break;
				case	5:
					ret	= cblk_buffer5[cur_pt];
					break;
				case	6:
					ret	= cblk_buffer6[cur_pt];
					break;
				case	7:
					ret	= cblk_buffer7[cur_pt];
					break;
				case	8:
					ret	= cblk_buffer8[cur_pt];
					break;
				case	9:
					ret	= cblk_buffer9[cur_pt];
					break;

			}
			
			break;
	}

	return ret;
}	

/* ------------------------------------------------------------------------------------------------ */


int	ctrl_cblk_info(int sel, int mode, int no_comp, int no_cblk, int no_pass, int v)
{
	static	int	cblk_len[MAX_IMAGE_COMPS][10];
	static	char	cblk_tot_pass[MAX_IMAGE_COMPS][10];
	static	char	cblk_num_bit_plane[MAX_IMAGE_COMPS][10];
	static	char	cblk_pass_len[MAX_IMAGE_COMPS][10][50];
	int		ret	=0;

	switch(sel)
	{
		case	CBLK_LEN:
			if(mode ==CTRL_READ)
				ret	= cblk_len[no_comp][no_cblk];
			else
				cblk_len[no_comp][no_cblk]		= v;
			break;
		case	CBLK_NUM_BITPLANE:
			if(mode ==CTRL_READ)
				ret	= cblk_num_bit_plane[no_comp][no_cblk];
			else
				cblk_num_bit_plane[no_comp][no_cblk]	= v;
			break;
		case	CBLK_TOT_PASS:
			if(mode ==CTRL_READ)
			{
				ret	= cblk_tot_pass[no_comp][no_cblk];
			}
			else{
				cblk_tot_pass[no_comp][no_cblk]	= v;
			}
			break;
		case	CBLK_PASS_LEN:
			if(mode==CTRL_READ)
				ret	= cblk_pass_len[no_comp][no_cblk][no_pass];
			else
				cblk_pass_len[no_comp][no_cblk][no_pass]	= v;
			break;
	}
	return	ret;

}
/* ----------------------------------------------------------------------- */

int	tier1_bac(int mode, int cx, int d, int no_comp, int no_cblk, unsigned char last_data)
{
	static	int	mqc_states[47][4] = 
	{
		{0x5601, 1, 1, 1},
		{0x3401, 2, 6, 0},
		{0x1801, 3, 9, 0},
		{0x0ac1, 4, 12, 0},
		{0x0521, 5, 29, 0},
		{0x0221, 38, 33, 0},
		{0x5601, 7, 6, 1},
		{0x5401, 8, 14, 0},
		{0x4801, 9, 14, 0},
		{0x3801, 10, 14, 0},
		{0x3001, 11, 17, 0},
		{0x2401, 12, 18, 0},
		{0x1c01, 13, 20, 0},
		{0x1601, 29, 21, 0},
		{0x5601, 15, 14, 1},
		{0x5401, 16, 14, 0},
		{0x5101, 17, 15, 0},
		{0x4801, 18, 16, 0},
		{0x3801, 19, 17, 0},
		{0x3401, 20, 18, 0},
		{0x3001, 21, 19, 0},
		{0x2801, 22, 19, 0},
		{0x2401, 23, 20, 0},
		{0x2201, 24, 21, 0},
		{0x1c01, 25, 22, 0},
		{0x1801, 26, 23, 0},
		{0x1601, 27, 24, 0},
		{0x1401, 28, 25, 0},
		{0x1201, 29, 26, 0},
		{0x1101, 30, 27, 0},
		{0x0ac1, 31, 28, 0},
		{0x09c1, 32, 29, 0},
		{0x08a1, 33, 30, 0},
		{0x0521, 34, 31, 0},
		{0x0441, 35, 32, 0},
		{0x02a1, 36, 33, 0},
		{0x0221, 37, 34, 0},
		{0x0141, 38, 35, 0},
		{0x0111, 39, 36, 0},
		{0x0085, 40, 37, 0},
		{0x0049, 41, 38, 0},
		{0x0025, 42, 39, 0},
		{0x0015, 43, 40, 0},
		{0x0009, 44, 41, 0},
		{0x0005, 45, 42, 0},
		{0x0001, 45, 43, 0},
		{0x5601, 46, 46, 0}
	};
	
	static	int	ctx_index[MQC_NUMCTXS];
	static	int	ctx_mps[MQC_NUMCTXS];
	static	int	C;
	static	unsigned int	A;
	static	unsigned int	CT;
	static	int	cur_pt;
	static	int	cur_cblk;
	static	unsigned char last;

	int	i;
	int	tempC;
	int	index;
	unsigned int	qe;
	int	val;

	switch(mode)
	{
		case	CTRL_INIT:
			A	= 0x08000;
			C	= 0;
			cur_pt	= -1;
			CT	= 12;
			last	= last_data;
			if(last == 0xFF)
			{
				CT	= 13;
			}
			cur_cblk	= no_cblk;
			/* -- MQC_INIT_CONTX -- */
			for(i =0; i <MQC_NUMCTXS; i++)
			{
				ctx_mps[i]	= 0;
				if (i ==0)
				{
					ctx_index[i]	= 4;
				}
				else if (i ==17)
				{
					ctx_index[i]	= 3;
				}
				else if (i ==18)
				{
					ctx_index[i]	= 46;
				}
				else
				{
					ctx_index[i]	= 0;
				}
			}
			/* ------------------- */
			return 0;
		case	CTRL_RESTART:
			A	= 0x08000;
			C	= 0;
			CT	= 12;
			cur_pt --;
			last	= cblk_in_buffer(CTRL_READ, no_comp, cur_cblk, cur_pt, 0);
			if(last == 0xFF)
			{
				CT	= 13;
			}
			cur_cblk	= no_cblk;
			/* -- MQC_INIT_CONTX -- */
			for(i =0; i <MQC_NUMCTXS; i++)
			{
				ctx_mps[i]	= 0;
				if (i ==0)
				{
					ctx_index[i]	= 4;
				}
				else if (i ==17)
				{
					ctx_index[i]	= 3;
				}
				else if (i ==18)
				{
					ctx_index[i]	= 46;
				}
				else
				{
					ctx_index[i]	= 0;
				}
			}	
			return 0;

		case	CTRL_READ:
			return cblk_in_buffer(CTRL_READ, no_comp, cur_cblk, cur_pt, 0);
		
		case	CTRL_READ_PT:
			return cur_pt;

		case	CTRL_ENCODE:
			index	= ctx_index[cx];
			qe	= mqc_states[index][MQC_STATE_QE];
			A	-= qe;
			/* -- MQC_CODE_MPS -- */
			if(ctx_mps[cx] ==d)
			{
				if(A <0x08000)
				{
					if(A <qe)
					{
						A	= qe;
					}
					else
					{
						C	+= qe;
					}

					ctx_index[cx]	= mqc_states[index][MQC_STATE_NMPS];

					do
					{
						A	<<= 1;
						C	<<= 1;
						CT--;
	
						/* Byte Out */
						if(CT ==0)
						{
							val	= (cur_pt!=-1)? cblk_in_buffer(CTRL_READ, no_comp, cur_cblk, cur_pt, 0): last;
							if(val ==0xFF)
							{
								cur_pt++;
								cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>20)&0xFF);
								C	&= 0x000FFFFF;
								CT	= 7;
							}
							else
							{
								if(C <0x08000000)
								{
									cur_pt++;
									cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>19)&0xFF);
									C	&= 0x0007FFFF;
									CT	= 8;
								}
								else
								{
									val	= (cur_pt!=-1)? cblk_in_buffer(CTRL_READ, no_comp, cur_cblk, cur_pt, 0): last;
									val += 1;
									cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, val&0xFF);
									if(val ==0xFF)
									{
										C	&= 0x07FFFFFF;
										cur_pt++;
										cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>20)&0xFF);
										C	&= 0x000FFFFF;
										CT	= 7;
									}
									else
									{
										cur_pt++;
										cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>19)&0xFF);
										C	&= 0x0007FFFF;
										CT	= 8;
									}
								}
							}
						}
					}while(A <0x08000);
				} 
				else
				{
					C	+= qe;
				}
			} 

			/* -- MQC_CODE_LPS -- */
			else
			{
				if(A >=qe)
				{
					A	= qe;
				}
				else
				{
					C	+= qe;
				}

				if(mqc_states[index][MQC_STATE_SWITCH] == 1)
				{
					ctx_mps[cx]	= 1-ctx_mps[cx];
				}

				ctx_index[cx]	= mqc_states[index][MQC_STATE_NLPS];
			
				do
				{
					A	<<= 1;
					C	<<= 1;
					CT--;
					/* Byte Out */
					if(CT ==0)
					{
						val	= (cur_pt!=-1)? cblk_in_buffer(CTRL_READ, no_comp, cur_cblk, cur_pt, 0) : last;
						if(val ==0xFF)
						{
							cur_pt++;
							cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>20)&0xFF);
							C	&= 0x000FFFFF;
							CT	= 7;
						}
						else
						{
							if(C <0x08000000)
							{
								cur_pt++;
								cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>19)&0xFF);
								C	&= 0x0007FFFF;
								CT	= 8;
							}
							else
							{
								val	= (cur_pt!=-1)?cblk_in_buffer(CTRL_READ, no_comp, cur_cblk, cur_pt, 0):last;
								val += 1;
								cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, val&0xFF);
								if(val ==0xFF)
								{
									C	&= 0x07FFFFFF;
									cur_pt++;
									cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>20)&0xFF);
									C	&= 0x000FFFFF;
									CT	= 7;
								}
								else
								{
									cur_pt++;
									cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>19)&0xFF);
									C	&= 0x0007FFFF;
									CT	= 8;
								}
							}
						}
					}
				}while(A <0x08000);
			}
			
			
			/* ------------------- */
			break;

		case	CTRL_FLUSH:
			tempC	= C + A;

			C	|= 0x0000FFFF;
			if(C >=tempC)
			{
				C	-= 0x00008000;
			}

			C	<<= CT;

			/* -- MQC_BYTEOUT -- */
			val	= (cur_pt!=-1)?cblk_in_buffer(CTRL_READ, no_comp, cur_cblk, cur_pt, 0):last;
			if(val ==0xFF)
			{
				cur_pt++;
				cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>20)&0xFF);
				C	&= 0x000FFFFF;
				CT	= 7;
			}
			else
			{
				if(C <0x08000000)
				{
					cur_pt++;
					cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>19)&0xFF);
					C	&= 0x0007FFFF;
					CT	= 8;
				}
				else
				{
					val	= (cur_pt!=-1)?cblk_in_buffer(CTRL_READ, no_comp, cur_cblk, cur_pt, 0): last;
					val += 1;
					cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, val&0xFF);
					if(val ==0xFF)
					{
						C	&= 0x07FFFFFF;
						cur_pt++;
						cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>20)&0xFF);
						C	&= 0x000FFFFF;
						CT	= 7;
					}
					else
					{
						cur_pt++;
						cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>19)&0xFF);
						C	&= 0x0007FFFF;
						CT	= 8;
					}
				}
			}
			/* ------------------- */
			C	<<= CT;

			/* -- MQC_BYTEOUT -- */
			val	= (cur_pt !=-1)?cblk_in_buffer(CTRL_READ, no_comp, cur_cblk, cur_pt, 0): last;

			if(val ==0xFF)
			{
				cur_pt++;
				cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>20)&0xFF);
				C	&= 0x000FFFFF;
				CT	= 7;
			}
			else
			{
				if(C <0x08000000)
				{
					cur_pt++;
					cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>19)&0xFF);
					C	&= 0x0007FFFF;
					CT	= 8;
				}
				else
				{
					val	= (cur_pt!=-1)?cblk_in_buffer(CTRL_READ, no_comp, cur_cblk, cur_pt, 0): last;
					val += 1;
					cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, val&0xFF);
					if(val ==0xFF)
					{
						C	&= 0x07FFFFFF;
						cur_pt++;
						cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>20)&0xFF);
						C	&= 0x000FFFFF;
						CT	= 7;
					}
					else
					{
						cur_pt++;
						cblk_in_buffer(CTRL_WRITE, no_comp, cur_cblk, cur_pt, (C >>19)&0xFF);
						C	&= 0x7FFFF;
						CT	= 8;
					}
				}
			}
			/* ------------------- */
			if (cblk_in_buffer(CTRL_READ, no_comp, cur_cblk, cur_pt, 0) != 0xFF)
			{
				cur_pt++;
			}
			break;
	}
	return 0;
}


/* ----------------------------------------------------------------------- */
#if 1
void	t1_zeroCoding(int vsc, int m, int n, int no_bit_plane, int orient, int offset_x, int offset_y, int no_comp, int tile_w, int cblk_w, int cblk_h)
{
	int		flag,
			value;
	int		i	= (m > 0) ? (m - 1) : 0;
	int		j	= (n > 0) ? (n - 1) : 0;
	int		H	= 0,
			V	= 0,
			D	= 0;
	int		tmpH	= 0,
			HplusV	= 0;

	int		cx	= 0,
			decision	= 0;
	for( ; (i <= (m + 1)) && (i < cblk_h); i++)
	{
		flag	= (vsc &&(m%4==3)&&(i==m+1))? 0 :ctrl_cblk_flags(CTRL_READ, n, i, 0, cblk_w);
		if(i!=m)
			V		+= ((flag & T1_SIG) != 0);
	}
	for( ; (j <= (n + 1)) && (j < cblk_w); j++)
	{
		flag	= ctrl_cblk_flags(CTRL_READ, j, m, 0, cblk_w);
		if(j!=n)
			H	+= ((flag & T1_SIG) != 0);
	}

	i	= (m > 0) ? (m - 1) : 0;
	for( ; (i <= (m + 1)) && (i < cblk_h); i++)
	{
		j	= (n > 0) ? (n - 1) : 0;
		for( ; (j <= (n + 1)) && (j < cblk_w); j++)
		{
			if(i != m && j != n)
			{
				flag	= (vsc&&(m%4==3)&&(i==m+1))? 0 : ctrl_cblk_flags(CTRL_READ, j, i, 0, cblk_w);
				D		+= ((flag & T1_SIG) != 0);
			}
		}
	}

	switch(orient)
	{
		case	1:
			tmpH = H;
			H = V;
			V = tmpH;
		case	0:
		case	2:
			if(H == 0)
			{
				if(V == 0)
				{
					if (D == 0)
						cx	= 0;
					else if (D == 1)
						cx	= 1;
					else
						cx	= 2;
				}
				else if(V == 1)
					cx	= 3;
				else
					cx	= 4;
			}
			else if(H == 1)
			{
				if(V == 0)
				{
					if(D == 0)
						cx	= 5;
					else
						cx	= 6;
				}
				else
					cx	= 7;
			}
			else
				cx	= 8;
			break;

		case	3:
			HplusV	= H + V;
			if(D == 0)
			{
				if(HplusV == 0)
					cx	= 0;
				else if(HplusV == 1)
					cx	= 1;
				else
					cx	= 2;
			}
			else if(D == 1)
			{
				if(HplusV == 0)
					cx	= 3;
				else if(HplusV == 1)
					cx	= 4;
				else
					cx	= 5;
			}
			else if(D == 2)
			{
				if(HplusV == 0)
					cx	= 6;
				else
					cx	= 7;
			}
			else
				cx	= 8;

			break;
	}
	value	= tile_in_buffer(CTRL_READ, no_comp, offset_x + n, offset_y + m, 0, tile_w);
	decision	= (int_abs(value) >> (no_bit_plane + 0)) & 0x01;

	////printf("CX : %d\t D : %d\t in ZeroCoding\n", cx, decision);
	tier1_bac(CTRL_ENCODE, cx, decision, no_comp, 0, 0);

	flag	= ctrl_cblk_flags(CTRL_READ, n, m, 0, cblk_w);
	flag	|= T1_VISIT;
	ctrl_cblk_flags(CTRL_WRITE, n, m, flag, cblk_w);
}


int t1_signCoding(int vsc, int m, int n, int offset_x, int offset_y, int no_comp, int tile_w, int cblk_w, int cblk_h)
{
	int flag;
	int H	= 0,
		V	= 0;
	int sigma,
		khai,
		khai2;

	int i,
		j;
	int cx	= 0;

	//i	= (m > 0) ? (m - 1) : (m + 1);
	i	= (m > 0) ? (m - 1) : 0;
	for( ; (i <=( m + 1)) && (i < cblk_h); i ++)
	{
		sigma	= ((ctrl_cblk_flags(CTRL_READ, n, i, 0, cblk_w) & T1_SIG) == T1_SIG) ? 1 : 0;
		sigma	= (vsc&&(m%4==3)&&(i==m+1))? 0 : sigma;
		khai	= (tile_in_buffer(CTRL_READ, no_comp, offset_x+n, offset_y+i, 0, tile_w) < 0) ? 1 : 0;
		khai	= (vsc&&(m%4==3)&&(i==m+1))? 0 : khai;
		if(i!=m)
			V		+= (sigma * (1 - 2 * khai));
	}

	V	= int_max(-1, V);
	V	= int_min(1, V);
	//j	= (n > 0) ? (n - 1) : (n + 1);
	j	= (n > 0) ? (n - 1) : 0;
	
	for( ; (j <= (n + 1)) && (j < cblk_w); j ++)
	{
		sigma	= ((ctrl_cblk_flags(CTRL_READ, j, m, 0, cblk_w) & T1_SIG) == T1_SIG) ? 1 : 0;
		khai	= (tile_in_buffer(CTRL_READ, no_comp, offset_x+j, offset_y+m, 0, tile_w) < 0) ? 1 : 0;
		if(j!=n)
			H		+= (sigma * (1 - 2 * khai));
	}

	H	= int_max(-1, H);
	H	= int_min(1, H);

	if(H == 1)
	{
		khai2 = 0;
		if(V == 1)
			cx	= 13;
		else if(V == 0)
			cx	= 12;
		else
			cx	= 11;
	}
	else if(H == 0)
	{
		khai2	= 0;
		if(V == 1)
			cx	= 10;
		else if(V == 0)
			cx	= 9;
		else
		{
			cx	= 10;
			khai2	= 1;
		}
	}
	else
	{
		khai2	= 1;

		if (V == 1)
			cx	= 11;
		else if (V == 0)
			cx	= 12;
		else
			cx	= 13;
	}

	khai	= (tile_in_buffer(CTRL_READ, no_comp, offset_x+n, offset_y+m, 0, tile_w) < 0) ? 1 : 0;

	tier1_bac(CTRL_ENCODE, cx, khai^khai2, no_comp, 0, 0);

	////printf("CX : %d\t D : %d\t in SignCoding\n", cx, khai^khai2);
	flag	= ctrl_cblk_flags(CTRL_READ, n, m, 0, cblk_w);
	flag	|= T1_SIG;
	ctrl_cblk_flags(CTRL_WRITE, n, m, flag, cblk_w);
	return	0;
}


int	t1_magRefCoding(int vsc, int m, int n, int no_bit_plane, int offset_x, int offset_y, int no_comp, int tile_w, int cblk_w, int cblk_h)
{
	int		flag,
			value;
	int		i,
			j,
			chk,
			decision;
	int		cx	= 0;

	flag	= ctrl_cblk_flags(CTRL_READ, n, m, 0, cblk_w);
	if(flag & T1_REFINE)
		cx	= 16;

	else
	{
		chk	= 0;
		i	= (m > 0) ? (m - 1) : 0;
		for( ; (i <= (m + 1)) && (i < cblk_h); i++)
		{
			j	= (n > 0) ? (n - 1) : 0;
			for( ; (j <=(n + 1)) && (j < cblk_w); j++)
			{
				if(i != m || j != n)
				{
					flag = (vsc&&(m%4==3)&&(i==m+1))? 0 : ctrl_cblk_flags(CTRL_READ, j, i, 0, cblk_w);
					chk += (flag & T1_SIG);
				}
			}
		}

		if(chk == 0)
			cx	= 14;
		else
			cx	= 15;
	}

	value		= tile_in_buffer(CTRL_READ, no_comp, offset_x + n, offset_y + m, 0, tile_w);
	decision	= (int_abs(value) >> (no_bit_plane + 0)) & 0x01;

	tier1_bac(CTRL_ENCODE, cx, decision, no_comp, 0, 0);

	////printf("CX : %d\t D : %d\t in MagRefCoding\n", cx, decision);
	flag		= ctrl_cblk_flags(CTRL_READ, n, m, 0, cblk_w);
	flag		|= T1_REFINE;
	ctrl_cblk_flags(CTRL_WRITE, n, m, flag, cblk_w);
	return	0;
}


int	t1_runLenCoding(int vsc, int m, int n, int no_bit_plane, int offset_x, int offset_y, int no_comp, int tile_w)
{
	int		runlen,
			value;
	int		cx	= 0;

	for(runlen = 0; runlen < 4 && ((m+runlen)<CBLK_HEIGHT); runlen++)
	{
		value	= tile_in_buffer(CTRL_READ, no_comp, offset_x + n, offset_y + (m + runlen), 0, tile_w);
		if((int_abs(value) >> (no_bit_plane + 0)) & 0x01)
			break;
	}
	cx	= 17;
	tier1_bac(CTRL_ENCODE, cx, (runlen != 4), no_comp, 0, 0);
	if(runlen != 4)
	{
		cx = 18;
		tier1_bac(CTRL_ENCODE, cx, (runlen >> 1), no_comp, 0,0);
	
		tier1_bac(CTRL_ENCODE, cx, (runlen & 1), no_comp, 0,0);
	}

	return runlen;
}


int	t1_enc_spp(int vsc, int no_bit_plane, int orient, int offset_x, int offset_y, int no_comp, int tile_w, int cblk_w, int cblk_h)
{
	int		value;
	int		m,
			m2,
			n;
	int		chk,
			chk2;
	int		i,
			j;
	/* row m, column n */
	for(m = 0; m < cblk_h; m += 4)
	{
		for(n = 0; n < cblk_w; n++)
		{
			for(m2 = m; m2 < m + 4 && m2 < cblk_h; m2++)
			{
				chk		= (ctrl_cblk_flags(CTRL_READ, n, m2, 0, cblk_w) & T1_SIG);
				chk2	= 0;

				i		= (m2 > 0) ? (m2 - 1) : 0;
				for( ; i <= m2 + 1 && i < cblk_h; i++)
				{
					j	= (n > 0) ? (n - 1) : 0;
					for( ; j <= n + 1 && j < cblk_w; j++)
					{
						if(i != m2 || j != n)
							chk2	+= (vsc&&(m2%4==3)&&(i==m2+1))? 0 :(ctrl_cblk_flags(CTRL_READ, j, i, 0, cblk_w) & T1_SIG);
					}
				}
				/* sigma[m2][n] == 0 && in a preferred neighborhood */
				if(chk == 0 && chk2 != 0)
				{
					/* Zero Coding */
					t1_zeroCoding(vsc, m2, n, no_bit_plane, orient, offset_x, offset_y, no_comp, tile_w, cblk_w, cblk_h);

					value = tile_in_buffer(CTRL_READ, no_comp, offset_x + n, offset_y + m2, 0, tile_w);
					if ((int_abs(value) >> (no_bit_plane + 0)) & 0x01) {
						t1_signCoding(vsc, m2, n, offset_x, offset_y, no_comp, tile_w, cblk_w, cblk_h);
					}
				} /* if (chk */
			}
		} /* for (n = */
	}
	return 0;
}


int t1_enc_mrp(int vsc, int no_bit_plane, int offset_x, int offset_y, int no_comp, int tile_w, int cblk_w, int cblk_h)
{
	int		m,
			m2,
			n;

	int		chk,
			value;
	/* row m, column n */
	for(m = 0; m < cblk_h; m += 4)
	{
		for(n = 0; n < cblk_w; n++)
		{
			for(m2 = m; (m2 < (m + 4)) && (m2 < cblk_h); m2++)
			{
				chk	= ctrl_cblk_flags(CTRL_READ, n, m2, 0, cblk_w) & (T1_SIG | T1_VISIT);
				/* sigma[m2][n] == 1 && etha[m2][n] == 0 */
				if (chk == T1_SIG)
				{
					/* Magnitude Refinement Coding */
					value	= tile_in_buffer(CTRL_READ, no_comp, offset_x + n, offset_y + m2, 0, tile_w);
					t1_magRefCoding(vsc, m2, n, no_bit_plane, offset_x, offset_y, no_comp, tile_w, cblk_w, cblk_h);
				}
			}
		} /* for (n = */
	} /* for (m = */
	return 0;
}


int t1_enc_cup(int vsc, int no_bit_plane, int orient, int offset_x, int offset_y, int no_comp, int tile_w, int cblk_w, int cblk_h)
{
	int		m,
			m2,
			n;
	int		chk,
			chk2,
			runlen;
	int		i,
			j;
	int		flag,
			value;
	/* row m, column n */
	for(m = 0; m < cblk_h; m += 4)
	{
		for(n = 0; n < cblk_w; n++)
		{
			for(m2 = m; (m2 <(m + 4))&&(m2 < cblk_h); m2++)
			{
				chk = ctrl_cblk_flags(CTRL_READ, n, m2, 0, cblk_w) & (T1_SIG | T1_VISIT);
				/* sigma[m2][n] == 0 && etha[m2][n] == 0 */
				if(chk == 0)
				{
					/* Run-length coding should not be applied to
					 *		the last section with fewer than four rows in a scattern pattern. */
					if((m2 + 3 < cblk_h) && (m2 % 4 == 0))
					{
						chk2	= 0;
						i		= (m2 > 0) ? (m2 - 1) : 0;
						for( ; ((i <= m2 + 4)&&(i<cblk_h)&&(!vsc))||(vsc&&(i<m+4)&&(i<cblk_h)); i++)
						{
							j	= (n > 0) ? (n - 1) : 0;
							for( ; j <= n + 1 && j < cblk_w; j++)
							{
								chk2	+= (vsc&&(m2%4==3)&&(i==m2+1))? 0 : (ctrl_cblk_flags(CTRL_READ, j, i, 0, cblk_w) & T1_SIG);
							}
						}
					
						if(chk2 == 0)
						{	/* Run-Length Coding */
							runlen = t1_runLenCoding(vsc, m2, n, no_bit_plane, offset_x, offset_y, no_comp, tile_w);
							m2 += runlen;
						}
						else
						{	/* Zero Coding */
							t1_zeroCoding(vsc, m2, n, no_bit_plane, orient, offset_x, offset_y, no_comp, tile_w, cblk_w, cblk_h);
						}
					}
					else
					{	/* Zero Coding */
						t1_zeroCoding(vsc, m2, n, no_bit_plane, orient, offset_x, offset_y, no_comp, tile_w, cblk_w, cblk_h);
					}

					/* NOT four consecutive 0 bits! */
					if(m2 < m + 4)
					{
						/* Encoded bit has value 1.*/
						value	= tile_in_buffer(CTRL_READ, no_comp, offset_x + n, offset_y + m2, 0, tile_w);
						if ((int_abs(value) >> (no_bit_plane + 0)) & 0x01)
						{
							t1_signCoding(vsc, m2, n, offset_x, offset_y, no_comp, tile_w, cblk_w, cblk_h);
						}
					}
				}
			} /* for (m2 = */
		} /* for (n = */
	}

	/* All ethas should be zero */
	for(m = 0; m < cblk_h; m++)
	{
		for(n = 0; n < cblk_w; n++)
		{
			flag	= ctrl_cblk_flags(CTRL_READ, n, m, 0, cblk_w);
			if(flag & T1_VISIT)
			{
				flag	&= ~T1_VISIT;
				ctrl_cblk_flags(CTRL_WRITE, n, m, flag, cblk_w);
			}
		}
	}
	return 0;
}


/* -------------------------------------------------------------------------------------------- */
void	tier1_bpc(int vsc, int num_res, int no_res, int offset, int no_cblk, int subband,
			      int level, int offset_x, int offset_y, int no_comp, 
				  int tile_w, int cblk_w, int cblk_h)
{
	int temp = 0;
	int	i,
		j;
	int	max			=0,
		value		=0,
		cblkno		=0;
	int	num_coding_pass	=0,
		num_bit_plane	=0,
		no_bit_plane	=0,
		type			=0;
	int	len;
	int nmse		=0,
		pass_type	=0;
	int		band;
	int		step[10]	= {68900, 68478, 68478, 68061, 32688, 32688, 31670, 16565, 16565, 17041};
	
	static	unsigned char last_data[3] = {0x00, 0x00, 0x00};
	if(no_cblk==0)
	{
		last_data[0] = 0x00;
		last_data[1] = 0x00;
		last_data[2] = 0x00;
	}
	band	= (no_res==0)? subband : (no_res-1)*3 + subband;
	cblkno	= offset + no_cblk;	
	for(i =0; i <cblk_h; i++)
	{
		for(j =0; j <cblk_w; j++)
		{
			value	= tile_in_buffer(CTRL_READ, no_comp, offset_x +j, offset_y +i, 0, tile_w);
			value	= (fix_mul(value, step[band]));

			tile_in_buffer(CTRL_WRITE, no_comp, offset_x +j, offset_y +i, value, tile_w);
			max	= int_max(max, int_abs(value));
		}
	}
	
	num_bit_plane	= max? ((int_floorlog2(max) +1) ) : 0;
	////printf("numbitplane:%d\t", num_bit_plane);	
	ctrl_cblk_info(CBLK_NUM_BITPLANE, CTRL_WRITE, no_comp, cblkno, 0, num_bit_plane);
	tier1_bac(CTRL_INIT, 0, 0, no_comp, cblkno, last_data[no_comp]);
	no_bit_plane	= num_bit_plane	-1;

	type	= 2;
	len	=0;

	while(no_bit_plane>=0)
	{
		for(pass_type =type; pass_type <3; pass_type++)
		{
			switch(pass_type)
			{
				case 0:
					nmse	= t1_enc_spp(vsc, no_bit_plane, subband, offset_x, offset_y, no_comp, tile_w, cblk_w, cblk_h);
					break;
				case 1:
					nmse	= t1_enc_mrp(vsc, no_bit_plane, offset_x, offset_y, no_comp, tile_w, cblk_w, cblk_h);
					break;
				case 2:
					nmse	= t1_enc_cup(vsc, no_bit_plane, subband, offset_x, offset_y, no_comp, tile_w, cblk_w, cblk_h);
					break;
			}

			num_coding_pass++;
		}
		type =0;
		no_bit_plane--;
	}
	temp = num_coding_pass;
	// //printf("numpass:%d\n", num_coding_pass);	
	tier1_bac(CTRL_FLUSH, 0, 0, no_comp, cblkno, 0);
	last_data[no_comp]	= (tier1_bac(CTRL_READ, 0, 0, no_comp, cblkno, 0))&0x00FF;
	////printf("cblkno:%d\t", cblkno);
	ctrl_cblk_info(CBLK_TOT_PASS, CTRL_WRITE, no_comp, cblkno, 0, num_coding_pass);
	
	len	= tier1_bac(CTRL_READ_PT, 0, 0, no_comp, cblkno, 0);
	// //printf("len:%x\n", len);
	////printf("cblkno:%d\n", cblkno);
	ctrl_cblk_info(CBLK_LEN, CTRL_WRITE, no_comp, cblkno, 0, len);


}

/* -------------------------------------------------------------------------------------------- */
int		tier1(int vsc, int tile_w, int tile_h, int compno, int num_res)
{
	int	no_res,
		no_subband,
		no_cblk,
		level;

	int	x,
		y,
		w,
		h;
	int	num_cblk,
		num_subband;
	int	offset;
	int	cx0,
		cy0;

	int	cblk_w,
		cblk_h,
		band_w,
		band_h,
		ncw,
		nch;
	
		for(no_res =0; no_res <num_res; no_res++)
		//for(no_res =num_res; no_res >=0; no_res--)
		{
			level	= num_res -1 - no_res;
			if(no_res ==0)
			{
				band_w	= int_ceildivpow2(tile_w, level);
				band_h	= int_ceildivpow2(tile_w, level);
				
				cblk_w	= band_w;
				cblk_h	= band_h;
				////printf("cblk_w :%d\tcblk_h:%d\n", cblk_w, cblk_h);

				ncw	= 1;
				nch	= 1;
				num_cblk	= 1;
				num_subband	= 1;
				offset		= 0;
				
				
			}
			else
			{
				band_w	= int_ceildivpow2(tile_w, level+1);
				band_h	= int_ceildivpow2(tile_w, level+1);
				
				cblk_w	= band_w;
				cblk_h	= band_h;
				////printf("cblk_w :%d\tcblk_h:%d\n", cblk_w, cblk_h);
				
				num_subband	= 3;

				ncw	= 1;
				nch	= 1;
				num_cblk	= 1;
				offset		= 3*(no_res-1)+1;
				
			}	
			for(no_subband =0; no_subband <num_subband; no_subband++)
			{

				if(no_res!=0)
				{
					if(no_subband==0)
					{
						cx0 = band_w;
						cy0	= 0;
					}
					else if(no_subband==1)
					{
						cx0	= 0;
						cy0	= band_h;
					}
					else
					{
						cx0	= band_w;
						cy0	= band_h;
					}
				}
				else
				{
					cx0	=0;
					cy0 =0;
				}
				x	= cx0;
				y	= cy0;
				////printf("nores : %d\t x :%d\t y:%d\n", no_res, x, y);
				for(no_cblk =0; no_cblk <num_cblk; no_cblk++)
				{
					////printf("offset :%d\n", offset);
					for(h =0; (h <cblk_h) && ((cy0+h)<tile_h); h++)
					{
						for(w =0; (w <cblk_w) && ((cx0+w)<tile_w); w++)
						{
							ctrl_cblk_flags(CTRL_WRITE, w, h, 0, cblk_w);
						}
					}
					if(no_res ==0)
					{
						tier1_bpc(vsc, num_res, no_res, offset ,no_cblk, no_subband, level, cx0, cy0, compno, tile_w, cblk_w, cblk_h);
					}
					else
					{
						tier1_bpc(vsc, num_res, no_res, offset ,no_cblk, no_subband+1, level, cx0, cy0, compno, tile_w, cblk_w, cblk_h);
					}

					cx0	+= cblk_w;
					if(((no_cblk+1)%nch)==0 && ((no_cblk+1)!=0))
					{
						cx0	= x;  
						cy0 += cblk_h;
					}
				}/*-- end of no_cblk --*/
				offset += num_cblk;
			}/*-- end of no_subband --*/
		}/*-- end of no_res --*/
	return 	1;
}
#endif
#endif
/* -------------------------------------------------------------------------- */

void	dc_level_shifting_color_compononent_transform(int tx0, int ty0, int tx1, int ty1, int imag_w, int imag_h, int num_comps, int no_comps)
{
	int	i,	j,	R0,	G0,	B0, T0;
	int	R1,	G1,	B1, T1;
	int	R2,	G2,	B2, T2;
	int	R3,	G3,	B3, T3;
	//int	buffer;
	int	addr;
	addr	= 0;

	for(i=0;i<(TILE_WIDTH>>1);i++)
	{
		for(j=0;j<(TILE_WIDTH>>1);j++)
		{
			
			R0	= image_src_buf[(tx0+ty0*imag_w+2*i*imag_w+2*j)*num_comps+0] -128;
			R1	= image_src_buf[(tx0+ty0*imag_w+2*i*imag_w+2*j+1)*num_comps+0] -128;
			R2	= image_src_buf[(tx0+ty0*imag_w+(2*i+1)*imag_w+2*j)*num_comps+0] -128;
			R3	= image_src_buf[(tx0+ty0*imag_w+(2*i+1)*imag_w+2*j+1)*num_comps+0] -128;
	
			if(num_comps>1)
			{
				G0	= image_src_buf[(tx0+ty0*imag_w+2*i*imag_w+2*j)*num_comps+1]-128;
				B0	= image_src_buf[(tx0+ty0*imag_w+2*i*imag_w+2*j)*num_comps+2]-128;
				
				G1	= image_src_buf[(tx0+ty0*imag_w+2*i*imag_w+2*j+1)*num_comps+1]-128;
				B1	= image_src_buf[(tx0+ty0*imag_w+2*i*imag_w+2*j+1)*num_comps+2]-128;

				G2	= image_src_buf[(tx0+ty0*imag_w+(2*i+1)*imag_w+2*j)*num_comps+1]-128;
				B2	= image_src_buf[(tx0+ty0*imag_w+(2*i+1)*imag_w+2*j)*num_comps+2]-128;

				G3	= image_src_buf[(tx0+ty0*imag_w+(2*i+1)*imag_w+2*j+1)*num_comps+1]-128;
				B3	= image_src_buf[(tx0+ty0*imag_w+(2*i+1)*imag_w+2*j+1)*num_comps+2]-128;
			
				switch(no_comps)
				{
					case 0 :
							
								T0	= fix_mul(R0, 2449) + fix_mul(G0, 4809) + fix_mul(B0, 934);
								T1	= fix_mul(R1, 2449) + fix_mul(G1, 4809) + fix_mul(B1, 934);
								T2	= fix_mul(R2, 2449) + fix_mul(G2, 4809) + fix_mul(B2, 934);
								T3	= fix_mul(R3, 2449) + fix_mul(G3, 4809) + fix_mul(B3, 934);
						break;
					case 1:
						
								T0	= fix_mul(B0, 4096) - fix_mul(R0, 1382) - fix_mul(G0, 2714); 
								T1	= fix_mul(B1, 4096) - fix_mul(R1, 1382) - fix_mul(G1, 2714); 
								T2	= fix_mul(B2, 4096) - fix_mul(R2, 1382) - fix_mul(G2, 2714); 
								T3	= fix_mul(B3, 4096) - fix_mul(R3, 1382) - fix_mul(G3, 2714); 
						break;
					case 2:
								T0	= fix_mul(R0, 4096) - fix_mul(G0, 3430) - fix_mul(B0, 666);
								T1	= fix_mul(R1, 4096) - fix_mul(G1, 3430) - fix_mul(B0, 666);
								T2	= fix_mul(R2, 4096) - fix_mul(G2, 3430) - fix_mul(B0, 666);
								T3	= fix_mul(R3, 4096) - fix_mul(G3, 3430) - fix_mul(B0, 666);
						break;
				}
			}
			else
			{
				T0	= R0;
				T1	= R1;
				T2	= R2;
				T3	= R3;
			}
			
			
			tile_in_buffer(CTRL_WRITE, no_comps, 2*j, 2*i, T0, TILE_WIDTH);  
			tile_in_buffer(CTRL_WRITE, no_comps, 2*j+1, 2*i, T1, TILE_WIDTH);  
			tile_in_buffer(CTRL_WRITE, no_comps, 2*j, 2*i+1, T2, TILE_WIDTH);  
			tile_in_buffer(CTRL_WRITE, no_comps, 2*j+1, 2*i+1, T3, TILE_WIDTH);  
			
			//buffer	= ((T0&0x0FF)<<24) | ((T1&0x0FF)<<16) | ((T2&0x0FF)<<8) | (T3&0x0FF);
			//tile_in[addr]	= buffer;
		
			addr++;
		}
		
	}
	
}
/* ---------------------------------------------------------------------- */
#if 1
void	dwt97(int no_comps, int num_res, int tile_w, int tile_h)
{
	int	l_buf[TILE_WIDTH];
	int	i,
		j,
		k;
	int	cur_w,
		cur_h;
	int	nxt_w	= tile_w,
		nxt_h	= tile_h;
	int	y_ext;
	int	lowFlen,
		highFlen;
	for(i =0; i <num_res -1; i++)
	{
		cur_w	= nxt_w;
		cur_h	= nxt_h;
		nxt_w	= cur_w>>1;
		nxt_h	= cur_h>>1;
		
		/* ------------------------
		 * 		Column DWT
		 * ---------------------- */
		lowFlen	= nxt_h;
		highFlen	= cur_h - nxt_h;
		for(j =0; j <cur_w; j++)
		{
			for(k =0; k <cur_h; k++)
			{
				l_buf[k]	= (tile_in_buffer(CTRL_READ, no_comps, j, k, 0, tile_w));
			}	
			y_ext	= l_buf[1] - fix_mul(l_buf[2] + l_buf[0], 12993);
			for(k =0; k <(highFlen-1); k++)
			{
				l_buf[2*k+1]	= l_buf[2*k +1] - fix_mul((l_buf[2*k] + l_buf[2*k +2]),12993);
			}
		
			l_buf[2*k +1]	= l_buf[2*k +1] - fix_mul(2*l_buf[2*k], 12993);
			l_buf[0]	= l_buf[0] - fix_mul(y_ext + l_buf[1], 434);
			for(k =1; k <lowFlen; k++)
			{
				l_buf[2*k]	= l_buf[2*k] - fix_mul((l_buf[2*k -1] + l_buf[2*k +1]), 434);
			}
			
			y_ext	= l_buf[1] + fix_mul(l_buf[2] + l_buf[0], 7233);
			for(k =0; k <(highFlen-1); k++)
			{
				l_buf[2*k+1]	= l_buf[2*k +1] + fix_mul((l_buf[2*k] + l_buf[2*k +2]),7233);
			}
		
			l_buf[2*k +1]	= l_buf[2*k +1] + fix_mul(2*l_buf[2*k], 7233);
			l_buf[0]	= l_buf[0] + fix_mul(y_ext + l_buf[1], 3633);
			
			for(k =1; k <lowFlen; k++)
			{
				l_buf[2*k]	= l_buf[2*k] + fix_mul((l_buf[2*k -1] + l_buf[2*k +1]), 3633);
			}
			
			for(k =0; k <highFlen; k++)
			{
				l_buf[2*k+1]	= fix_mul(l_buf[2*k+1], 5038);
			}
			for(k =0; k <lowFlen; k++)
			{
				l_buf[2*k]	= fix_mul(l_buf[2*k], 6659);
			}

			for(k =0; k <lowFlen; k++)
			{
				tile_in_buffer(CTRL_WRITE, no_comps, j, k, l_buf[2*k], tile_w);
			}
			for(k =0; k <highFlen; k++)
			{
				tile_in_buffer(CTRL_WRITE, no_comps, j, lowFlen +k, l_buf[2*k +1], tile_w);
			}
			
		
		}
		/* ------------------------
		 * 		Row DWT
		 * ---------------------- */
		lowFlen	= nxt_w;
		highFlen	= cur_w - nxt_w;
		for(j =0; j <cur_h; j++)
		{
			for(k =0; k <cur_w; k++)
			{
				l_buf[k]	= tile_in_buffer(CTRL_READ, no_comps, k, j, 0, tile_w);	
			}
			y_ext	= l_buf[1] - fix_mul(l_buf[2] + l_buf[0], 12993);
			for(k =0; k <(highFlen-1); k++)
			{
				l_buf[2*k+1]	= l_buf[2*k +1] - fix_mul((l_buf[2*k] + l_buf[2*k +2]),12993);
			}

			l_buf[2*k +1]	= l_buf[2*k +1] - fix_mul(2*l_buf[2*k], 12993);
			l_buf[0]	= l_buf[0] - fix_mul(y_ext + l_buf[1], 434);
		
			for(k =1; k <lowFlen; k++)
			{
				l_buf[2*k]	= l_buf[2*k] - fix_mul((l_buf[2*k -1] + l_buf[2*k +1]), 434);
			}
			
			y_ext	= l_buf[1] + fix_mul(l_buf[2] + l_buf[0], 7233);
			for(k =0; k <(highFlen-1); k++)
			{
				l_buf[2*k+1]	= l_buf[2*k +1] + fix_mul((l_buf[2*k] + l_buf[2*k +2]),7233);
			}
		
			l_buf[2*k +1]	= l_buf[2*k +1] + fix_mul(2*l_buf[2*k], 7233);
			l_buf[0]	= l_buf[0] + fix_mul(y_ext + l_buf[1], 3633);
			
			for(k =1; k <lowFlen; k++)
			{
				l_buf[2*k]	= l_buf[2*k] + fix_mul((l_buf[2*k -1] + l_buf[2*k +1]), 3633);
			}
			
			for(k =0; k <highFlen; k++)
			{
				l_buf[2*k+1]	= fix_mul(l_buf[2*k+1], 5038);
			}
			for(k =0; k <lowFlen; k++)
			{
				l_buf[2*k]	= fix_mul(l_buf[2*k], 6659);
			}

			for(k =0; k <lowFlen; k++)
			{
				tile_in_buffer(CTRL_WRITE, no_comps, k, j, l_buf[2*k], tile_w);
			}
			for(k =0; k <highFlen; k++)
			{
				tile_in_buffer(CTRL_WRITE, no_comps, lowFlen+k, j, l_buf[2*k +1], tile_w);
			}
			
		}

	}
	DONEDWT=1;
}

#endif
/* ----------------------------------------------------------------------- */
#if 1

void	tgt_inclusionInfo(int code)
{
	int	i	= 1;
	int	n	= code;

	while(n >>=1)
		i	= (i<<1);
	while(i)
	{
	       	if(i &code)
				tile_out_buffer(CTRL_WRITE_BIT, 1, 1);
			else 
				tile_out_buffer(CTRL_WRITE_BIT, 0, 1);

		i	= (i>>1);
	}
}

/* ----------------------------------------------------------------------- */

void	tgt_zeroBitPlane(unsigned int code)
{
	unsigned int	i	= 1;
	unsigned int	n	= code >>1;

	while(n >>=1)
		i	= (i<<1);
	while(i)
	{
	       	if(i &code)
				tile_out_buffer(CTRL_WRITE_BIT, 1, 1);
			else 
				tile_out_buffer(CTRL_WRITE_BIT, 0, 1);

		i	= (i>>1);
	}
}

/* ----------------------------------------------------------------------- */

int	tgt_encode(int mode, int num_cblks_hor_in_band, int num_cblks_ver_in_band)
{
	static	unsigned int	tgt_data[MAX_TGT_LEVEL][10];
	static	unsigned int	tgt_code[MAX_TGT_LEVEL][10];
	int	i; 
	

	if(mode ==CTRL_WRITE)
	{
		tgt_data[0][num_cblks_hor_in_band]	= num_cblks_ver_in_band;
		return 1;
	}
	else if(mode ==CTRL_READ)
	{
		return tgt_code[0][num_cblks_hor_in_band];
	}

	tgt_code[0][0]	= 1;
	for(i = 0; i < tgt_data[0][0]; i++)
	{
		tgt_code[0][0]	= (tgt_code[0][0] << 1);
	}
	tgt_code[0][0]	= (tgt_code[0][0] << 1) | 0x01;
	return 0;
}
/* ----------------------------------------------------------------------- */


/* Code-block length indicator */
void	t2_putcommacode(int n)
{

	/* There are k 1's followed by 0 */
	while (--n >= 0)
	{
		tile_out_buffer(CTRL_WRITE_BIT, 1, 1);
	}

	tile_out_buffer(CTRL_WRITE_BIT, 0, 1);
}

/* Table 7.10 
 * - Codewords for the Number of Coding Passes */
void	t2_putnumpasses(int n)
{
	if(n ==1)
		tile_out_buffer(CTRL_WRITE_BIT, 0, 1);
	else if(n ==2)
		tile_out_buffer(CTRL_WRITE_BIT, 2, 2);
	else if(n <=5)
		tile_out_buffer(CTRL_WRITE_BIT, 0x000C | (n - 3), 4);
	else if(n <=36)
		tile_out_buffer(CTRL_WRITE_BIT, 0x01E0 | (n - 6), 9);
	else if(n <=164)
		tile_out_buffer(CTRL_WRITE_BIT, 0xFF80 | (n - 37), 16);
}


int	t2_encode_packet(int no_res, int num_res, int no_comp)
{
	int no_band, no_cblk;
	int i, increment;
	int num_bands, num_cblks;
	int LBlocks, len, num_pass, offset, offset2;
	int data;
	int total_len;
	int	level;
	int	band_w,
		band_h,
		cblk_w,
		cblk_h,
		ncw,
		nch;
	int	zrpck;
	int	temp;
	int start_pt = tile_out_buffer(CTRL_READ_PT, 0, 0);
	int expn,
		band;
	int expnarray97[10]	= {12, 12, 12, 12, 10, 10, 10, 10, 10, 10};
	/* SOP */
	tile_out_buffer(CTRL_WRITE, 0xFF, 1);
	tile_out_buffer(CTRL_WRITE, 0x91, 1);
	tile_out_buffer(CTRL_WRITE, 0x00, 1);
	tile_out_buffer(CTRL_WRITE, 0x04, 1);
	tile_out_buffer(CTRL_WRITE, (no_res % 65536) / 256, 1);
	tile_out_buffer(CTRL_WRITE, (no_res % 65536) % 256, 1);
	tile_out_buffer(CTRL_WRITE_BIT, 1, 1);
	/* It means non-zero packet. */
	level	= num_res -1 - no_res;
	if(no_res ==0)
	{
		band_w	= int_ceildivpow2(TILE_WIDTH, level);
		band_h	= int_ceildivpow2(TILE_WIDTH, level);
		cblk_w	= band_w;
		cblk_h	= band_h;
		
		ncw	=1;
		nch	=1;
		num_cblks	= 1;
		num_bands	= 1;
		offset		= 0;
		
		
	}
	else
	{
		band_w	= int_ceildivpow2(TILE_WIDTH, level+1);
		band_h	= int_ceildivpow2(TILE_WIDTH, level+1);
		cblk_w	= int_min(band_w, CBLK_WIDTH);
		cblk_h	= int_min(band_h, CBLK_HEIGHT);
		num_bands	= 3;
		ncw	=1;
		nch	= 1;
		num_cblks	= 1;
		offset		= 3*(no_res -1) + 1;
		
	}
	
	offset2 = offset;

	zrpck	=1;
	
	offset2	= offset;
	for(no_band = 0; (no_band < num_bands) ; no_band++)
	//for(no_band = 0; (no_band < num_bands)&&(zrpck==0) ; no_band++)
	{
		band	= (no_res==0)? 0 : (no_res-1)*3 + no_band+1;
		expn	= expnarray97[band]; 
		for(no_cblk = 0; no_cblk < num_cblks; no_cblk++)
		{
			num_pass	= ctrl_cblk_info(CBLK_TOT_PASS, CTRL_READ, no_comp, offset2 + no_cblk, 0, 0);
			data	= expn + 1;
			data	-= ctrl_cblk_info(CBLK_NUM_BITPLANE, CTRL_READ, no_comp, offset2 + no_cblk, 0, 0);
			////printf("nocblk:%d\t", no_cblk);
			if(num_pass==0)
			{
				tgt_encode(CTRL_WRITE, no_cblk, 15);
			}
			else
			{
				tgt_encode(CTRL_WRITE, no_cblk, data);
			}
		}
		if(no_res == 0)
		{
			tgt_encode(CTRL_ENCODE, 1, 1);
		}
		else
		{
			tgt_encode(CTRL_ENCODE, nch, ncw);
		}
		
		zrpck	= 1;

		for(no_cblk = 0; no_cblk < num_cblks; no_cblk++)
		{
			num_pass	= ctrl_cblk_info(CBLK_TOT_PASS, CTRL_READ, no_comp, offset2 + no_cblk, 0, 0);
			if(num_pass!=0)
			{
				zrpck	=0;
				break;
			}
		}
		if(zrpck==1)
		{
			tile_out_buffer(CTRL_WRITE_BIT, 0, 1); 
		}
		for(no_cblk = 0; (no_cblk < num_cblks)&&(zrpck==0); no_cblk++)
		{
			increment	= 0;

			LBlocks		= 3;
			len			= ctrl_cblk_info(CBLK_LEN, CTRL_READ, no_comp, offset2 + no_cblk, 0, 0);
			num_pass	= ctrl_cblk_info(CBLK_TOT_PASS, CTRL_READ, no_comp, offset2 + no_cblk, 0, 0);

			/* if cblk not included, go to the next cblk  */
			if (num_pass == 0)
			{
					tgt_inclusionInfo(1);
					continue;
			}
			tgt_inclusionInfo(1);
			temp	= tgt_encode(CTRL_READ, no_cblk, 0);
			////printf("bitplane:%d\n", temp);
			tgt_zeroBitPlane(temp);

			/* Number of coding passes for each code-block */
			t2_putnumpasses(num_pass);

			/* Code-block length indicator
			 * Add 1, because int_floorlog2(cblkLen) is 1 less than needed number of bits. */
			increment	= int_max(increment, 
					int_floorlog2(len) + 1 - (LBlocks + int_floorlog2(num_pass)));
			t2_putcommacode(increment);
			LBlocks		+= increment;
			tile_out_buffer(CTRL_WRITE_BIT, len, LBlocks + int_floorlog2(num_pass));
		}
		offset2			+= num_cblks;
	}
/* -- EPH -- */
	tile_out_buffer(CTRL_WRITE, 0xFF, 1);
	tile_out_buffer(CTRL_WRITE, 0x92, 1);
/* --------- */
	len			= 0;
	total_len	= 0;
	offset2		= offset;
	for(no_band = 0; no_band < num_bands; no_band++)
	{
		for(no_cblk = 0; no_cblk < num_cblks; no_cblk++)
		{
			len			= ctrl_cblk_info(CBLK_LEN, CTRL_READ, no_comp, offset2 + no_cblk, 0, 0);
			/* //printf("len in tier2:%x\t cblkno:%d\n", len, offset2+no_cblk);	 */
			num_pass	= ctrl_cblk_info(CBLK_TOT_PASS, CTRL_READ, no_comp, offset2 + no_cblk, 0, 0);
			/* if cblk not included, go to the next cblk  */
			if(num_pass == 0)
			{
				continue;
			}
			total_len	+= len;
			for(i = 0; i < len; i++)
			{
				////printf("cblk:%x\n", cblk_in_buffer(CTRL_READ, no_comp, offset2 + no_cblk, i, 0));
				tile_out_buffer(CTRL_WRITE, cblk_in_buffer(CTRL_READ, no_comp, offset2 + no_cblk, i, 0), 1);
			}
		}
		offset2	+= num_cblks;
	}

	return (tile_out_buffer(CTRL_READ_PT, 0, 0)  - start_pt);
}


int	tier2(int num_res, int no_comp)
{
	int		no_res;
	int		total_len	= 0;
	int		ret			= 0;
		for(no_res = 0; no_res < num_res; no_res++)
		{
			ret	= t2_encode_packet(no_res, num_res, no_comp);
		}

    return total_len;
}

/* ----------------------------------------------------------------------- */

void	jp2_write_mainheader_SIZ(int imag_w, int imag_h, int num_comps, int sgnd, int prec)
{
	int	len,
		start_pt;
	int	i;

	tile_out_buffer(CTRL_WRITE, JP2_MS_SIZ, 2);
	start_pt	= tile_out_buffer(CTRL_READ_PT, 0, 0);
	tile_out_buffer(CTRL_WRITE, 0, 2);
	tile_out_buffer(CTRL_WRITE, 0, 2);
	tile_out_buffer(CTRL_WRITE, imag_w, 4);
	tile_out_buffer(CTRL_WRITE, imag_h, 4);
	tile_out_buffer(CTRL_WRITE, 0, 4);
	tile_out_buffer(CTRL_WRITE, 0, 4);
	tile_out_buffer(CTRL_WRITE, TILE_WIDTH, 4);
	tile_out_buffer(CTRL_WRITE, TILE_HEIGHT, 4);
	tile_out_buffer(CTRL_WRITE, 0, 4);
	tile_out_buffer(CTRL_WRITE, 0, 4);
	tile_out_buffer(CTRL_WRITE, num_comps, 2);

	for(i =0; i <num_comps; i++)
	{
		tile_out_buffer(CTRL_WRITE, prec-1 + (sgnd <<7), 1);
		tile_out_buffer(CTRL_WRITE, 1, 1);
		tile_out_buffer(CTRL_WRITE, 1, 1);
	}
	len		= tile_out_buffer(CTRL_READ_PT, 0, 0) - start_pt;
	tile_out_buffer(CTRL_PUT, len, (2 <<24) | start_pt);

}

/* ----------------------------------------------------------------------- */

void	jp2_write_mainheader_COD(int num_res, int num_comps)
{
	int	len,
		start_pt;
	int	csty,
		prg,
		num_layers,
		mct;
	int	xcb,
		ycb;
	unsigned char cblksty;
	csty	= (0x02 | 0x04);
	prg	= 4;
	num_layers	= 1;
	xcb	= int_floorlog2(CBLK_WIDTH);
	ycb	= int_floorlog2(CBLK_HEIGHT);
	cblksty	= 0;

	if(num_comps ==1)
		mct	= 0;
	else
		mct	= 1;

	tile_out_buffer(CTRL_WRITE, JP2_MS_COD, 2);
	start_pt	= tile_out_buffer(CTRL_READ_PT, 0, 0);
	tile_out_buffer(CTRL_WRITE, 0, 2);
	tile_out_buffer(CTRL_WRITE, csty, 1);
	tile_out_buffer(CTRL_WRITE, prg, 1);
	tile_out_buffer(CTRL_WRITE, num_layers, 2);
	tile_out_buffer(CTRL_WRITE, mct, 1);
	tile_out_buffer(CTRL_WRITE, num_res -1, 1);
	tile_out_buffer(CTRL_WRITE, xcb -2, 1);
	tile_out_buffer(CTRL_WRITE, ycb -2, 1);
	tile_out_buffer(CTRL_WRITE, cblksty, 1);
	tile_out_buffer(CTRL_WRITE, 0, 1);//reversible : 1 (5,3) / 0 (9,7) 

	len	= tile_out_buffer(CTRL_READ_PT, 0, 0) - start_pt;
	tile_out_buffer(CTRL_PUT, len, (2<<24) | start_pt);

}

/* ----------------------------------------------------------------------- */
void	jp2_write_tileheader_COC(int num_res, int compno, int cblk_w, int cblk_h)
{
	int	len,
		start_pt;
	int	csty,
		xcb,
		ycb;
	unsigned char cblksty;
	cblksty	= 0;

	csty	= 0;
	//xcb	= int_floorlog2(cblk_w);
	//ycb = int_floorlog2(cblk_h);
	xcb	= int_floorlog2(CBLK_WIDTH);
	ycb	= int_floorlog2(CBLK_HEIGHT);

	tile_out_buffer(CTRL_WRITE, JP2_MS_COC,2);
	start_pt	= tile_out_buffer(CTRL_READ_PT, 0, 0);
	tile_out_buffer(CTRL_WRITE, 0, 2);
	tile_out_buffer(CTRL_WRITE, compno, 1);
	
	tile_out_buffer(CTRL_WRITE, csty, 1);
	tile_out_buffer(CTRL_WRITE, num_res-1,1);
	tile_out_buffer(CTRL_WRITE, xcb-2, 1);
	tile_out_buffer(CTRL_WRITE, ycb-2, 1);
	tile_out_buffer(CTRL_WRITE, cblksty, 1);
	tile_out_buffer(CTRL_WRITE, 0, 1);

	len	= tile_out_buffer(CTRL_READ_PT, 0, 0) - start_pt;
	tile_out_buffer(CTRL_PUT, len, (2<<24)|start_pt);
}
/* ----------------------------------------------------------------------- */

void	jp2_write_mainheader_QCD(int num_res)
{
	int	len,
		start_pt;
	int	no_subband, 
		num_subband, 
		expn, 
		mant;
	int	num_guard_bits	= 2;
	int	qntsty;
	int expnarray97[10]	= {12, 12, 12, 12, 10, 10, 10, 10, 10, 10};	
	int	mantarray[10]	= {1848, 1872, 1872, 1896, 5, 5, 71, 2003, 2003, 1890};

	qntsty	= 2;
	tile_out_buffer(CTRL_WRITE, JP2_MS_QCD, 2);
	start_pt	= tile_out_buffer(CTRL_READ_PT, 0, 0);
	tile_out_buffer(CTRL_WRITE, 0, 2);
	tile_out_buffer(CTRL_WRITE, qntsty + (num_guard_bits <<5), 1);
	num_subband	= num_res * 3 -2;


	for(no_subband =0; no_subband <num_subband; no_subband++)
	{
		expn	= expnarray97[no_subband];
		mant	= mantarray[no_subband];
		tile_out_buffer(CTRL_WRITE, (expn<<11) + mant, 2);
	}


	len	= tile_out_buffer(CTRL_READ_PT, 0, 0) - start_pt;
	tile_out_buffer(CTRL_PUT, len, (2 <<24) | start_pt);
}

/* ----------------------------------------------------------------------- */

void	jp2_write_tileheader_QCC(int num_res, int compno)
{
	int	len,
		start_pt;
	int	no_subband, 
		num_subband, 
		expn, 
		mant;
	int	num_guard_bits	= 2;
	int	qntsty;	
	int expnarray97[10]	= {12, 12, 12, 12, 10, 10, 10, 10, 10, 10};	
	int	mantarray[10]	= {1848, 1872, 1872, 1896, 5, 5, 71, 2003, 2003, 1890};

	qntsty	= 2;
	tile_out_buffer(CTRL_WRITE, JP2_MS_QCC, 2);
	start_pt	= tile_out_buffer(CTRL_READ_PT, 0, 0);
	tile_out_buffer(CTRL_WRITE, 0, 2);
	tile_out_buffer(CTRL_WRITE, compno, 1);
	tile_out_buffer(CTRL_WRITE, qntsty+(num_guard_bits <<5), 1);
	num_subband	= num_res * 3 -2;


	for(no_subband =0; no_subband <num_subband; no_subband++)
	{
		expn	= expnarray97[no_subband];
		mant	= mantarray[no_subband];
			tile_out_buffer(CTRL_WRITE, (expn<<11) + mant, 2);
	}

	len	= tile_out_buffer(CTRL_READ_PT, 0, 0) - start_pt;
	tile_out_buffer(CTRL_PUT, len, (2 <<24) | start_pt);
}

/* ----------------------------------------------------------------------- */

void	jp2_write_mainheader(int imag_w, int imag_h, int num_comps, int sgnd,
				int prec, int num_res)
{
	tile_out_buffer(CTRL_WRITE, JP2_MS_SOC, 2);
	jp2_write_mainheader_SIZ(imag_w, imag_h, num_comps, sgnd, prec);
	jp2_write_mainheader_COD(num_res, num_comps);
	jp2_write_mainheader_QCD(num_res);

}

/* ----------------------------------------------------------------------- */

int	jp2_write_tileheader_SOT(int no_tile)
{
	int	len,
		start_pt;
	int	SOT_start_pt;
	
	tile_out_buffer(CTRL_WRITE, JP2_MS_SOT, 2);
	start_pt	= tile_out_buffer(CTRL_READ_PT, 0, 0);
	tile_out_buffer(CTRL_WRITE, 0, 2);
	tile_out_buffer(CTRL_WRITE, no_tile, 2);
	SOT_start_pt	= tile_out_buffer(CTRL_READ_PT, 0, 0);
	tile_out_buffer(CTRL_WRITE, 0, 4);
	tile_out_buffer(CTRL_WRITE, 0, 1);
	tile_out_buffer(CTRL_WRITE, 1, 1);

	len	= tile_out_buffer(CTRL_READ_PT, 0, 0) - start_pt;
	tile_out_buffer(CTRL_PUT, len, (2 <<24) | start_pt);
	return	SOT_start_pt;
}

/* ----------------------------------------------------------------------- */

void	jp2_write_tileheader_SOD()
{
	tile_out_buffer(CTRL_WRITE, JP2_MS_SOD, 2);
}

/* --------------------------------------------------------------------------------*/

int	tile_header(int no_tile, int num_res, int num_comps, int tile_w, int tile_h)
{
	int	SOT_start_pt;
	int	compno ;
	int	cblk_w,
		cblk_h;
	SOT_start_pt	= jp2_write_tileheader_SOT(no_tile);
	cblk_w	= int_ceildivpow2(tile_w, (num_res-1));	
	cblk_h	= int_ceildivpow2(tile_h, (num_res-1));	
	
	if(num_comps>1)
	{
		for(compno=1; compno<num_comps; compno++)
		{
			jp2_write_tileheader_COC(num_res, compno, cblk_w, cblk_h);
			jp2_write_tileheader_QCC(num_res, compno );
		}
	}
	
	jp2_write_tileheader_SOD();
	return	SOT_start_pt;
}
/* ----------------------------------------------------------------------- */
#endif
#if 0
int read_file(char *str)
{
	
	int	res;
	int	max_length;
	
	FILE*	fp	= fopen(str, "rb");	
	if(fp	== NULL)
	{
		f//printf(stderr, "ERROR! Fail to open %s for reading...\n\n", str);
		return -1;
	}

	max_length = MAX_IMAGE_HEIGHT * MAX_IMAGE_WIDTH * MAX_IMAGE_COMPS;

	res = fread(image_src_buf, sizeof(unsigned char), max_length, fp);
	fclose(fp);
	
	return res;
}
#endif
/* -------------------------------------------------------------------------- */
int write_file(char* name, int length)
{
	
	int	res;
	
	FILE*	fp	= fopen(name, "wb");
	if(fp	== NULL)
	{
		//fprintf(stderr, "ERROR! Fail to open [%s] for writing!\n", name);
		return -1;
	}

	res = fwrite(image_dst_buf, sizeof(unsigned char), length, fp);
	fclose(fp);
	
	return res;
}
    
/* -------------------------------------------------------------------------- */

int main()
{
	/* raw image characteristic */
	//int	len,	
	int i;
	int j;
	int	tileno,	num_tile;
	int	compno;
	int	offset ;
	int	num_tile_hor,	num_tile_ver;
	int	start_pt,	SOT_start_pt,	start_SOC;
	int	tile_len; //total_len;

	int	tx0, ty0, tx1, ty1;
	unsigned char *temp;

    // UART init
 //   UartStdOutInit();

    printf("\n*****\tJPEG 2000!\t*****\n\n");
    printf("**********\tTEST START!\t**********\n");
    

	num_tile_hor	= (IMAGEW+TILE_WIDTH-1)/TILE_WIDTH;
	num_tile_ver	= (IMAGEH+TILE_WIDTH-1)/TILE_WIDTH;
	num_tile	= num_tile_hor * num_tile_ver;
	offset		=0;

	ENDWT	= 0;

	//len	= read_file(FILENAME);
	tx0	=0;
	ty0	=0;
	tx1	=0;
	ty1	=0;
	total_len	=0;
	start_SOC	= tile_out_buffer(CTRL_READ_PT, 0, 0);
	jp2_write_mainheader(IMAGEW, IMAGEH, COLR, 0, 8, 4);
	for(tileno=0;tileno<num_tile;tileno++)
	{
		for(compno=0;compno<COLR;compno++)
		{
			tx0	= (tileno % num_tile_hor) * TILE_WIDTH ;
			ty0	= (tileno / num_tile_hor) * TILE_HEIGHT ;
			tx1	= int_min(tx0+TILE_WIDTH, IMAGEW);
			ty1	= int_min(ty0+TILE_HEIGHT, IMAGEH);
			

			start_pt	= tile_out_buffer(CTRL_READ_PT, 0, 0) ;
			SOT_start_pt	= tile_header(tileno, 4, COLR, TILE_WIDTH, TILE_HEIGHT);
	
			dc_level_shifting_color_compononent_transform(tx0, ty0, tx1, ty1, IMAGEW, IMAGEH, COLR, compno);
			ENDWT	= 1;
			DONEDWT=0;
			
			////printf("offset1\n");
			//while(DONEDWT!=1)
			//{
				dwt97(compno, 4, TILE_WIDTH, TILE_HEIGHT);
			
			////printf("offset2\n");
			//}
			ENDWT	= 0;
			tier1(0, TILE_WIDTH, TILE_HEIGHT, compno, 4 );
			
			////printf("offset3\n");
			tier2(4, compno); 
		
			////printf("offset4\n");
			tile_len	= tile_out_buffer(CTRL_READ_PT, 0, 0) - start_pt ;  
			tile_out_buffer(CTRL_PUT, tile_len, (4 <<24) | SOT_start_pt);
			len			= tile_out_buffer(CTRL_READ_PT, 0, 0) ;
			for(i =0; i<len; i++)
			{
				image_dst_buf[total_len + i]	= tile_out_buffer(CTRL_READ, 0, i);
			}
		
			////printf("len:%d\n", len);
			total_len	+= len;
			tile_out_buffer(CTRL_INIT, 0, 0);
		}
	}
	
	tile_out_buffer(CTRL_WRITE, JP2_MS_EOC, 2);
	len			= tile_out_buffer(CTRL_READ_PT, 0, 0);
	
	for(i = 0; i <len; i++)
	{
		image_dst_buf[total_len + i]	= tile_out_buffer(CTRL_READ, 0, i);
	}
	
	total_len += len;
	temp = image_dst_buf;
	printf("total_len:%p, %x\n", temp, total_len);
	printf("len:%d\n", len);
	//write_file(OUTFILE, total_len + len);

	j = 0;
	for ( j= 0 ; j < total_len+len ; j++)
	{
		printf("buffer[%d] = %p\n",j, *(image_dst_buf + j ));
		if(j == 9) 
			j = total_len/2;
		else if (j == total_len/2 + 9)
			j = total_len - 10;
	}

    printf("**********\tTEST PASSED\t**********\n\n");


    // End simulation
//    UartEndSimulation();

#ifdef SYISS
    end_main();
#endif
	return 0;
}

