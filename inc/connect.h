#ifndef __CONNECT_H__
#define __CONNECT_H__
#include "parser.h"
int disable_nagle(int);
ssize_t ftp_recv(int, void *, size_t, int);
ssize_t ftp_send(int, const void *, size_t, int);
int get_ip_addr(struct in_addr *);
ssize_t ftp_send_mode(int, const void *, size_t, int,enum SEND_TYPE);
#endif
