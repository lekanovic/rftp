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
#include "helpers.h"
#include "err_print.h"
#include "parser.h"
#include "connect.h"
#include "mem_op.h"

#define HOSTNAME_LEN	200
#define SERVER_PORT	21

static int sfd;

void connection(void* argument)
{
	int client_sfd;

	client_sfd = *((int*)argument);

	handle_msg(client_sfd);
}

static int handle_incomming_clients()
{
	int pid=0;
	int client_sfd;
	socklen_t addrlen;
	struct sockaddr_in client_addr;

	addrlen = sizeof(struct sockaddr_in);

	if ((client_sfd = accept(sfd,(struct sockaddr*)&client_addr,&addrlen)) < 0)
		ERR("accept\n");

	if ((pid = fork()) < 0)
		ERR("fork\n");

	if (pid == 0) {
		char ipstr[20];

		printf("Connected %s:%d\n",
			inet_ntoa(client_addr.sin_addr),
			client_sfd);

		close(sfd);

		connection(&client_sfd);

		get_ip_address(client_sfd,ipstr);

		printf("Disconnected %s:%d\n",
			ipstr,
			client_sfd);

		close(client_sfd);

		exit(0);
	} else {
		close(client_sfd);
	}
	return 0;
}

int start_server()
{
	char hostname[HOSTNAME_LEN];
	int port=SERVER_PORT,ret=0;
	struct sockaddr_in server_addr;
	struct hostent *he;

	if ((sfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
		ERR("socket\n");
again:
	if ((ret = gethostname(hostname,HOSTNAME_LEN)) < 0)
		ERR("gethostname\n");

	if ((he = gethostbyname(hostname)) == NULL) {
		ERR("gethostbyname\n");
	}

	server_addr.sin_family = AF_INET;
	get_ip_addr(&server_addr.sin_addr);
	server_addr.sin_port = htons(port);


	printf("Server: %s %s %s:%d\n",
		he->h_name,
		hostname,
		inet_ntoa(server_addr.sin_addr),
		port);

	if (bind(sfd, (struct sockaddr*)&server_addr,sizeof(server_addr)) < 0) {
		port++;
		sleep(1);
		goto again;
	}


	if (listen(sfd,5) < 0)
		ERR("listen\n");

	while(1) {
		handle_incomming_clients();
	}

	close(sfd);
}

void close_server()
{
	close(sfd);
	exit(1);
}


