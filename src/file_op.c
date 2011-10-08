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
#include "mem_op.h"

char** ls()
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

	line = (char **)ftp_alloc(256*256);

	dir = opendir(".");
	/* Loop through directory entries. */
	for(i=0; (dp = readdir(dir)) != NULL;i++) {
		line[i] = ftp_alloc(256);

		/* Get entry's information. */
		if (stat(dp->d_name, &statbuf) == -1)
			continue;

		/* Print out type, permissions, and number of links. */
		// printf("%10.10s", (statbuf.st_mode));
		sprintf(line[i],"%4d", (int)statbuf.st_nlink);

		/* Print out owner's name if it is found using getpwuid(). */
		if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
			sprintf(line[i]," %-8.8s", pwd->pw_name);
		else
			sprintf(line[i]," %-8d", statbuf.st_uid);

		/* Print out group name if it is found using getgrgid(). */
		if ((grp = getgrgid(statbuf.st_gid)) != NULL)
			sprintf(line[i]," %-8.8s", grp->gr_name);
		else
			sprintf(line[i]," %-8d", statbuf.st_gid);

		/* Print size of file. */
		sprintf(*line," %9jd", (intmax_t)statbuf.st_size);
		tm = localtime(&statbuf.st_mtime);

		/* Get localized date string. */
		strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
		sprintf(line[i]," %s %s\n", datestring, dp->d_name);

	}
	closedir(dir);
	return line;
}
