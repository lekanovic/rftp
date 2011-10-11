#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "parser.h"
#include "mem_op.h"
#include "file_op.h"
/* CR \r
 * LF \n
 */
#define LOG_IN_OK		"230 Welcome to server\r\n"
#define WELCOME_MSG		"220 Raddes ftp server beta v1.0\r\n"
#define LOGIN_MSG		"331 Anonymous access allowed\r\n"
#define SYSTEM_TYPE		"215 LINUX-2.6\r\n"
#define OPEN_ASCII_MODE		"150 Opening ASCII mode data connection\r\n"
#define OPEN_BINARY_MODE	"150 Opening binary mode data connection\r\n"
#define WORKING_DIR_CHANGED	"200 Working directory changed\r\n"
#define TRANSFER_COMPLETE	"226 Transfer complete\r\n"
#define GOODBYE			"221 Goodbye\r\n"
#define PORT_CMD_OK		"200 PORT command successful\r\n"
#define START_STOR_CMD		"150 File start to download\r\n"
#define ENTER_PASSV_MODE	"227 Entering Passive Mode\r\n"
#define FILE_UNAVAILABLE	"550 Requested action not taken. File unavailable\r\n"
#define FILE_ACTION_OK		"250 Requested file action okay, completed\r\n"
#define DEBUG 0

#define debug_print(args ...) if (DEBUG) fprintf(stderr, args)

static int parse_msg(int,char*);

static ssize_t ftp_recv(int sockfd, void *buf, size_t len, int flags)
{
	int bytes;

	if ((bytes = recv(sockfd,buf,len,flags)) < 0)
		printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));

	debug_print("\033[01;31m[DATA:recv:fd=%d] %s\033[0m\n",sockfd,(char*)buf);

	return bytes;
}
static ssize_t ftp_send(int sockfd, const void *buf, size_t len, int flags)
{
	int bytes;

	debug_print("\033[01;34m[DATA:send:fd=%d] %s\033[0m\n",sockfd,(char*)buf);

	if ((bytes = send(sockfd,buf,len,flags)) < 0)
		printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));

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

		memset(buf,0,1024);
	}
	printf("echo msg exit\n");
	return 0;
}
int data_fd;

static int parse_msg(int client_sfd,char* msg)
{
	if ( strstr(msg,"ABOR") != NULL) {
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
		handle_cwd(client_sfd,msg);
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
		return handle_list(client_sfd,msg);
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
		handle_mkd(client_sfd,msg);
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
		handle_pass(client_sfd,msg);
	} else if ( strstr(msg,"PASV") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		data_fd = handle_pasv(client_sfd);
		printf("Passive port opened %d\n",data_fd);
	} else if ( strstr(msg,"PBSZ") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"PORT") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		data_fd = handle_port(client_sfd,msg);
		printf("Active port opened %d\n",data_fd);
	} else if ( strstr(msg,"PROT") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"PWD") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		handle_pwd(client_sfd);
	} else if ( strstr(msg,"QUIT") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		return -1;
	} else if ( strstr(msg,"REIN") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"REST") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"RETR") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		handle_retr(client_sfd,msg);
	} else if ( strstr(msg,"RMD") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		handle_rmd(client_sfd,msg);
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
		handle_stor(client_sfd,msg);
	} else if ( strstr(msg,"STOU") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"STRU") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"SYST") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		handle_syst(client_sfd,msg);
	} else if ( strstr(msg,"TYPE") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		handle_type(client_sfd,msg);
	} else if ( strstr(msg,"USER") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
		handle_user(client_sfd,msg);
	} else
		return -1;

	return 0;
}
// RMD  <SP> <pathname> <CRLF>
// 250 Requested file action okay, completed.
int handle_rmd(int cmd_port,char* msg)
{
	int ret;
	char* send_msg;
	char* dir = msg + 4;
	dir[strlen(dir)-1] = dir[strlen(dir)-2] = '\0';//remove \r\n

	if ((ret=rmdir(dir)) < 0) {
		printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));
		send_msg = calloc(4+strlen(FILE_UNAVAILABLE),sizeof(char));
		sprintf(send_msg,"%s",FILE_UNAVAILABLE);
	} else {
		send_msg = calloc(strlen(FILE_ACTION_OK)+1,
				sizeof(char));
		sprintf(send_msg,"%s",FILE_ACTION_OK);
	}
	ftp_send(cmd_port,send_msg,strlen(send_msg),0);

	free(send_msg);
	return 0;
}
int handle_mkd(int cmd_port,char* msg)
{
	char* send_msg;
	char* dir = msg + 4;
	int ret;

	dir[strlen(dir)-1] = dir[strlen(dir)-2] = '\0';//remove \r\n

	if ((ret=mkdir(dir,S_IRWXU)) < 0) {
		printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));
		send_msg = calloc(4+strlen(FILE_UNAVAILABLE),sizeof(char));
		sprintf(send_msg,"%s",FILE_UNAVAILABLE);
	} else {
		send_msg = calloc(4+strlen(dir)+strlen("\'directory created\'")+1,
				 sizeof(char));
		sprintf(send_msg,"257 %s directory created\r\n",dir);
	}
	ftp_send(cmd_port,send_msg,strlen(send_msg),0);

	free(send_msg);
	return 0;
}
int handle_type(int cmd_port,char* msg)
{
	if ( strstr(msg,"TYPE A") != NULL)
		ftp_send(cmd_port,OPEN_ASCII_MODE,strlen(OPEN_ASCII_MODE),0);
	else if ( strstr(msg,"TYPE I") != NULL)
		ftp_send(cmd_port,OPEN_BINARY_MODE,strlen(OPEN_BINARY_MODE),0);
	return 0;
}
int handle_pass(int cmd_port,char* msg)
{
	ftp_send(cmd_port,LOG_IN_OK,strlen(LOG_IN_OK),0);
	return 0;
}
int handle_syst(int cmd_port,char* msg)
{
	ftp_send(cmd_port,SYSTEM_TYPE,strlen(SYSTEM_TYPE),0);
	return 0;
}
int handle_user(int cmd_port,char* msg)
{
	ftp_send(cmd_port,LOGIN_MSG,strlen(LOGIN_MSG),0);
	return 0;
}
int handle_stor(int cmd_port, char *msg)
{
	int fd,bytes,data;
	char *file = msg + 5;
	char buf[1024];
	memset(buf,0,1024);
	file[strlen(file)-1] = file[strlen(file)-2] = '\0';//remove \r\n

	ftp_send(cmd_port,START_STOR_CMD,strlen(START_STOR_CMD),0);

	if ((fd=open(file,O_CREAT|O_RDWR,S_IRWXU)) < 0)
		printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));

	while ((data=ftp_recv(data_fd,buf,1024,0)) != 0) {
		if ((bytes=write(fd,buf,data)) < 0)
			printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));
		lseek(fd,0,SEEK_END);
	}

	ftp_send(cmd_port,TRANSFER_COMPLETE,strlen(TRANSFER_COMPLETE),0);

	close(fd);
	close(data_fd);
	return 0;
}
int handle_retr(int cmd_port, char* msg)
{
	char *buf;
	int fd,file_size,bytes;
	char *file = msg + 5;

	file[strlen(file)-1] = file[strlen(file)-2] = '\0';

	if ((fd = open(file,O_RDONLY)) < 0) {
		printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));
		ftp_send(cmd_port,FILE_UNAVAILABLE,strlen(FILE_UNAVAILABLE),0);
		return 0;
	}

	if ((file_size = lseek(fd,0,SEEK_END)) < 0) {
		printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));
	}

	if (lseek(fd,0,SEEK_SET) < 0) {
		printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));
	}

	if ((buf = malloc(file_size)) == NULL) {
		printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));
	}

	if ((bytes = read(fd,buf,file_size)) < 0) {
		printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));
	}

	ftp_send(cmd_port,OPEN_BINARY_MODE,strlen(OPEN_BINARY_MODE),0);

	ftp_send(data_fd,buf,file_size,0);

	ftp_send(cmd_port,TRANSFER_COMPLETE,strlen(TRANSFER_COMPLETE),0);

	close(data_fd);
	close(fd);
	free(buf);

	return 0;
}
int handle_cwd(int cmd_port,char* msg)
{
	char *str;
	if ((str = calloc(strlen(msg),sizeof(char))) == NULL)
		printf("calloc failed\n");

	strcpy(str,msg+4);
	str[strlen(str)-2] = '\0';

	if (chdir(str) < 0) {
		printf("%s\n",strerror(errno));
	}
	free(str);

	ftp_send(cmd_port,WORKING_DIR_CHANGED,strlen(WORKING_DIR_CHANGED),0);

	return 0;

}
int handle_list(int cmd_port,char* msg)
{
	int i=0;
	char **ppdir;

	ftp_send(cmd_port,OPEN_ASCII_MODE,strlen(OPEN_ASCII_MODE),0);

	if ((ppdir=ls()) != NULL) {
		for (i=0; ppdir[i] != NULL;i++){
			ftp_send(data_fd,ppdir[i],strlen(ppdir[i]),0);
			ftp_free(ppdir[i]);
		}
		ftp_free(ppdir);
	}

	ftp_send(cmd_port,TRANSFER_COMPLETE,strlen(TRANSFER_COMPLETE),0);

	close(data_fd);

	return 2;
}
int handle_pwd(int cmd_port)
{
#define BUF_SIZE	1024
	char *msg;

	msg = calloc(BUF_SIZE, sizeof(char));

	strcpy(msg, "257 ");

	getcwd(msg,BUF_SIZE);

	strcat(msg,"\r\n");

	ftp_send(cmd_port,msg,strlen(msg),0);

	free(msg);

	return 0;
}
static int getport(int a,int b)
{
	return (a*256 + b);
}
/*
 * passive FTP :
 *     command : client >1023 -> server 21
 *     data    : client >1023 -> server > 1023
 *
 * ip[4]*256+ip[5] => 178,16 => 178*256 + 16 = 45584;
 */
int handle_pasv(int cmd_port)
{
	int sfd,client_pasv;
	int port=45584,a=178,b=16;
	char msg[1024];
	struct sockaddr_in address;
	socklen_t addrlen;

	memset(msg,0,1024);

	if ((sfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
		printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));
again:
	port = getport(a,b);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(sfd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		b++;
		goto again;
	}
	if (listen(sfd,3) < 0)
		printf("[%s:%s:%d] %s\n",__FILE__,__func__,__LINE__,strerror(errno));

	addrlen = sizeof(struct sockaddr_in);

	sprintf(msg,"227 Entering Passive Mode (127,0,0,1,%d,%d)\r\n",a,b);

	ftp_send(cmd_port,msg,strlen(msg),0);

	client_pasv = accept(sfd,(struct sockaddr*)&address,&addrlen);

	return client_pasv;
}

int handle_port(int cmd_port,char* msg)
{
	int fd;
	char* pch;
	int ip[6],i=0;
	struct sockaddr_in serv_addr;

	pch = strtok(msg," ,");
	pch = strtok(NULL," ,");

	while (pch != NULL) {
		ip[i++] = atoi(pch);
		pch = strtok(NULL," ,");
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(ip[4]*256+ip[5]);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	fd = socket(AF_INET,SOCK_STREAM,0);
	if (connect(fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
		printf("%s %d %s port %d\n",__func__,__LINE__,
				strerror(errno),serv_addr.sin_port);

	ftp_send(cmd_port,PORT_CMD_OK,strlen(PORT_CMD_OK),0);

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





