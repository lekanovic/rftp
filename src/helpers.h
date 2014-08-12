#ifndef __HELPERS_H__
#define __HELPERS_H__
#include "config_parser.h"
int rm_crlf(char*);
void replace(char*);
char* replace_str(const char*, const char*, const char*);
int64_t timespecDiff(struct timespec *, struct timespec *);
int get_ip_address(int,char*);
int get_sendbuf_size(int);
int get_recvbuf_size(int);
int getport(int,int);
int starts_with(const char *, const char *);
int file_exist(char *);
int dir_exist(const char *);
int get_user_id(const char *);
int init_dir(const char*);
int change_working_dir();
int change_grp(const char*,const char*);
int dele_dir(char*);
int setup_user_env(const char*);
void initialize_system(struct configs*);
#endif
