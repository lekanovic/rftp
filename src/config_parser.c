#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "iniparser.h"
#include "helpers.h"

#define CONFIG_FILE "config_rftp.ini"

//This was taken from: http://ndevilla.free.fr/iniparser/
int  parse_ini_file(char *,struct configs* cfg);

void create_ini_file(struct configs* cfg)
{
	char cwd[1024];
	FILE    *   ini ;

	if (file_exist(CONFIG_FILE)){
		printf("file exist %s\n",CONFIG_FILE);
		parse_ini_file(CONFIG_FILE,cfg);
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
		"ServerHomeDir = %s/publish ;"
		"\n"
        "LogFile = logfile.log ;"
		"\n"
		"\n"
		"[Settings]\n"
		"\n"
		"DisableNagle = yes ;\n"
		"AllowAnonymousLogin = yes ;\n"
		"port = 21 ;\n"
		"\n"
	,cwd);

	fclose(ini);

	parse_ini_file(CONFIG_FILE,cfg);
}


int parse_ini_file(char * ini_name,struct configs* cfg)
{
	dictionary *ini ;

	/* Some temporary variables to hold query results */
	char *home_dir;
    char *log_file;

	ini = iniparser_load(ini_name);

	if (ini==NULL) {
		fprintf(stderr, "cannot parse file: %s\n", ini_name);
		return -1 ;
	}

	iniparser_dump(ini, stderr);

	home_dir = iniparser_getstring(ini,
					"Directory:ServerHomeDir", NULL);
    log_file = iniparser_getstring(ini,
					"Directory:LogFile", NULL);
	cfg->disable_nagle_algorithm = iniparser_getboolean(ini,
					"Settings:UseNagles", -1);
	cfg->allow_anonymous_login = iniparser_getboolean(ini,
					"Settings:AllowAnonymousLogin", -1);
	cfg->port =  iniparser_getint(ini,
					"Settings:port",0);

	memset(cfg->server_dir,0,DIR_LENGTH);
	strcpy(cfg->server_dir,home_dir);

    memset(cfg->log_file,0,FILENAME_MAX);
    strcpy(cfg->log_file,log_file);

	iniparser_freedict(ini);
	return 0 ;
}


