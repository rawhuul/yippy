CC=gcc
CFLAGS=-g -Wall
SRCDIR=src

yippy:	linenoise mpc
	${CC} ${CFLAGS} -o yippy ${SRCDIR}/yippy.c linenoise.o mpc.o

linenoise:
	${CC} ${CFLAGS} -c ${SRCDIR}/linenoise.c

mpc:
	${CC} ${CFLAGS} -c ${SRCDIR}/mpc.c

clean:
	rm linenoise.o yippy
