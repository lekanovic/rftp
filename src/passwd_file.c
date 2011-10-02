#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "passwd_file.h"

#define PASSWD_FILE	"passwd"

static int get_all_posts(struct user*);


/*
 * @param u - adds an new user to passwd file
 * @return - true if user added, false otherwise
 */
int add_user(struct user u)
{
	int ret=1;
	FILE* file;

	if ((file = fopen(PASSWD_FILE,"a+")) == NULL)
		printf("can't open file [%s:%d]\n",__func__,__LINE__);

	if (!find_user(u.name))
		fwrite(&u,1,sizeof(struct user),file);
	else {
		ret = 0;
	}

	fclose(file);
	return ret;
}

static int get_all_posts(struct user* user_list)
{
	FILE* file;
	int bytes,i=0;

	if ((file = fopen(PASSWD_FILE,"a+")) == NULL)
		printf("can't open file [%s:%d]\n",__func__,__LINE__);

	while (!feof(file)) {
		bytes=fread(&user_list[i],1,sizeof(struct user),file);
		if (strlen(user_list[i].name) == 0) break;
		i++;
	}

	fclose(file);
	return i;
}

/*
 * @param name - remove user with name from passwd file
 * @return - no comment
 */
int remove_user(const char* name)
{
#define ELEMENTS	100
	FILE* file;
	int i=0,nbr_of_posts=0,ret=1;
	struct user* old_user_list;

	if ((old_user_list = calloc(ELEMENTS, sizeof(struct user))) == NULL)
		printf("calloc failed\n");

	nbr_of_posts = get_all_posts(old_user_list);

	if ((file = fopen(PASSWD_FILE,"w")) == NULL)
		printf("can't open file [%s:%d]\n",__func__,__LINE__);

	for (i=0; i<nbr_of_posts ;i++ ) {
		if (strcmp(old_user_list[i].name,name) != 0)
			fwrite(&old_user_list[i],1,sizeof(struct user),file);
	}

	fclose(file);
	free(old_user_list);

	return ret;
}

/*
 * @param name - find user in passwd file
 * @return true if user with "name" was found
 *         false otherwise.
 */
int find_user(const char* name)
{
	FILE* file;
	int bytes,ret=0;
	struct user u1;

	if ((file = fopen(PASSWD_FILE,"a+")) == NULL)
		printf("can't open file [%s:%d]\n",__func__,__LINE__);

	while (!feof(file)) {
		memset(&u1,0,sizeof(u1));
		bytes=fread(&u1,1,sizeof(u1),file);
		if (strcmp(name,u1.name) == 0) {
			ret = 1;
			break;
		}
	}

	fclose(file);
	return ret;
}

/*
 * @param u2 - get password from user name
 *    contained in u2.name
 * @return u2 - if user "u2.name" found password
 *    will be returned "u2.password".
 * @return - true if user was found, false otherwise
 */
int get_passwd(struct user* u2)
{
	FILE* file;
	int bytes,ret=0;
	struct user u1;

	if ((file = fopen(PASSWD_FILE,"a+")) == NULL)
		printf("can't open file [%s:%d]\n",__func__,__LINE__);

	while (!feof(file)) {
		memset(&u1,0,sizeof(u1));
		bytes=fread(&u1,1,sizeof(struct user),file);
		if (strcmp(u2->name,u1.name) == 0) {
			ret = 1;
			strcpy(u2->passwd,u1.passwd);
			break;
		}
	}

	fclose(file);
	return ret;
}

/*
 * @param name - user to be tested for password
 * @param passwd - match this passwd from the passwd file
 * @return - true if user passwd approved, false otherwise
 */
int accept_user(const char* name, const char* passwd)
{
	struct user u;
	strcpy(u.name,name);

	if (!get_passwd(&u)) {
		printf("No such user\n");
		return 0;
	} 

	if (strcmp(passwd,u.passwd) == 0)
		return 1;
	else
		return 0;
}

