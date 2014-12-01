#include <stdio.h>
#include <time.h>
#include "mem_op.h"
#include "logger.h"
#include "helpers.h"

static FILE* logfile;

void initlogfile(struct configs* cfg)
{
    logfile = fopen(cfg->log_file,"a");
}
void filelog(int client_fd, char* cmd)
{
#define IP_ADDR_LEN 16
    char *buffer = NULL;
    char ipstr[IP_ADDR_LEN];
    char* tmp;
    int size=0;
    time_t ltime;

    ltime = time(NULL);

    memset(ipstr,0,IP_ADDR_LEN);

    get_ip_address(client_fd, ipstr);
    
    tmp = asctime(localtime(&ltime));
    tmp[strlen(tmp) - 1] = 0;

    size = strlen(cmd)+1+strlen(tmp)+IP_ADDR_LEN;

    rm_crlf(cmd);

    buffer = ftp_alloc(size) ;

    memset(buffer,0,size);

    sprintf(buffer,"|%s|%s|%s|\n",tmp,ipstr,cmd);

    fputs(buffer,logfile);

    ftp_free(buffer);
}
void closelogfile()
{
    fclose(logfile);
}
