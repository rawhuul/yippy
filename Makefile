DFLAGS=-DVERSION=\"0.6\"
CFLAGS=-g -Wall -I include/
LDFLAGS=-lm

LINENOSIE_NG=$(wildcard linenoise-ng/src/*.cpp)
SOURCES=$(wildcard src/*.c)
TARGET=yippy

$(TARGET): linenoise-ng
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS) $(DFLAGS)

linenoise-ng:
	$(CXX) $(CFLAGS) -c linenoise-ng/src/*.cpp
