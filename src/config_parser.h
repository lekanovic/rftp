#ifndef __CONFIG_H__
#define __CONFIG_H__
struct configs
{
	int disable_nagle_algorithm;
	char* server_dir;
	int allow_anonymous_login;
	int port;
};
#define DIR_LENGTH	(1024)
void create_ini_file(struct configs* cfg);
#endif
