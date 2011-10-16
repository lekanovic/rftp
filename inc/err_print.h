#ifndef __ERR_PRINT_H__
#define __ERR_PRINT_H__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define AGGRESIVE	0

#define ERR(fmt, args...) \
	do{ \
		printf("[%s:%s:%d] %s "fmt,__FILE__,__func__,__LINE__,strerror(errno)); \
		if (AGGRESIVE) \
			exit(1); \
	}while(0);

#endif
