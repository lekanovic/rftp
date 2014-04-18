CC=gcc
INC=-I../inc -I/usr/lib/gcc/x86_64-linux-gnu/4.6.1/
CFLAGS=-pthread -lcrypt -Werror -Wall -Wno-unused-but-set-variable
SRC=main.c setup.c parser.c file_op.c mem_op.c connect.c login.c time_op.c helpers.c config_parser.c

libdir.x86_64 = iniparser.x86_64
libdir.i686   = iniparser.i686

MACHINE=$(shell uname -m)

LIBDIR=$(libdir.$(MACHINE))

LFLAGS= -L../lib -l$(LIBDIR)

OBJS=$(SRC:.c=.o)

.c.o:
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

server: $(OBJS)
	$(CC) -o server $(OBJS) $(CFLAGS) $(LFLAGS) $(INC)


depend: $(SRC)
	makedepend $(INC) $^

clean:
	@find . -name '*.o'|xargs rm -f
	@rm server


# DO NOT DELETE

main.o: /usr/include/stdio.h /usr/include/features.h /usr/include/libio.h
main.o: /usr/include/_G_config.h /usr/include/wchar.h
main.o: /usr/include/netinet/in.h /usr/include/stdint.h /usr/include/endian.h
main.o: /usr/include/signal.h /usr/include/time.h /usr/include/unistd.h
main.o: /usr/include/getopt.h ../inc/setup.h
setup.o: /usr/include/stdio.h /usr/include/features.h /usr/include/libio.h
setup.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/netdb.h
setup.o: /usr/include/netinet/in.h /usr/include/stdint.h
setup.o: /usr/include/endian.h /usr/include/rpc/netdb.h
setup.o: /usr/include/arpa/inet.h /usr/include/string.h
setup.o: /usr/include/xlocale.h /usr/include/stdlib.h /usr/include/alloca.h
setup.o: /usr/include/errno.h /usr/include/unistd.h /usr/include/getopt.h
setup.o: ../inc/helpers.h ../inc/err_print.h ../inc/parser.h ../inc/connect.h
setup.o: ../inc/mem_op.h
parser.o: /usr/include/stdio.h /usr/include/features.h /usr/include/libio.h
parser.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/stdlib.h
parser.o: /usr/include/alloca.h /usr/include/string.h /usr/include/xlocale.h
parser.o: /usr/include/errno.h /usr/include/unistd.h /usr/include/getopt.h
parser.o: /usr/include/netinet/in.h /usr/include/stdint.h
parser.o: /usr/include/endian.h /usr/include/dirent.h
parser.o: /usr/include/arpa/inet.h /usr/include/fcntl.h /usr/include/time.h
parser.o: ../inc/helpers.h ../inc/parser.h ../inc/mem_op.h ../inc/file_op.h
parser.o: ../inc/err_print.h ../inc/connect.h ../inc/ftp_msg.h ../inc/login.h
file_op.o: /usr/include/dirent.h /usr/include/features.h
file_op.o: /usr/include/stdlib.h /usr/include/alloca.h /usr/include/pwd.h
file_op.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
file_op.o: /usr/include/wchar.h /usr/include/grp.h /usr/include/time.h
file_op.o: /usr/include/locale.h /usr/include/xlocale.h
file_op.o: /usr/include/langinfo.h /usr/include/nl_types.h
file_op.o: /usr/include/stdint.h /usr/include/string.h /usr/include/fcntl.h
file_op.o: /usr/include/errno.h ../inc/mem_op.h
mem_op.o: /usr/include/stdlib.h /usr/include/features.h /usr/include/alloca.h
mem_op.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
mem_op.o: /usr/include/wchar.h /usr/include/string.h /usr/include/xlocale.h
mem_op.o: /usr/include/assert.h ../inc/mem_op.h
connect.o: /usr/include/stdio.h /usr/include/features.h /usr/include/libio.h
connect.o: /usr/include/_G_config.h /usr/include/wchar.h
connect.o: /usr/include/stdlib.h /usr/include/alloca.h
connect.o: /usr/include/netinet/in.h /usr/include/stdint.h
connect.o: /usr/include/endian.h /usr/include/netinet/tcp.h
connect.o: /usr/include/unistd.h /usr/include/getopt.h /usr/include/net/if.h
connect.o: /usr/include/arpa/inet.h ../inc/connect.h ../inc/parser.h
connect.o: ../inc/err_print.h /usr/include/errno.h /usr/include/string.h
connect.o: /usr/include/xlocale.h ../inc/mem_op.h ../inc/helpers.h
login.o: /usr/include/shadow.h /usr/include/features.h /usr/include/paths.h
login.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
login.o: /usr/include/wchar.h /usr/include/pwd.h /usr/include/unistd.h
login.o: /usr/include/getopt.h /usr/include/string.h /usr/include/xlocale.h
login.o: ../inc/login.h
time_op.o: /usr/include/stdlib.h /usr/include/features.h
time_op.o: /usr/include/alloca.h /usr/include/time.h ../inc/err_print.h
time_op.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
time_op.o: /usr/include/wchar.h /usr/include/errno.h /usr/include/string.h
time_op.o: /usr/include/xlocale.h
helpers.o: /usr/include/stdio.h /usr/include/features.h /usr/include/libio.h
helpers.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/errno.h
helpers.o: /usr/include/string.h /usr/include/xlocale.h /usr/include/stdlib.h
helpers.o: /usr/include/alloca.h /usr/include/time.h /usr/include/arpa/inet.h
helpers.o: /usr/include/netinet/in.h /usr/include/stdint.h
helpers.o: /usr/include/endian.h ../inc/err_print.h
