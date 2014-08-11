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
#include "config_parser.h"

#define HOSTNAME_LEN	200

static int sfd;

static int handle_incomming_clients(struct configs cfg)
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

		if (cfg.disable_nagle_algorithm)
			disable_nagle(client_sfd);

		handle_msg(client_sfd,cfg);

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
	int ret=0;
	struct sockaddr_in server_addr;
	struct hostent *he;
	struct configs cfg;
	cfg.server_dir = ftp_alloc(DIR_LENGTH);

	if ((sfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
		ERR("socket\n");
again:
	if ((ret = gethostname(hostname,HOSTNAME_LEN)) < 0)
		ERR("gethostname\n");

	if ((he = gethostbyname(hostname)) == NULL) {
		ERR("gethostbyname\n");
	}

	initialize_system(&cfg);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(cfg.port);

	printf("Server: %s %s %s:%d\n",
		he->h_name,
		hostname,
		inet_ntoa(server_addr.sin_addr),
		cfg.port);

	if (bind(sfd, (struct sockaddr*)&server_addr,sizeof(server_addr)) < 0) {
		cfg.port++;
		sleep(1);
		perror("bind");
		goto again;
	}


	if (listen(sfd,5) < 0)
		ERR("listen\n");

	while(1) {
		handle_incomming_clients(cfg);
	}

	close(sfd);

	ftp_free(cfg.server_dir);
}

void close_server()
{
	close(sfd);
	exit(1);
}


