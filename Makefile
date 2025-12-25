CFLAGS=-Wall -Wextra -Wswitch-enum -std=c11
OS := $(shell uname)

ifeq ($(OS), Darwin)
	LIBS=-framework Cocoa -framework OpenGL
	CC=clang
else ifeq ($(OS), Linux)
	LIBS=-lX11 -lGL -lXrandr
	CC=gcc
else
	LIBS=-lopengl32 -lgdi32
	CC=gcc
endif

hohoho: main.c
	$(CC) $(CFLAGS) main.c $(LIBS) -o hohoho

clean:
	rm hohoho
