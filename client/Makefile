#MAKEFILE
#battleship client

# Compilers and options
CC=gcc
CPPFLAGS= -MMD
CFLAGS= -pthread
LDFLAGS=
LDLIBS= -lm

SRC =  comcli.c interface.c main.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ}

-include ${DEP}

clean:
	rm -f ${OBJ} ${DEP}
	rm -f main

# END
