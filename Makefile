CC=gcc
CFLAGS=-g -Wall
SRCDIR=src

yippy:	eval linenoise mpc parser 
	${CC} ${CFLAGS} -o yippy ${SRCDIR}/yippy.c eval.o linenoise.o mpc.o parser.o 

linenoise:
	${CC} ${CFLAGS} -c ${SRCDIR}/linenoise.c

mpc:
	${CC} ${CFLAGS} -c ${SRCDIR}/mpc.c

parser:
	${CC} ${CFLAGS} -c ${SRCDIR}/parser.c

eval:
	${CC} ${CFLAGS} -c ${SRCDIR}/eval.c

clean:
	rm linenoise.o mpc.o eval.o yippy
