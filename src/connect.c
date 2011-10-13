#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include "connect.h"
#include "err_print.h"

#define DEBUG 0

#define debug_print(args ...) if (DEBUG) fprintf(stderr, args)

int disable_nagle(int cmd_port)
{
	int ret,flag;

	flag = 1;

	if ((ret=setsockopt( cmd_port, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag))) < 0)
		ERR("setsockopt\n");

	return ret;
}

ssize_t ftp_recv(int sockfd, void *buf, size_t len, int flags)
{
	int bytes;

	if ((bytes = recv(sockfd,buf,len,flags)) < 0)
		ERR("recv\n");

	debug_print("\033[01;31m[DATA:recv:fd=%d] %s\033[0m\n",sockfd,(char*)buf);

	return bytes;
}
ssize_t ftp_send(int sockfd, const void *buf, size_t len, int flags)
{
	int bytes;

	debug_print("\033[01;34m[DATA:send:fd=%d] %s\033[0m\n",sockfd,(char*)buf);

	if ((bytes = send(sockfd,buf,len,flags)) < 0)
		ERR("send\n");

	return bytes;
}

