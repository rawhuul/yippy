CC=gcc
CFLAGS=-g -Wall
SRCDIR=src

ifeq ($(OS),Windows_NT)
	CFLAGS+=-D WIN32

yippy:	eval error mpc parser types
	${CC} ${CFLAGS} -o yippy ${SRCDIR}/yippy.c eval.o error.o mpc.o parser.o types.o

else

yippy:	eval error linenoise mpc parser types
	${CC} ${CFLAGS} -o yippy ${SRCDIR}/yippy.c eval.o error.o linenoise.o mpc.o parser.o types.o

linenoise:
	${CC} ${CFLAGS} -c ${SRCDIR}/linenoise.c

endif


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

clean:
	rm linenoise.o mpc.o eval.o parser.o error.o types.o yippy
