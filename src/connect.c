#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
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

int get_ip_addr(struct in_addr *ip)
{
	struct ifreq buffer[32];
	struct ifconf intfc;
	struct ifreq *pIntfc;
	int i,fd,num_intfc;

	intfc.ifc_len = sizeof(buffer);
	intfc.ifc_buf = (char*) buffer;

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		ERR("socket\n");
		return 0;
	}

	if (ioctl(fd, SIOCGIFCONF, &intfc) < 0) {
		ERR("ioctl\n");
		return 0;
	}

	pIntfc = intfc.ifc_req;
	num_intfc = intfc.ifc_len / sizeof(struct ifreq);

	for (i = 0; i < num_intfc; i++) {
		struct ifreq *item = &(pIntfc[i]);

		if (strstr(item->ifr_name,"eth") != NULL) {
			*ip = (((struct sockaddr_in *)&item->ifr_addr)->sin_addr);
		}
	}
	return 1;
}
