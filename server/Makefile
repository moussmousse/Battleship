#MAKEFILE
#battleship server

# Compilers and options
CC=gcc
CPPFLAGS= -MMD
CFLAGS= -Wall -std=c99
LDFLAGS=
LDLIBS= -lm

SRC = comserv.c gestpart.c  main.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ}

-include ${DEP}

clean:
	rm -f ${OBJ} ${DEP}
	rm -f main

# END
