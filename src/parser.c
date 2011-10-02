#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <dirent.h>
#include <arpa/inet.h>
#include "parser.h"
/* CR \r
 * LF \n
 */
#define WELCOME_MSG	"220 Raddes ftp server beta v1.0\r\n"
#define LOGIN_MSG	"331 Anonymous access allowed\r\n"
#define SYSTEM_TYPE	"211 Ubuntu 11.04 Intel Core i7\r\n"
#define PWD_OK	"257 /radde/padde\r\n"
#define OPEN_ASCII_MODE	"150 Opening ASCII mode data connection for file list\r\n"
#define GOODBYE	"221 Goodbye\r\n"

static int parse_msg(int,char*);

static ssize_t ftp_recv(int sockfd, void *buf, size_t len, int flags)
{
	int bytes;

	if ((bytes = recv(sockfd,buf,len,0)) < 0)
		printf("%s line %d\n",strerror(errno),__LINE__);

	printf("[DATA:recv:fd=%d] %s\n",sockfd,(char*)buf);

	return bytes;
}
static ssize_t ftp_send(int sockfd, const void *buf, size_t len, int flags)
{
	int bytes;

	printf("[DATA:send:fd=%d] %s\n",sockfd,(char*)buf);

	if ((bytes = send(sockfd,buf,len,0)) < 0)
		printf("%s line %d\n",strerror(errno),__LINE__);

	return bytes;
}


int handle_msg(int client_sfd)
{
	char buf[1024];
	int bytes,response;

	if (send(client_sfd,WELCOME_MSG,strlen(WELCOME_MSG),0) < 0)
		printf("%s line %d\n",strerror(errno),__LINE__);

	while(1) {
		if ((bytes = ftp_recv(client_sfd,buf,1024,0)) == 0) {
			printf("Client closed connection\n");
			return 0;
		}
		response = parse_msg(client_sfd,buf);
		if (response < 0) {
			printf("Client exited, bye\n\r");
			ftp_send(client_sfd,GOODBYE,strlen(GOODBYE),0);
			break;
		}

		if (ftp_send(client_sfd,buf,strlen(buf),0) < 0)
			printf("%s line %d\n",strerror(errno),__LINE__);

		memset(buf,0,1024);
	}
	printf("echo msg exit\n");
	return 0;
}
int port_fd;

static int parse_msg(int client_sfd,char* msg)
{
	if (strstr(msg,"?") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"ABOR") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"ACCT") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"ALLO") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"APPE") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"AUTH") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"CCC") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"CDUP") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"CONF") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"CWD") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		//strncpy(msg,". .. a b c\n\r",1024);
	} else if ( strstr(msg,"DELE") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"ENC") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"EPRT") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"FEAT") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"LANG") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"LIST") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		ftp_send(client_sfd,OPEN_ASCII_MODE,strlen(OPEN_ASCII_MODE),0);
		return handle_list(msg);
	} else if ( strstr(msg,"LPRT") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"LPSV") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"MDTM") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"MIC") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"MKD") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"MLSD") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"MLST") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"MODE") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"NLST") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"NOOP") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"OPTS") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"PASS") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		memset(msg,0,1024);
		strncpy(msg,"230 Welcome to server\n",1024);
	} else if ( strstr(msg,"PASV") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"PBSZ") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"PORT") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		port_fd = handle_port(msg);
	} else if ( strstr(msg,"PROT") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"PWD") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		handle_pwd(msg);
	} else if ( strstr(msg,"QUIT") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		return -1;
	} else if ( strstr(msg,"REIN") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"REST") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"RETR") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"RMD") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"RNFR") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"RNTO") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"SITE") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"SIZE") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"SMNT") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"STAT") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"STOR") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"STOU") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"STRU") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"SYST") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		memset(msg,0,1024);
		strncpy(msg,SYSTEM_TYPE,strlen(SYSTEM_TYPE));
	} else if ( strstr(msg,"TYPE") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"USER") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		memset(msg,0,1024);
		strncpy(msg,LOGIN_MSG,strlen(LOGIN_MSG));
	} else
		return -1;

	return 0;
}

int handle_list(char* msg)
{
	DIR *dir;
	struct dirent *dent;
	int bytes=0;

	memset(msg,0,1024);
	dir = opendir(".");

	while ((dent = readdir(dir)) != NULL) {
		bytes += strlen(dent->d_name);
		if ( bytes >= 1024 ) break;
		strcat(msg,dent->d_name);
		strcat(msg," ");
	}
	strcat(msg,"\r\n");
	ftp_send(port_fd,msg,strlen(msg),0);

	memset(msg,0,1024);
	strcpy(msg,"226 Transfer complete\r\n");
	close(port_fd);

	return 2;
}
int handle_pwd(char* msg)
{
	char tmp[512];

	memset(tmp,0,512);
	getcwd(tmp,512);
	memset(msg,0,1024);
	strcpy(msg,"250 ");
	strcat(msg,tmp);
	strcat(msg,"\r\n");

	return 0;
}

int handle_port(char* msg)
{
	int fd;
	char* pch;
	char tmp[10];
	int ip[6],i=0;
	struct sockaddr_in serv_addr;

	pch = strtok(msg," ,");
	pch = strtok(NULL," ,");

	while (pch != NULL) {
		ip[i++] = atoi(pch);
		pch = strtok(NULL," ,");
	}

	memset(tmp,0,10);
	sprintf(tmp,"%x%x",ip[4],ip[5]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(strtol(tmp,NULL,16));
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	fd = socket(AF_INET,SOCK_STREAM,0);
	if (connect(fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
		printf("%s %d %s port %d\n",__func__,__LINE__,
				strerror(errno),serv_addr.sin_port);

	memset(msg,0,1024);
	strcpy(msg, "200 PORT command successful\r\n");
	return fd;
}

int echo_msg(int client_sfd)
{
	char buf[1024];
	int bytes;

	if (send(client_sfd,WELCOME_MSG,strlen(WELCOME_MSG),0) < 0)
		printf("%s line %d\n",strerror(errno),__LINE__);

	while(1) {
		if ((bytes = recv(client_sfd,buf,1024,0)) < 0)
			printf("%s line %d\n",strerror(errno),__LINE__);
		else if (bytes == 0) {//Client closed connection
			printf("Client closed connection\n");
			return 0;
		}

		if (send(client_sfd,buf,strlen(buf),0) < 0)
			printf("%s line %d\n",strerror(errno),__LINE__);

		if (strstr(buf,"exit") != NULL) {
			printf("client exits\n");
		 	break;
		}

		printf("[DATA] %s\n",buf);

		memset(buf,0,1024);
	}
	printf("echo msg exit\n");
	return 0;
}




