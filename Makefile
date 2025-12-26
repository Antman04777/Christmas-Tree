CFLAGS=-Wall -Wextra -Wswitch-enum -std=c11
OS := $(shell uname)

ifeq ($(OS), Darwin)
	CC=clang
else ifeq ($(OS), Linux)
	CC=gcc
else
	CC=gcc
endif

hohoho: main.c
	$(CC) $(CFLAGS) main.c -o hohoho

clean:
	rm hohoho
