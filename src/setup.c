#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "parser.h"

#define THREADS	20

static int sfd;

void* connection(void* argument)
{
	int client_sfd;
	char buf[1024];

	client_sfd = *((int*)argument);
	printf("new client, sock =%d\n",client_sfd);

	handle_msg(client_sfd);

	printf("client exit, sock =%d\n",client_sfd);
	close(client_sfd);
	pthread_exit(NULL);
}

int start_server()
{
	int client_sfd,i=0;
	int addrlen;
	struct sockaddr_in address;
	char buf[1024];
	pthread_t threads[THREADS];

	if ((sfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
		printf("%s line %d\n",strerror(errno),__LINE__);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(7000);

	if (bind(sfd, (struct sockaddr*)&address, sizeof(address)) < 0)
		printf("%s line %d\n",strerror(errno),__LINE__);

	if (listen(sfd,3) < 0)
		printf("%s line %d\n",strerror(errno),__LINE__);

	addrlen = sizeof(struct sockaddr_in);

	while(1) {
		client_sfd = accept(sfd,(struct sockaddr*)&address,&addrlen);
		pthread_create(&threads[i++], NULL, connection, (void*)&client_sfd);
	}
	pthread_exit(NULL);
	close(sfd);
}

void close_server()
{
	close(sfd);
	exit(1);
}


