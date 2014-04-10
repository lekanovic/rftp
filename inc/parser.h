#ifndef __PARSER_H__
#define __PARSER_H__
int echo_msg(int);
int handle_msg(int);
int handle_list(int,char*);
int handle_pwd(int);
int handle_type(int,char*);
int handle_port(int,char*);
int handle_pasv(int);
int handle_cwd(int,char*);
int handle_retr(int,char*);
int handle_stor(int,char*);
int handle_user(int,char*,char*);
int handle_syst(int,char*);
int handle_pass(int,char*,char*);
int handle_mkd(int,char*);
int handle_rmd(int,char*);
int handle_dele(int,char*);
int handle_feat(int);
int handle_noop(int);
int handle_nlst(int);
int handle_size(int,char*);
int handle_site(int,char*);

enum SEND_TYPE {
	ascii,
	ebcdic,
	image,
	local
};

#endif
