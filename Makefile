version=\"0.3\"

CC=gcc
CFLAGS=-g -Wall -DVERSION=${version} -lm
SRCDIR=src

all: main

ifeq ($(OS),Windows_NT)
	CFLAGS+=-D WIN32

main:	builtins eval error mpc parser types yippy
	${CC} ${CFLAGS} -o yippy ${SRCDIR}/main.c yippy.o builtins.o eval.o error.o mpc.o parser.o types.o

clean:
	rm builtins.o mpc.o eval.o parser.o error.o types.o yippy.o yippy

else

main:	builtins eval error linenoise mpc parser types yippy
	${CC} ${CFLAGS} -o yippy ${SRCDIR}/main.c builtins.o eval.o error.o linenoise.o mpc.o parser.o types.o yippy.o

linenoise:
	${CC} ${CFLAGS} -c ${SRCDIR}/linenoise.c

clean:
	rm builtins.o linenoise.o mpc.o eval.o parser.o error.o types.o yippy.o yippy

endif

yippy:
	${CC} ${CFLAGS} -c ${SRCDIR}/yippy.c

builtins:
	${CC} ${CFLAGS} -c ${SRCDIR}/builtins.c

mpc:
	${CC} ${CFLAGS} -c ${SRCDIR}/mpc.c

parser:
	${CC} ${CFLAGS} -c ${SRCDIR}/parser.c

eval:
	${CC} ${CFLAGS} -c ${SRCDIR}/eval.c

error:
	${CC} ${CFLAGS} -c ${SRCDIR}/error.c

types:
	${CC} ${CFLAGS} -c ${SRCDIR}/types.c
