CC=gcc
CFLAGS=-g -Wall
SRCDIR=src

# $(info Compiling for $(OS))

ifeq ($(OS),Windows_NT)
	CFLAGS+=-D WIN32

yippy:	builtins eval error mpc parser types
	${CC} ${CFLAGS} -o yippy ${SRCDIR}/yippy.c builtins.o eval.o error.o mpc.o parser.o types.o


clean:
	rm builtins.o mpc.o eval.o parser.o error.o types.o yippy

else

yippy:	builtins eval error linenoise mpc parser types
	${CC} ${CFLAGS} -o yippy ${SRCDIR}/yippy.c builtins.o eval.o error.o linenoise.o mpc.o parser.o types.o

linenoise:
	${CC} ${CFLAGS} -c ${SRCDIR}/linenoise.c


clean:
	rm builtins.o linenoise.o mpc.o eval.o parser.o error.o types.o yippy

endif


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
