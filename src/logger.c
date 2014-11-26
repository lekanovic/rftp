#include <stdio.h>
#include <time.h>
#include "logger.h"
#include "helpers.h"

static FILE* logfile;

void initlogfile(struct configs* cfg)
{
    logfile = fopen(cfg->log_file,"a");
}
void filelog(int client_fd, char* cmd)
{
#define BUF_SIZE 100
    char buffer[BUF_SIZE];
    char ipstr[20];
    char* tmp;
    time_t ltime;

    ltime = time(NULL);
    memset(buffer,0,BUF_SIZE);
    memset(ipstr,0,20);

    get_ip_address(client_fd, ipstr);
    
    tmp = asctime(localtime(&ltime));
    tmp[strlen(tmp) - 1] = 0;

    rm_crlf(cmd);

    sprintf(buffer,"|%s|%s|%s|\n",tmp,ipstr,cmd);

    fputs(buffer,logfile);
}
void closelogfile()
{
    fclose(logfile);
}
