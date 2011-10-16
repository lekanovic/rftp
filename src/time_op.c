#include <stdlib.h>
#include <time.h>
#include "err_print.h"

int cur_time_str(char* buffer)
{
	char *t;
	time_t curtime;
	struct tm *loctime;

	if( (curtime = time(NULL)) < 0){
		ERR("time\n");
		return 0;
	}

	if ((loctime = localtime(&curtime)) == NULL) {
		ERR("localtime\n");
		return 0;
	}

	if ((t=asctime(loctime)) == NULL) {
		ERR("asctime\n");
		return 0;
	}

	sprintf(buffer,"%s",asctime(loctime));

	return 1;
}
