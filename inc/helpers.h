#ifndef __HELPERS_H__
#define __HELPERS_H__
int rm_crlf(char*);
void replace(char*);
char* replace_str(const char*, const char*, const char*);
int64_t timespecDiff(struct timespec *, struct timespec *);
#endif
