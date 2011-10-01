#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#include "setup.h"

void handler(int sig)
{
	printf("ctrl-c\n");
	close_server();
}

void main()
{
	signal(SIGINT, handler);
	start_server();
}



