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
#include "debug_print.h"
#include "logger.h"

#define BUF_SIZE	1024
#define USER_NAME_LEN	30

#define END_CONNECTION		(-1)
#define NO_USER			(-2)
#define WRONG_PASSWD		(-3)
#define FILE_BUFFER_NAME	1024

//http://mina.apache.org/ftpserver/ftp-commands.html

static int parse_msg(int,char*,char*);
static int data_fd;

enum SEND_TYPE send_mode=ascii;

struct ftp_cmd_func
{
	char* cmd;
	int (*func_ptr)(struct req_input* in);
};

struct ftp_cmd_func ftp_cmds[] = {
	{ "ABOR", NULL },
	{ "ACCT", NULL },
	{ "ALLO", NULL },
	{ "APPE", NULL },
	{ "AUTH", NULL },
	{ "CCC", NULL },
	{ "CDUP", NULL },
	{ "CONF", NULL },
	{ "CWD", handle_cwd },
	{ "DELE", handle_dele },
	{ "ENC", NULL },
	{ "EPRT", NULL },
	{ "FEAT", handle_feat },
	{ "LANG", NULL },
	{ "LIST", handle_list },
	{ "LPRT", NULL },
	{ "LPSV", NULL },
	{ "MDTM", handle_mdtm },
	{ "MIC", NULL },
	{ "MKD", handle_mkd },
	{ "MLSD", NULL },
	{ "MLST", handle_nlst },
	{ "MODE", NULL },
	{ "NLST", NULL },
	{ "NOOP", handle_noop },
	{ "OPTS", NULL },
	{ "PASS", handle_pass },
	{ "PASV", handle_pasv },
	{ "PBSZ", NULL },
	{ "PORT", handle_port },
	{ "PROT", NULL },
	{ "PWD", handle_pwd },
	{ "QUIT", handle_quit },
	{ "REIN", NULL },
	{ "REST", NULL },
	{ "RETR", handle_retr },
	{ "RMD", handle_rmd },
	{ "RNFR", handle_rnfr },
	{ "SITE", handle_site },
	{ "SIZE", handle_size },
	{ "SMNT", NULL },
	{ "STAT", NULL },
	{ "STOR", handle_stor },
	{ "STOU", NULL },
	{ "STRU", NULL },
	{ "SYST", handle_syst },
	{ "TYPE", handle_type },
	{ "USER", handle_user },
	{ "EPSV", handle_epsv },
	{ NULL, NULL }
};

static int verify_login(int cmd_port, char* user_name,struct configs cfg)
{
	char msg[BUF_SIZE];
	int bytes;
	struct req_input in;
	in.client_fd = cmd_port;
	in.user_name = user_name;
	in.msg = msg;

	memset(msg,0,BUF_SIZE);

	while (1) {
		if ((bytes = ftp_recv(cmd_port,msg,BUF_SIZE,0)) == 0) {
			printf("Client closed connection\n");
            filelog(cmd_port,msg);
			return 0;
		}
		if (strstr(msg,"USER") != NULL) {
			if (cfg.allow_anonymous_login) {
				if ( strstr(msg,"USER anonymous") != NULL) {
					setup_user_env(cfg.server_dir);
					ftp_send(cmd_port,LOG_IN_OK,strlen(LOG_IN_OK),0);
                    filelog(cmd_port,"USER anonymous");
					return 1;
				}
			}

			if (handle_user(&in) == NO_USER) {
				DEBUG_PRINT(cmd_port,"%s is trying to login\n",user_name);
				return 0;
			}
		} else if ( strstr(msg,"PASS") != NULL) {
			if (handle_pass(&in) == WRONG_PASSWD) {
                filelog(cmd_port,"Wrong password");
				return 0;
            }
			else {
				int userId = setup_user_env(cfg.server_dir);

				DEBUG_PRINT(cmd_port,"SetUid=%d for %s\n",userId,user_name);

				return 1;
			}
		}  else if ( strstr(msg,"FEAT") != NULL) {
			handle_feat(&in);
		}

		memset(msg,0,BUF_SIZE);
	}
}

int handle_msg(int client_sfd,struct configs cfg)
{
	char user_name[USER_NAME_LEN];
	char buf[BUF_SIZE]={0};
	int bytes,response;

	ftp_send(client_sfd,WELCOME_MSG,strlen(WELCOME_MSG),0);

	if (!verify_login(client_sfd,user_name,cfg)){
		ftp_send(client_sfd,GOODBYE,strlen(GOODBYE),0);
		return 0;
	}

	while(response != END_CONNECTION) {
		if ((bytes = ftp_recv(client_sfd,buf,BUF_SIZE,0)) == 0) {
			printf("Client closed connection\n");
            filelog(client_sfd,"Client closed connection\n");
			return 0;
		}
		response = parse_msg(client_sfd,buf,user_name);

		memset(buf,0,BUF_SIZE);
	}
	return 0;
}

static int parse_msg(int client_sfd,char* msg,char* user_name)
{
	int i=0;
	struct req_input in;
	int size = ARRAY_SIZE(ftp_cmds);
	int ret=0;

	in.msg = msg;
	in.client_fd = client_sfd;
	in.user_name = user_name;

	for (;i < size ; i++) {
		if ( strstr(msg,ftp_cmds[i].cmd) != NULL &&
			ftp_cmds[i].func_ptr != NULL) {
			ret = ftp_cmds[i].func_ptr(&in);
            filelog(client_sfd,msg);
			break;

		}
	}

	return ret;
}
//https://tools.ietf.org/html/rfc3659#page-10
int handle_mdtm(struct req_input* in)
{
	// MDTM remote-filename
	char buf[256];
	struct tm *foo;
	struct stat attrib;
	char *file = in->msg + 5;

	rm_crlf(file);

	if (!file_exist(file)) {
		ftp_send(in->client_fd,FILE_NOT_FOUND,strlen(FILE_NOT_FOUND),0);
		return 0;
	}

	if (dir_exist(file)) {
		char message[128];
		sprintf(message,"550 %s is not retrievable\r\n",file);
		ftp_send(in->client_fd,message,strlen(message),0);
		return 0;
	}

	stat(file, &attrib);
	foo = gmtime(&(attrib.st_mtime));

	if (debug_mode) {
		printf("Last change of %s\n",file);
		printf("Year: %d\n", foo->tm_year);
		printf("Month: %d\n", foo->tm_mon);
		printf("Day: %d\n", foo->tm_mday);
		printf("Hour: %d\n", foo->tm_hour);
		printf("Minute: %d\n", foo->tm_min);
		printf("Second: %d\n", foo->tm_sec);
	}

	sprintf(buf,"213 %d%d%d%d%d%d\r\n",
		foo->tm_year,
		foo->tm_mon,
		foo->tm_mday,
		foo->tm_hour,
		foo->tm_min,
		foo->tm_sec);

	ftp_send(in->client_fd,buf,strlen(buf),0);

	return 0;
}

int handle_rnfr(struct req_input* in)
{
	//RNFR /home/tester/Untitled Folder
	char *file_path = in->msg + 5;
	char old_file_name[FILE_BUFFER_NAME];
	char recv_msg[BUF_SIZE];
	int bytes;

	memset(recv_msg,0,BUF_SIZE);
	memset(old_file_name,0,FILE_BUFFER_NAME);

	rm_crlf(file_path);

	strcpy(old_file_name,file_path);

	if (!file_exist(file_path)){
		DEBUG_PRINT(in->client_fd,"File does not exist %s\n",file_path);
		ftp_send(in->client_fd,FILE_NOT_FOUND,strlen(FILE_NOT_FOUND),0);
		return 0;
	}

	ftp_send(in->client_fd,REQUEST_FILE_ACTION,strlen(REQUEST_FILE_ACTION),0);

	if ((bytes = ftp_recv(in->client_fd,recv_msg,BUF_SIZE,0)) == 0) {
		ERR("Client closed connection\n");
		return 0;
	}
	// RNTO must be precedded with an RNFR
	if (strstr(recv_msg,"RNTO") == NULL){
		ERR("Error RNTO command must be followed by an RNFR command\n");
		ftp_send(in->client_fd,BAD_COMMAND_SEQUENCE,strlen(BAD_COMMAND_SEQUENCE),0);
		return 0;
	}

	//RNTO /home/tester/newFileName
	char *new_file_name = recv_msg + 5;

	rm_crlf(old_file_name);
	rm_crlf(new_file_name);

	if(rename(old_file_name, new_file_name) == 0) {
		DEBUG_PRINT(in->client_fd,"[%s] has been renamed to [%s].\n", old_file_name, new_file_name);
	} else {
		fprintf(stderr, "Error renaming %s to %s\n", old_file_name,new_file_name);
		printf("an error: %s\n", strerror(errno));
		ftp_send(in->client_fd,FILE_NOT_FOUND,strlen(FILE_NOT_FOUND),0);
		return -1;
	}

	ftp_send(in->client_fd,FILE_ACTION_DONE,strlen(FILE_ACTION_DONE),0);
	memset(old_file_name,0,FILE_BUFFER_NAME);

	return 0;
}
int handle_site(struct req_input* in)
{
	ftp_send(in->client_fd,FEAT_NOT_IMPLEMENTED,strlen(FEAT_NOT_IMPLEMENTED),0);
	return 0;
}
int handle_size(struct req_input* in)
{
	char buf[BUF_SIZE]={0};
	struct stat st;
	int fd;
	char *file = in->msg + 5;

	rm_crlf(file);

	if ((fd=stat(file,&st)) < 0) {
		ERR("stat\n");
		ftp_send(in->client_fd,FILE_NOT_FOUND,strlen(FILE_NOT_FOUND),0);
	} else {
		sprintf(buf,"213 %d\r\n",(int)st.st_size);
		ftp_send(in->client_fd,buf,strlen(buf),0);
	}

	return 0;
}
int handle_nlst(struct req_input* in)
{
	int i=0;
	char **ppdir;

	ftp_send(in->client_fd,OPEN_ASCII_MODE,strlen(OPEN_ASCII_MODE),0);

	if ((ppdir=ls(ONLY_FILE_NAMES)) != NULL) {
		for (i=0; ppdir[i] != NULL;i++){
			ftp_send(data_fd,ppdir[i],strlen(ppdir[i]),0);
			ftp_free(ppdir[i]);
		}
		ftp_free(ppdir);
	}

	ftp_send(in->client_fd,TRANSFER_COMPLETE,strlen(TRANSFER_COMPLETE),0);

	close(data_fd);

	return 0;
}
int handle_noop(struct req_input* in)
{
	ftp_send(in->client_fd,SEND_OK,strlen(SEND_OK),0);
	return 0;
}
int handle_feat(struct req_input* in)
{
	ftp_send(in->client_fd,FEAT_NOT_IMPLEMENTED,strlen(FEAT_NOT_IMPLEMENTED),0);
	return 0;
}
// DELE <SP> <pathname> <CRLF>
int handle_dele(struct req_input* in)
{
	int ret;
	char *send_msg,*file = in->msg + 5;

	rm_crlf(file);

	if ((ret=unlink(file)) < 0) {
		ERR("unlink");
	}

	send_msg = ftp_calloc(25 + strlen(file), sizeof(char));

	sprintf(send_msg,"250 %s has been deleted\r\n",file);

	ftp_send(in->client_fd,send_msg,strlen(send_msg),0);

	ftp_free(send_msg);

	return 0;
}
// RMD  <SP> <pathname> <CRLF>
// 250 Requested file action okay, completed.
int handle_rmd(struct req_input* in)
{
	char* send_msg;
	char* dir = in->msg + 4;

	rm_crlf(dir);

	if (!dir_exist(dir)) {
		ERR("dir does not exist\n");
		send_msg = ftp_calloc(4+strlen(FILE_UNAVAILABLE),sizeof(char));
		sprintf(send_msg,"%s",FILE_UNAVAILABLE);
	} else {
		dele_dir(dir);
		send_msg = ftp_calloc(strlen(FILE_ACTION_OK)+1,
				sizeof(char));
		sprintf(send_msg,"%s",FILE_ACTION_OK);
	}
	ftp_send(in->client_fd,send_msg,strlen(send_msg),0);

	ftp_free(send_msg);
	return 0;
}
int handle_mkd(struct req_input* in)
{
	char* send_msg;
	char* dir = in->msg + 4;
	int ret;
	int size=0;

	rm_crlf(dir);

	if ((ret=mkdir(dir,S_IRWXU)) < 0) {
		ERR("mkdir\n");
		size = (4+strlen(FILE_UNAVAILABLE));
		send_msg = ftp_calloc(size,sizeof(char));
		sprintf(send_msg,"%s",FILE_UNAVAILABLE);
	} else {
		size = (4+strlen(dir)+strlen("\'directory created\'")+3);
		send_msg = ftp_calloc(size, sizeof(char));
		sprintf(send_msg,"257 %s directory created\r\n",dir);
	}

	ftp_send(in->client_fd,send_msg,strnlen(send_msg,size),0);

	ftp_free(send_msg);
	return 0;
}
int handle_type(struct req_input* in)
{
	if ( strstr(in->msg,"TYPE A") != NULL) {
		send_mode=ascii;
		ftp_send(in->client_fd,TYPE_ASCII_MODE,strlen(TYPE_ASCII_MODE),0);
	} else if ( strstr(in->msg,"TYPE I") != NULL) {
		send_mode=image;
		ftp_send(in->client_fd,TYPE_BINARY_MODE,strlen(TYPE_BINARY_MODE),0);
	} else if ( strstr(in->msg,"TYPE E") != NULL) {
		send_mode=ebcdic;
		ftp_send(in->client_fd,TYPE_EBCDIC_MODE,strlen(TYPE_EBCDIC_MODE),0);
	} else {
		ERR("No TYPE\n");
	}

	return 0;
}
int handle_pass(struct req_input* in)
{
	char* passwd = in->msg + 5;

	rm_crlf(passwd);

	if (!check_passwd(in->user_name,passwd)) {
		ftp_send(in->client_fd,AUTHEN_FAILED,
				strlen(AUTHEN_FAILED),0);
		DEBUG_PRINT(in->client_fd,"User %s typed incorrect passwd: %s\n",in->user_name,passwd);
		return WRONG_PASSWD;
	}

	DEBUG_PRINT(in->client_fd,"User %s logged in successfully\n",in->user_name);

	ftp_send(in->client_fd,LOG_IN_OK,strlen(LOG_IN_OK),0);

	return 0;
}
int handle_syst(struct req_input* in)
{
	ftp_send(in->client_fd,SYSTEM_TYPE,strlen(SYSTEM_TYPE),0);
	return 0;
}
int handle_user(struct req_input* in)
{
	memset(in->user_name,0,30);
	strcpy(in->user_name,in->msg + 5);

	rm_crlf(in->user_name);

	if (!find_user(in->user_name)) {
		ftp_send(in->client_fd,INVALID_USER_NAME,
				strlen(INVALID_USER_NAME),0);
		DEBUG_PRINT(in->client_fd,"%s could not be found\n",in->user_name);
		return NO_USER;
	}

	ftp_send(in->client_fd,NEED_PASSWD,
			strlen(NEED_PASSWD),0);

	return 0;
}
int handle_stor(struct req_input* in)
{
	int fd,bytes,data,buf_size;
	char *buf,*file = in->msg + 5;
	time_t start,finish;
	struct stat st;

	rm_crlf(file);

	buf_size = get_recvbuf_size(in->client_fd);

	buf = ftp_alloc(buf_size);

	ftp_send(in->client_fd,START_STOR_CMD,strlen(START_STOR_CMD),0);

	if ((fd=open(file,O_CREAT|O_RDWR,S_IRWXU)) < 0)
		ERR("open\n");

	start = time(NULL);

	while ((data=ftp_recv(data_fd,buf,buf_size,0)) != 0) {
		if ((bytes=write(fd,buf,data)) < 0) {
			ERR("write\n");
			exit(1);
		}
		lseek(fd,0,SEEK_END);
	}

	finish = time(NULL) - start;

	if (stat(file,&st) < 0)
		ERR("stat\n");

	DEBUG_PRINT(in->client_fd,"File %s %d bytes received in %ldsec\n",
		file,
		(int)st.st_size,
		(long)finish);

	ftp_send(in->client_fd,TRANSFER_COMPLETE,strlen(TRANSFER_COMPLETE),0);

	close(fd);
	close(data_fd);
	ftp_free(buf);

	return 0;
}
int handle_retr(struct req_input* in)
{
	int fd,bytes,buf_size;
	time_t start,finish;
	char *buf,*file = in->msg + 5;
	struct stat st;

	rm_crlf(file);

	buf_size = get_recvbuf_size(in->client_fd);

	buf = ftp_alloc(buf_size);

	if ((fd = open(file,O_RDONLY)) < 0) {
		ftp_send(in->client_fd,FILE_UNAVAILABLE,strlen(FILE_UNAVAILABLE),0);
		return 0;
	}

	if ( send_mode == ascii) {
		ftp_send(in->client_fd,OPEN_ASCII_MODE,strlen(OPEN_ASCII_MODE),0);
	} else if ( send_mode == image) {
		ftp_send(in->client_fd,OPEN_BINARY_MODE,strlen(OPEN_BINARY_MODE),0);
	} else if ( send_mode == ebcdic) {
		ftp_send(in->client_fd,OPEN_EBCDIC_MODE,strlen(OPEN_EBCDIC_MODE),0);
	} else if ( send_mode == local) {

	}

	start = time(NULL);

	while ((bytes = read(fd,buf,buf_size)) != 0) {
		ftp_send_mode(data_fd,buf,bytes,0,send_mode);
	}

	finish = time(NULL) - start;

	if (stat(file,&st) < 0)
		ERR("stat\n");

	DEBUG_PRINT(in->client_fd,"File %s %d bytes sent in %ldsec\n",
		file,
		(int)st.st_size,
		(long)finish);

	ftp_send(in->client_fd,TRANSFER_COMPLETE,strlen(TRANSFER_COMPLETE),0);

	close(data_fd);
	close(fd);
	ftp_free(buf);

	return 0;
}
int handle_cwd(struct req_input* in)
{
	char *str;
	if ((str = ftp_calloc(strlen(in->msg),sizeof(char))) == NULL)
		ERR("calloc\n");

	strcpy(str,in->msg+4);

	rm_crlf(str);

	if (chdir(str) < 0) {
		ERR("chdir\n")
		ftp_send(in->client_fd,NOT_AN_DIRECTORY,strlen(NOT_AN_DIRECTORY),0);
	} else {
		ftp_send(in->client_fd,WORKING_DIR_CHANGED,strlen(WORKING_DIR_CHANGED),0);
	}

	ftp_free(str);

	return 0;

}
int handle_list(struct req_input* in)
{
	int i=0;
	char **ppdir;
	char data[5*1024]={0};

	if ( send_mode == ascii) {
		ftp_send(in->client_fd,OPEN_ASCII_MODE,strlen(OPEN_ASCII_MODE),0);
	} else if ( send_mode == image) {
		ftp_send(in->client_fd,OPEN_BINARY_MODE,strlen(OPEN_BINARY_MODE),0);
	} else if ( send_mode == ebcdic) {
		ftp_send(in->client_fd,OPEN_EBCDIC_MODE,strlen(OPEN_EBCDIC_MODE),0);
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

	ftp_send(in->client_fd,TRANSFER_COMPLETE,strlen(TRANSFER_COMPLETE),0);

	close(data_fd);

	return 2;
}
int handle_pwd(struct req_input* in)
{
	char *msg;
	char tmp[NAME_MAX];

	msg = ftp_calloc(BUF_SIZE, sizeof(char));

	if (getcwd(tmp,NAME_MAX) == NULL) {
		ERR("getcwd\n");
	}

	sprintf(msg,"257 \"%s\" is the current directory.\r\n",tmp);

	ftp_send(in->client_fd,msg,strlen(msg),0);

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
int handle_pasv(struct req_input* in)
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

	ftp_send(in->client_fd,msg,strlen(msg),0);

	if ((data_fd = accept(sfd,(struct sockaddr*)&address,&addrlen)) < 0)
		ERR("accept\n");

	if (data_fd < 0) {
		ftp_send(in->client_fd,CANNOT_OPEN_DATA_CON,
				strlen(CANNOT_OPEN_DATA_CON),0);
	}
	return client_pasv;
}

int handle_port(struct req_input* in)
{
	int fd;
	char* pch;
	char buf[20]={0};
	int ip[6],i=0;
	struct sockaddr_in serv_addr;

	pch = strtok(in->msg," ,");
	pch = strtok(NULL," ,");

	while (pch != NULL) {
		ip[i++] = atoi(pch);
		pch = strtok(NULL," ,");
	}

	sprintf(buf,"%d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);

	serv_addr.sin_addr.s_addr = inet_addr(buf);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(ip[4]*256+ip[5]);

	DEBUG_PRINT(in->client_fd,"Connecting to %s:%d\n",inet_ntoa(serv_addr.sin_addr),serv_addr.sin_port);

	fd = socket(AF_INET,SOCK_STREAM,0);
	if (connect(fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
		ERR("connect\n");

	ftp_send(in->client_fd,PORT_CMD_OK,strlen(PORT_CMD_OK),0);

	return fd;
}

int handle_epsv(struct req_input* in)
{
	ftp_send(in->client_fd,
		COMMAND_NOT_UNDERSTOOD,
		strlen(COMMAND_NOT_UNDERSTOOD),0);
	return 0;
}

int handle_quit(struct req_input* in)
{
	ftp_send(in->client_fd,GOODBYE,strlen(GOODBYE),0);
	return END_CONNECTION;
}
