#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "iniparser.h"
#include "helpers.h"

#define CONFIG_FILE "config_rftp.ini"

int  parse_ini_file(char * ini_name);
/*
int main(int argc, char * argv[])
{
    int     status ;

    if (argc<2) {
        create_example_ini_file();
        status = parse_ini_file("example.ini");
    } else {
        status = parse_ini_file(argv[1]);
    }
    return status ;
}
*/
void create_ini_file(void)
{
	char cwd[1024];
	FILE    *   ini ;
	puts("create_ini_file");
	if (file_exist(CONFIG_FILE)){
		printf("file exist %s\n",CONFIG_FILE);
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
		"ServerHomeDir       = %s ;\n"
		"\n"
		"\n"
		"[Settings]\n"
		"\n"
		"UseNagles = yes ;\n"
	"\n",cwd);
	fclose(ini);
}


int parse_ini_file(char * ini_name)
{
    dictionary  *   ini ;

    /* Some temporary variables to hold query results */
    int             b ;
    int             i ;
    double          d ;
    char        *   s ;

    ini = iniparser_load(ini_name);
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: %s\n", ini_name);
        return -1 ;
    }
    iniparser_dump(ini, stderr);

    /* Get pizza attributes */
    printf("Pizza:\n");

    b = iniparser_getboolean(ini, "pizza:ham", -1);
    printf("Ham:       [%d]\n", b);
    b = iniparser_getboolean(ini, "pizza:mushrooms", -1);
    printf("Mushrooms: [%d]\n", b);
    b = iniparser_getboolean(ini, "pizza:capres", -1);
    printf("Capres:    [%d]\n", b);
    b = iniparser_getboolean(ini, "pizza:cheese", -1);
    printf("Cheese:    [%d]\n", b);

    /* Get wine attributes */
    printf("Wine:\n");
    s = iniparser_getstring(ini, "wine:grape", NULL);
    printf("Grape:     [%s]\n", s ? s : "UNDEF");
    
    i = iniparser_getint(ini, "wine:year", -1);
    printf("Year:      [%d]\n", i);

    s = iniparser_getstring(ini, "wine:country", NULL);
    printf("Country:   [%s]\n", s ? s : "UNDEF");
    
    d = iniparser_getdouble(ini, "wine:alcohol", -1.0);
    printf("Alcohol:   [%g]\n", d);

    iniparser_freedict(ini);
    return 0 ;
}


