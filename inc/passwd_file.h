#ifndef __PASSWDFILE_H__
#define __PASSWDFILE_H__
struct user {
	char name[50];
	char passwd[20];
};

int add_user(struct user);
int remove_user(const char*);
int find_user(const char*);
int get_passwd(struct user*);
int accept_user(const char*, const char*);

#endif
