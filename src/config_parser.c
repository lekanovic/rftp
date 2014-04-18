#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "iniparser.h"
#include "helpers.h"

#define CONFIG_FILE "config_rftp.ini"
extern int disable_nagle_algorithm;
extern char server_dir[1024];

//This was taken from: http://ndevilla.free.fr/iniparser/
int  parse_ini_file(char *);

void create_ini_file(void)
{
	char cwd[1024];
	FILE    *   ini ;

	if (file_exist(CONFIG_FILE)){
		printf("file exist %s\n",CONFIG_FILE);
		parse_ini_file(CONFIG_FILE);
		return;
	}

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		fprintf(stdout, "Current working dir: %s\n", cwd);
	else {
		perror("getcwd() error");
		return ;
	}

	ini = fopen(CONFIG_FILE, "w");
	fprintf(ini,
		"#\n"
		"# This is an config file for rftp server\n"
		"#\n"
		"\n"
		"[Directory]\n"
		"\n"
		"ServerHomeDir = %s ;\n"
		"\n"
		"\n"
		"[Settings]\n"
		"\n"
		"DisableNagle = yes ;\n"
	"\n",cwd);
	fclose(ini);
}


int parse_ini_file(char * ini_name)
{
	dictionary *ini ;

	/* Some temporary variables to hold query results */
	char *home_dir;

	ini = iniparser_load(ini_name);

	if (ini==NULL) {
		fprintf(stderr, "cannot parse file: %s\n", ini_name);
		return -1 ;
	}

	iniparser_dump(ini, stderr);

	home_dir = iniparser_getstring(ini, "Directory:ServerHomeDir", NULL);
	disable_nagle_algorithm = iniparser_getboolean(ini, "Settings:UseNagles", -1);

	memset(server_dir,0,sizeof(server_dir));
	strcpy(server_dir,home_dir);

	iniparser_freedict(ini);
	return 0 ;
}


