CC=gcc
CFLAGS=-Iinc -pthread -lcrypt -Werror -Wall -Wno-unused-but-set-variable
SRC=src/main.o \
    src/setup.o \
    src/parser.o \
    src/file_op.o \
    src/mem_op.o \
    src/connect.o \
    src/login.o \
    src/time_op.o \
    src/helpers.o

build: $(SRC)
	$(CC) -o server $(SRC) $(CFLAGS)

.PHONY: clean

clean:
	rm server src/*.o
