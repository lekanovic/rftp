#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "mem_op.h"

static const char *get_perms(mode_t mode);

char** ls(int name_only)
{
	DIR* dir;
	struct dirent *dp;
	struct stat statbuf;
	struct passwd *pwd;
	struct group *grp;
	struct tm *tm;
	char datestring[256];
	char** line;
	int i=0;

	//Check if dir exists
	if ((dir = opendir(".")) == NULL) {
		printf("%s\n",strerror(errno));
		return NULL;
	}

	line = (char **)ftp_alloc(256*256);
	memset(line,0,256*256);

	/* Loop through directory entries. */
	for(i=0; (dp = readdir(dir)) != NULL;i++) {
		if (dp->d_name[0] == '.' || dp->d_name[1] == '.'){
			i--;
			continue;
		}

		line[i] = ftp_alloc(256);
		memset(line[i],0,256);

		/* Get entry's information. */
		if (stat(dp->d_name, &statbuf) == -1)
			continue;

		/* We only want file names */
		if ( name_only) {
			sprintf(line[i],"%s\r\n",dp->d_name);
			continue;
		}

		/* Print out type, permissions, and number of links. */
		sprintf(line[i],"%10.10s", get_perms(statbuf.st_mode));
		sprintf(line[i] + strlen(line[i]),"%4d", (int)statbuf.st_nlink);

		/* Print out owner's name if it is found using getpwuid(). */
		if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
			sprintf(line[i] + strlen(line[i])," %-8.8s", pwd->pw_name);
		else
			sprintf(line[i] + strlen(line[i])," %-8d", statbuf.st_uid);

		/* Print out group name if it is found using getgrgid(). */
		if ((grp = getgrgid(statbuf.st_gid)) != NULL)
			sprintf(line[i] + strlen(line[i])," %-8.8s", grp->gr_name);
		else
			sprintf(line[i] + strlen(line[i])," %-8d", statbuf.st_gid);

		/* Print size of file. */
		sprintf(line[i] + strlen(line[i]) ," %9jd", (intmax_t)statbuf.st_size);
		tm = localtime(&statbuf.st_mtime);

		/* Get localized date string. */
		strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
		sprintf(line[i] + strlen(line[i])," %s %s\r\n", datestring, dp->d_name);
	}
	closedir(dir);
	return line;

}

static char perms_buff[30];

static const char *get_perms(mode_t mode)
{
	char ftype = '?';
	if (S_ISREG(mode)) ftype = '-';
	if (S_ISLNK(mode)) ftype = 'l';
	if (S_ISDIR(mode)) ftype = 'd';
	if (S_ISBLK(mode)) ftype = 'b';
	if (S_ISCHR(mode)) ftype = 'c';
	if (S_ISFIFO(mode)) ftype = '|';

	sprintf(perms_buff, "%c%c%c%c%c%c%c%c%c%c %c%c%c",
		ftype,
		mode & S_IRUSR ? 'r' : '-',
		mode & S_IWUSR ? 'w' : '-',
		mode & S_IXUSR ? 'x' : '-',
		mode & S_IRGRP ? 'r' : '-',
		mode & S_IWGRP ? 'w' : '-',
		mode & S_IXGRP ? 'x' : '-',
		mode & S_IROTH ? 'r' : '-',
		mode & S_IWOTH ? 'w' : '-',
		mode & S_IXOTH ? 'x' : '-',
		mode & S_ISUID ? 'U' : '-',
		mode & S_ISGID ? 'G' : '-',
		mode & S_ISVTX ? 'S' : '-');

	return (const char *)perms_buff;
}
