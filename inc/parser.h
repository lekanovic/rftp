#ifndef __PARSER_H__
#define __PARSER_H__
int echo_msg(int);
int handle_msg(int);
int handle_list(char*);
int handle_pwd(char*);
int handle_port(char*);
int handle_pasv(int,char*);
int handle_cwd(char*);
int handle_retr(int,char*);
int handle_stor(int,char*);
#endif
