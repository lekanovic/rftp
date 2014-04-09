#define _XOPEN_SOURCE
#include <shadow.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "login.h"
extern struct passwd *getpwent (void);
#define NOT_FOUND	0
#define OP_OK		1

static int check_pass(const char *plainpw, const char *cryptpw)
{
	char *str = (char*)crypt(plainpw,cryptpw);

	return (strcmp(str, cryptpw) == 0);
}

int find_user(const char* name)
{
	struct passwd *pw;

	if ((pw = getpwnam(name)) == NULL) {
		return NOT_FOUND;
	}
	return OP_OK;
}

int check_passwd(const char* name, const char* passwd)
{
	struct spwd *sp;

	if ((sp = getspnam(name)) == NULL)
		return NOT_FOUND;

	if (!check_pass(passwd,sp->sp_pwdp))
		return NOT_FOUND;

	return OP_OK;
}

char* get_home_dir(const char* name)
{
	struct passwd *ps=getpwnam(name);
	return ps->pw_dir;
}
