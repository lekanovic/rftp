#ifndef __DEBUG_PRINT_H__
#define __DEBUG_PRINT_H__

#include <stdio.h>
#include <errno.h>
#include <string.h>

extern int debug_mode;

#define DEBUG_PRINT(sock,fmt, args...) \
	do{ \
	if (debug_mode) { \
		char ipstr[20]; \
		get_ip_address(sock,ipstr); \
		printf("IP: %s %s:%d "fmt,ipstr,__FILE__,__LINE__,args); \
	} \
	}while(0);

#endif

