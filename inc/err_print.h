#ifndef __ERR_PRINT_H__
#define __ERR_PRINT_H__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#define ERR(fmt, args...) printf("[%s:%s:%d] %s "fmt,__FILE__,__func__,__LINE__,strerror(errno));

#endif
