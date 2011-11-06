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
#include <time.h>
#include "helpers.h"
#include "parser.h"
#include "mem_op.h"
#include "file_op.h"
#include "err_print.h"
#include "connect.h"
#include "ftp_msg.h"
#include "login.h"

#define BUF_SIZE	1024
#define USER_NAME_LEN	30

#define NO_USER			(-2)
#define WRONG_PASSWD		(-3)
//http://mina.apache.org/ftpserver/ftp-commands.html

extern int debug_mode;

#define PLOG(sock,fmt, args...) \
	if (debug_mode) { \
		char ipstr[20]; \
		get_ip_address(sock,ipstr); \
		printf("IP: %s %s:%d "fmt,ipstr,__FILE__,__LINE__,args); \
	}

static int parse_msg(int,char*,char*);
int data_fd;

enum SEND_TYPE send_mode=ascii;

static int verify_login(int cmd_port, char* user_name)
{
	char msg[BUF_SIZE];
	int bytes;
	memset(msg,0,BUF_SIZE);

	while (1) {
		if ((bytes = ftp_recv(cmd_port,msg,BUF_SIZE,0)) == 0) {
			printf("Client closed connection\n");
			return 0;
		}
		if (strstr(msg,"USER") != NULL) {
			if (handle_user(cmd_port,msg,user_name) == NO_USER) {
				PLOG(cmd_port,"%s is trying to login\n",user_name);
				return 0;
			}
		} else if ( strstr(msg,"PASS") != NULL) {
			if (handle_pass(cmd_port,msg,user_name) == WRONG_PASSWD)
				return 0;
			else {
				char *dir = get_home_dir(user_name);
				PLOG(cmd_port,"HOME: %s\n",dir);
				if (chdir(dir) < 0)
					ERR("chdir\n");
				return 1;
			}
		}
		memset(msg,0,BUF_SIZE);
	}
}

int handle_msg(int client_sfd)
{
	char user_name[USER_NAME_LEN];
	char buf[BUF_SIZE]={0};
	int bytes,response;

	ftp_send(client_sfd,WELCOME_MSG,strlen(WELCOME_MSG),0);

	if (!verify_login(client_sfd,user_name)){
		ftp_send(client_sfd,GOODBYE,strlen(GOODBYE),0);
		return 0;
	}

	while(1) {
		if ((bytes = ftp_recv(client_sfd,buf,BUF_SIZE,0)) == 0) {
			printf("Client closed connection\n");
			return 0;
		}
		response = parse_msg(client_sfd,buf,user_name);
		if (response < 0) {
			ftp_send(client_sfd,GOODBYE,strlen(GOODBYE),0);
			break;
		}

		memset(buf,0,BUF_SIZE);
	}
	return 0;
}

static int parse_msg(int client_sfd,char* msg,char* user_name)
{
	if ( strstr(msg,"ABOR") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"ACCT") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"ALLO") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"APPE") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"AUTH") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"CCC") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"CDUP") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"CONF") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"CWD") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		handle_cwd(client_sfd,msg);
	} else if ( strstr(msg,"DELE") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		handle_dele(client_sfd,msg);
	} else if ( strstr(msg,"ENC") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"EPRT") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"FEAT") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		handle_feat(client_sfd);
	} else if ( strstr(msg,"LANG") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"LIST") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		return handle_list(client_sfd,msg);
	} else if ( strstr(msg,"LPRT") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"LPSV") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"MDTM") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"MIC") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"MKD") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		handle_mkd(client_sfd,msg);
	} else if ( strstr(msg,"MLSD") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"MLST") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"MODE") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"NLST") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		handle_nlst(client_sfd);
	} else if ( strstr(msg,"NOOP") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		handle_noop(client_sfd);
	} else if ( strstr(msg,"OPTS") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"PASS") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		return handle_pass(client_sfd,msg,user_name);
	} else if ( strstr(msg,"PASV") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		if ((data_fd = handle_pasv(client_sfd)) < 0) {
			ftp_send(client_sfd,CANNOT_OPEN_DATA_CON,
					strlen(CANNOT_OPEN_DATA_CON),0);
		}
		PLOG(client_sfd,"Passive port opened %d\n",data_fd);
	} else if ( strstr(msg,"PBSZ") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"PORT") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		data_fd = handle_port(client_sfd,msg);
		PLOG(client_sfd,"Active port opened %d\n",data_fd);
	} else if ( strstr(msg,"PROT") != NULL) {
		printf("%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"PWD") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		handle_pwd(client_sfd);
	} else if ( strstr(msg,"QUIT") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		return -1;
	} else if ( strstr(msg,"REIN") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"REST") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"RETR") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		handle_retr(client_sfd,msg);
	} else if ( strstr(msg,"RMD") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		handle_rmd(client_sfd,msg);
	} else if ( strstr(msg,"RNFR") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"RNTO") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"SITE") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"SIZE") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		handle_size(client_sfd,msg);
	} else if ( strstr(msg,"SMNT") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"STAT") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"STOR") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		handle_stor(client_sfd,msg);
	} else if ( strstr(msg,"STOU") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"STRU") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
	} else if ( strstr(msg,"SYST") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		handle_syst(client_sfd,msg);
	} else if ( strstr(msg,"TYPE") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		handle_type(client_sfd,msg);
	} else if ( strstr(msg,"USER") != NULL) {
		PLOG(client_sfd,"%s %d %s\n",__func__,__LINE__,msg);
		return handle_user(client_sfd,msg,user_name);
	} else if ( strstr(msg,"EPSV") != NULL) {
		ftp_send(client_sfd,COMMAND_NOT_UNDERSTOOD, strlen(COMMAND_NOT_UNDERSTOOD),0);
	} else {
		return -1;
	}

	return 0;
}
int handle_size(int cmd_port,char *msg)
{
	char buf[BUF_SIZE]={0};
	struct stat st;
	int fd;
	char *file = msg + 5;

	rm_crlf(file);

	if ((fd=stat(file,&st)) < 0)
		ERR("stat\n");

	sprintf(buf,"213 %d\r\n",(int)st.st_size);

	ftp_send(cmd_port,buf,strlen(buf),0);

	return 0;
}
int handle_nlst(int cmd_port)
{
	int i=0;
	char **ppdir;

	ftp_send(cmd_port,OPEN_ASCII_MODE,strlen(OPEN_ASCII_MODE),0);

	if ((ppdir=ls(ONLY_FILE_NAMES)) != NULL) {
		for (i=0; ppdir[i] != NULL;i++){
			ftp_send(data_fd,ppdir[i],strlen(ppdir[i]),0);
			ftp_free(ppdir[i]);
		}
		ftp_free(ppdir);
	}

	ftp_send(cmd_port,TRANSFER_COMPLETE,strlen(TRANSFER_COMPLETE),0);

	close(data_fd);

	return 0;
}
int handle_noop(int cmd_port)
{
	ftp_send(cmd_port,SEND_OK,strlen(SEND_OK),0);
	return 0;
}
int handle_feat(int cmd_port)
{
	ftp_send(cmd_port,FEAT_NOT_IMPLEMENTED,strlen(FEAT_NOT_IMPLEMENTED),0);
	return 0;
}
// DELE <SP> <pathname> <CRLF>
int handle_dele(int cmd_port,char *msg)
{
	int ret;
	char *send_msg,*file = msg + 5;

	rm_crlf(file);

	if ((ret=unlink(file)) < 0) {
		ERR("unlink\n");
	}

	send_msg = ftp_calloc(25 + strlen(file), sizeof(char));

	sprintf(send_msg,"250 %s has been deleted\r\n",file);

	ftp_send(cmd_port,send_msg,strlen(send_msg),0);

	ftp_free(send_msg);

	return 0;
}
// RMD  <SP> <pathname> <CRLF>
// 250 Requested file action okay, completed.
int handle_rmd(int cmd_port,char* msg)
{
	int ret;
	char* send_msg;
	char* dir = msg + 4;

	rm_crlf(dir);

	if ((ret=rmdir(dir)) < 0) {
		ERR("rmdir\n");
		send_msg = ftp_calloc(4+strlen(FILE_UNAVAILABLE),sizeof(char));
		sprintf(send_msg,"%s",FILE_UNAVAILABLE);
	} else {
		send_msg = ftp_calloc(strlen(FILE_ACTION_OK)+1,
				sizeof(char));
		sprintf(send_msg,"%s",FILE_ACTION_OK);
	}
	ftp_send(cmd_port,send_msg,strlen(send_msg),0);

	ftp_free(send_msg);
	return 0;
}
int handle_mkd(int cmd_port,char* msg)
{
	char* send_msg;
	char* dir = msg + 4;
	int ret;

	rm_crlf(dir);

	if ((ret=mkdir(dir,S_IRWXU)) < 0) {
		ERR("mkdir\n");
		send_msg = ftp_calloc(4+strlen(FILE_UNAVAILABLE),sizeof(char));
		sprintf(send_msg,"%s",FILE_UNAVAILABLE);
	} else {
		send_msg = ftp_calloc(4+strlen(dir)+strlen("\'directory created\'")+1,
				 sizeof(char));
		sprintf(send_msg,"257 %s directory created\r\n",dir);
	}
	ftp_send(cmd_port,send_msg,strlen(send_msg),0);

	ftp_free(send_msg);
	return 0;
}
int handle_type(int cmd_port,char* msg)
{
	if ( strstr(msg,"TYPE A") != NULL) {
		send_mode=ascii;
		ftp_send(cmd_port,TYPE_ASCII_MODE,strlen(TYPE_ASCII_MODE),0);
	} else if ( strstr(msg,"TYPE I") != NULL) {
		send_mode=image;
		ftp_send(cmd_port,TYPE_BINARY_MODE,strlen(TYPE_BINARY_MODE),0);
	} else if ( strstr(msg,"TYPE E") != NULL) {
		send_mode=ebcdic;
		ftp_send(cmd_port,TYPE_EBCDIC_MODE,strlen(TYPE_EBCDIC_MODE),0);
	} else {
		ERR("No TYPE\n");
	}

	return 0;
}
int handle_pass(int cmd_port,char* msg,char* user_name)
{
	char* passwd = msg + 5;

	rm_crlf(passwd);

	if (!check_passwd(user_name,passwd)) {
		ftp_send(cmd_port,AUTHEN_FAILED,
				strlen(AUTHEN_FAILED),0);
		PLOG(cmd_port,"User %s typed incorrect passwd: %s\n",user_name,passwd);
		return WRONG_PASSWD;
	}

	PLOG(cmd_port,"User %s logged in successfully\n",user_name);

	ftp_send(cmd_port,LOG_IN_OK,strlen(LOG_IN_OK),0);

	return 0;
}
int handle_syst(int cmd_port,char* msg)
{
	ftp_send(cmd_port,SYSTEM_TYPE,strlen(SYSTEM_TYPE),0);
	return 0;
}
int handle_user(int cmd_port,char* msg,char* user_name)
{
	memset(user_name,0,30);
	strcpy(user_name,msg + 5);

	rm_crlf(user_name);

	if (!find_user(user_name)) {
		ftp_send(cmd_port,INVALID_USER_NAME,
				strlen(INVALID_USER_NAME),0);
		PLOG(cmd_port,"%s could not be found\n",user_name);
		return NO_USER;
	}

	ftp_send(cmd_port,NEED_PASSWD,
			strlen(NEED_PASSWD),0);

	return 0;
}
int handle_stor(int cmd_port, char *msg)
{
	int fd,bytes,data,buf_size;
	char *buf,*file = msg + 5;
	time_t start,finish;
	struct stat st;

	rm_crlf(file);

	buf_size = get_recvbuf_size(cmd_port);

	buf = ftp_alloc(buf_size);

	ftp_send(cmd_port,START_STOR_CMD,strlen(START_STOR_CMD),0);

	if ((fd=open(file,O_CREAT|O_RDWR,S_IRWXU)) < 0)
		ERR("open\n");

	start = time(NULL);

	while ((data=ftp_recv(data_fd,buf,buf_size,0)) != 0) {
		if ((bytes=write(fd,buf,data)) < 0)
			ERR("write\n");
		lseek(fd,0,SEEK_END);
	}

	finish = time(NULL) - start;

	if (stat(file,&st) < 0)
		ERR("stat\n");

	PLOG(cmd_port,"File %s %d bytes received in %ldsec\n",
		file,
		(int)st.st_size,
		(long)finish);

	ftp_send(cmd_port,TRANSFER_COMPLETE,strlen(TRANSFER_COMPLETE),0);

	close(fd);
	close(data_fd);
	ftp_free(buf);

	return 0;
}
int handle_retr(int cmd_port, char* msg)
{
	int fd,bytes,buf_size;
	time_t start,finish;
	char *buf,*file = msg + 5;
	struct stat st;

	rm_crlf(file);

	buf_size = get_recvbuf_size(cmd_port);

	buf = ftp_alloc(buf_size);

	if ((fd = open(file,O_RDONLY)) < 0) {
		ftp_send(cmd_port,FILE_UNAVAILABLE,strlen(FILE_UNAVAILABLE),0);
		return 0;
	}

	if ( send_mode == ascii) {
		ftp_send(cmd_port,OPEN_ASCII_MODE,strlen(OPEN_ASCII_MODE),0);
	} else if ( send_mode == image) {
		ftp_send(cmd_port,OPEN_BINARY_MODE,strlen(OPEN_BINARY_MODE),0);
	} else if ( send_mode == ebcdic) {
		ftp_send(cmd_port,OPEN_EBCDIC_MODE,strlen(OPEN_EBCDIC_MODE),0);
	} else if ( send_mode == local) {

	}

	start = time(NULL);

	while ((bytes = read(fd,buf,buf_size)) != 0) {
		ftp_send_mode(data_fd,buf,bytes,0,send_mode);
	}

	finish = time(NULL) - start;

	if (stat(file,&st) < 0)
		ERR("stat\n");

	PLOG(cmd_port,"File %s %d bytes sent in %ldsec\n",
		file,
		(int)st.st_size,
		(long)finish);

	ftp_send(cmd_port,TRANSFER_COMPLETE,strlen(TRANSFER_COMPLETE),0);

	close(data_fd);
	close(fd);
	ftp_free(buf);

	return 0;
}
int handle_cwd(int cmd_port,char* msg)
{
	char *str;
	if ((str = ftp_calloc(strlen(msg),sizeof(char))) == NULL)
		ERR("calloc\n");

	strcpy(str,msg+4);

	rm_crlf(str);

	if (chdir(str) < 0) {
		ERR("chdir\n")
		ftp_send(cmd_port,NOT_AN_DIRECTORY,strlen(NOT_AN_DIRECTORY),0);
	} else {
		ftp_send(cmd_port,WORKING_DIR_CHANGED,strlen(WORKING_DIR_CHANGED),0);
	}

	ftp_free(str);

	return 0;

}
int handle_list(int cmd_port,char* msg)
{
	int i=0;
	char **ppdir;
	char data[5*1024]={0};

	if ( send_mode == ascii) {
		ftp_send(cmd_port,OPEN_ASCII_MODE,strlen(OPEN_ASCII_MODE),0);
	} else if ( send_mode == image) {
		ftp_send(cmd_port,OPEN_BINARY_MODE,strlen(OPEN_BINARY_MODE),0);
	} else if ( send_mode == ebcdic) {
		ftp_send(cmd_port,OPEN_EBCDIC_MODE,strlen(OPEN_EBCDIC_MODE),0);
	} else if ( send_mode == local) {

	}

	if ((ppdir=ls(!ONLY_FILE_NAMES)) != NULL) {
		for (i=0; ppdir[i] != NULL;i++){
			strcat(data,ppdir[i]);
			ftp_free(ppdir[i]);
		}
		ftp_free(ppdir);
	}

	ftp_send_mode(data_fd,data,strlen(data),0,send_mode);

	ftp_send(cmd_port,TRANSFER_COMPLETE,strlen(TRANSFER_COMPLETE),0);

	close(data_fd);

	return 2;
}
int handle_pwd(int cmd_port)
{
	char *msg;

	msg = ftp_calloc(BUF_SIZE, sizeof(char));

	strcpy(msg, "257 ");

	getcwd(msg+strlen(msg),BUF_SIZE);

	strcat(msg,"\r\n");

	ftp_send(cmd_port,msg,strlen(msg),0);

	ftp_free(msg);

	return 0;
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
	char msg[BUF_SIZE]={0};
	char *ip;
	struct sockaddr_in address;
	socklen_t addrlen;

	if ((sfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
		ERR("socket\n");
again:
	port = getport(a,b);

	address.sin_family = AF_INET;
	get_ip_addr(&address.sin_addr);
	address.sin_port = htons(port);

	if (bind(sfd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		b++;
		goto again;
	}
	if (listen(sfd,3) < 0)
		ERR("listen\n");

	addrlen = sizeof(struct sockaddr_in);

	ip = inet_ntoa(address.sin_addr);

	replace(ip);

	sprintf(msg,"227 Entering Passive Mode (%s,%d,%d)\r\n",ip,a,b);

	ftp_send(cmd_port,msg,strlen(msg),0);

	if ((client_pasv = accept(sfd,(struct sockaddr*)&address,&addrlen)) < 0)
		ERR("accept\n");

	return client_pasv;
}

int handle_port(int cmd_port,char* msg)
{
	int fd;
	char* pch;
	char buf[20]={0};
	int ip[6],i=0;
	struct sockaddr_in serv_addr;

	pch = strtok(msg," ,");
	pch = strtok(NULL," ,");

	while (pch != NULL) {
		ip[i++] = atoi(pch);
		pch = strtok(NULL," ,");
	}

	sprintf(buf,"%d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);

	serv_addr.sin_addr.s_addr = inet_addr(buf);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(ip[4]*256+ip[5]);

	PLOG(cmd_port,"Connecting to %s:%d\n",inet_ntoa(serv_addr.sin_addr),serv_addr.sin_port);

	fd = socket(AF_INET,SOCK_STREAM,0);
	if (connect(fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
		ERR("connect\n");

	ftp_send(cmd_port,PORT_CMD_OK,strlen(PORT_CMD_OK),0);

	return fd;
}


