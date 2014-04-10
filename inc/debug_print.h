#ifndef __DEBUG_PRINT_H__
#define __DEBUG_PRINT_H__

#include <stdio.h>
#include <errno.h>
#include <string.h>

extern int debug_mode;

#define DEBUG_PRINT(format,args...) \
	do{ \
	      if (debug_mode) \
			printf(format, ##args); \
	}while(0);

#endif
