#ifndef __LOGGER_H__
#define __LOGGER_H__
#include "config_parser.h"
void initlogfile(struct configs* cfg);
void filelog(int, char* );
void closelogfile();
#endif
