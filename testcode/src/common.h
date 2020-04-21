#ifdef SYISS
#ifndef __COMM__
#define __COMM__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define	CLA_STAT	(0xFFFF0000)
#define ARGC_MASK	(0x1)
#define	ARGV_MASK	(0x2)
#define CLA_ARGC	(0xFFFF0004)
#define CLA_ARGV	(0xFFFF00F0)

char** get_argv(int * argc);

void end_main();

#endif
#endif
