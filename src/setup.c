#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "parser.h"

#define THREADS	20

static int sfd;

void* connection(void* argument)
{
	int client_sfd;

	client_sfd = *((int*)argument);
	printf("new client, sock =%d\n",client_sfd);

	handle_msg(client_sfd);

	printf("client exit, sock =%d\n",client_sfd);
	close(client_sfd);
	pthread_exit(NULL);
}

int start_server()
{
	int client_sfd,i=0,port=7000;
	socklen_t addrlen;
	struct sockaddr_in server_addr,client_addr;
	pthread_t threads[THREADS];

	if ((sfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
		printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));
again:
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	if (bind(sfd, (struct sockaddr*)&server_addr,sizeof(server_addr)) < 0) {
		port++;
		goto again;
	}
	printf("Using port: %d\n",port);
	if (listen(sfd,3) < 0)
		printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));

	addrlen = sizeof(struct sockaddr_in);

	while(1) {
		client_sfd = accept(sfd,(struct sockaddr*)&client_addr,&addrlen);
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


