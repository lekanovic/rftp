#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include "setup.h"

int debug_mode = 0;
int disable_nagle_algorithm = 0;
char server_dir[1024];
int allow_anonymous_login=0;

void handler(int sig)
{
	printf("ctrl-c\n");
	close_server();
}

int parse_arg(int argc, char** argv)
{
	int c;

	while ((c = getopt(argc, argv, "dnc")) != -1) {
		switch (c) {
		case 'd':
			printf("using debug mode\n");
			debug_mode=1;
			break;
		case 'n':
			printf("disable Nagle's algorithm\n");
			disable_nagle_algorithm = 1;
			break;
		case 'c':
			break;
		case '?':
			break;
		}
	}
	return 0;
}

int main(int argc, char** argv)
{
	signal(SIGINT, handler);
	parse_arg(argc,argv);
	start_server();
	return 0;
}



