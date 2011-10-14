#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "err_print.h"
#include "parser.h"

static int sfd;

void connection(void* argument)
{
	int client_sfd;

	client_sfd = *((int*)argument);

	handle_msg(client_sfd);
}

int start_server()
{
	char hostname[200];
	int client_sfd,pid=0,port=7000,ret=0;
	socklen_t addrlen;
	struct sockaddr_in server_addr,client_addr;
	struct hostent *he;

	if ((sfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
		ERR("socket\n");
again:
	if ((ret = gethostname(hostname,200)) < 0)
		ERR("gethostname\n");

	if ((he = gethostbyname(hostname)) == NULL) {
		ERR("gethostbyname\n");
	}

	server_addr.sin_family = AF_INET;
	memcpy(&server_addr.sin_addr,he->h_addr_list[0],he->h_length);
	server_addr.sin_port = htons(port);

	if (bind(sfd, (struct sockaddr*)&server_addr,sizeof(server_addr)) < 0) {
		port++;
		goto again;
	}

	printf("Server: %s %s %s:%d\n",
		he->h_name,
		hostname,
		inet_ntoa(*((struct in_addr *)he->h_addr)),
		port);

	if (listen(sfd,5) < 0)
		ERR("listen\n");

	addrlen = sizeof(struct sockaddr_in);

	while(1) {
		if ((client_sfd = accept(sfd,(struct sockaddr*)&client_addr,&addrlen)) < 0)
			ERR("accept\n");

		if ((pid = fork()) < 0)
			ERR("fork\n");

		if (pid == 0) {
			char ipstr[100];
			struct sockaddr_in disconn_addr;
			socklen_t addr_len;

			printf("Connected %s:%d\n",
				inet_ntoa(client_addr.sin_addr),
				client_sfd);

			close(sfd);

			connection(&client_sfd);

			/*
			 * This is an bug in getpeername socklen_t is 8 bytes
			 * the library code expects 4 bytes so the struct was
			 * no populated because it was too short. So extra 4
			 * had to be added.
			*/
			addr_len = sizeof(addr_len) + 4;

			if ((ret=getpeername(client_sfd,(struct sockaddr *)&disconn_addr,&addr_len)) < 0 )
				ERR("getpeername\n");

			inet_ntop(AF_INET,&disconn_addr.sin_addr,ipstr,sizeof(ipstr));

			printf("Disconnected %s:%d\n",
				ipstr,
				client_sfd);

			close(client_sfd);

			exit(0);
		} else {
			close(client_sfd);
		}
	}

	close(sfd);
}

void close_server()
{
	close(sfd);
	exit(1);
}


