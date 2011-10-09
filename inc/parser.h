#ifndef __PARSER_H__
#define __PARSER_H__
int echo_msg(int);
int handle_msg(int);
int handle_list(int,char*);
int handle_pwd(int);
int handle_port(int,char*);
int handle_pasv(int,char*);
int handle_cwd(int,char*);
int handle_retr(int,char*);
int handle_stor(int,char*);
int handle_user(int,char*);
int handle_syst(int,char*);
int handle_pass(int,char*);
#endif
