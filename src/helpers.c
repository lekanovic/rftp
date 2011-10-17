#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int rm_crlf(char* str)
{
	int len,ret=0;
	len = strlen(str);

	if (str[len-2] == '\r' && str[len-1] == '\n') {
		str[len-2] = str[len-1] = '\0';
		ret = 1;
	}

	return ret;
}

void replace(char*ip)
{
	int i;
	char *tmp = ip;
	for (i=0; tmp[i] != '\0';i++) {
		if( tmp[i] == '.')
			tmp[i] = ',';
	}
}
