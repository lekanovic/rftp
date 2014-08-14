#ifndef __PARSER_H__
#define __PARSER_H__
#include "config_parser.h"

struct req_input
{
	int client_fd;
	char* msg;
	char* user_name;
};


int handle_list(struct req_input*);
int handle_pwd(struct req_input*);
int handle_type(struct req_input*);
int handle_port(struct req_input*);
int handle_pasv(struct req_input*);
int handle_cwd(struct req_input*);
int handle_retr(struct req_input*);
int handle_stor(struct req_input*);
int handle_user(struct req_input*);
int handle_syst(struct req_input*);
int handle_pass(struct req_input*);
int handle_mkd(struct req_input*);
int handle_rmd(struct req_input*);
int handle_dele(struct req_input*);
int handle_feat(struct req_input*);
int handle_noop(struct req_input*);
int handle_nlst(struct req_input*);
int handle_size(struct req_input*);
int handle_site(struct req_input*);
int handle_rnfr(struct req_input*);
int handle_mdtm(struct req_input*);
int handle_epsv(struct req_input*);
int handle_quit(struct req_input*);

int handle_msg(int,struct configs);

enum SEND_TYPE {
	ascii,
	ebcdic,
	image,
	local
};

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

#endif
