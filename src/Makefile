CC=gcc
CFLAGS=-Iinc -pthread -Werror -Wall
SRC=src/main.o \
    src/setup.o \
    src/parser.o

build: $(SRC)
	$(CC) -o server $(SRC) $(CFLAGS)

.PHONY: clean

clean:
	rm server src/*.o
