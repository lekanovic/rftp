#ifndef __CONFIG_H__
#define __CONFIG_H__
struct configs
{
	int disable_nagle_algorithm;
	char* server_dir;
	int allow_anonymous_login;
};
void create_ini_file(struct configs* cfg);
#endif
