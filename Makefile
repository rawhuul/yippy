PROJ = yippy
CC ?= gcc
STD ?=-ansi
DFLAGS=-DVERSION=\"0.6\"
CFLAGS=-pedantic -O3 -g -Wall -Wextra -Wformat=2 -Wshadow \
  -Wno-long-long -Wno-overlength-strings -Wno-format-nonliteral -Wcast-align \
  -Wwrite-strings  -Wredundant-decls \
  -Wnested-externs -Wmissing-include-dirs 

CXXFLAGS=-O3 -std=c++11 -Wall -Wextra -DNDEBUG -fomit-frame-pointer
LDFLAGS=-lm -lstdc++
IDIR=-I include/ -I linenoise-ng/include/ -I linenoise-ng/src/ -I mpc/

SOURCES=$(wildcard src/*.c)
LINE=linenoise-ng
PARSER=mpc

$(PROJ): $(DEPENDENCY).o $(PARSER).o
	@$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS) $(DFLAGS) $(IDIR) *.o

$(DEPENDENCY).o:
	@$(CXX) $(CXXFLAGS) -c linenoise-ng/src/*.cpp $(IDIR)

$(PARSER).o:
	@$(CC) $(CFLAGS) -c mpc/*.c $(IDIR)

clean:
	@rm *.o $(PROJ)
