

## Makefile.am -- Process this file with automake to produce Makefile.in

bin_PROGRAMS = rftpserver

rftpserver_LDFLAGS = -pthread
rftpserver_LDADD = -lcrypt libiniparser.i686.a

rftpserver_SOURCES = \
	connect.c \
	file_op.c \
	helpers.c \
	login.c \
	main.c \
	mem_op.c \
	parser.c \
	setup.c \
	time_op.c \
	config_parser.c \
	config_parser.h \
	connect.h \
	debug_print.h \
	dictionary.h \
	err_print.h \
	file_op.h \
	ftp_msg.h \
	helpers.h \
	iniparser.h \
	login.h \
	mem_op.h \
	parser.h \
	setup.h \
	time_op.h


