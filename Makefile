DFLAGS=-DVERSION=\"0.6\"
CFLAGS=-g -Wall -I include/
LDFLAGS=-lm

SOURCES=$(shell find src/ ! -name "*.cc" -type f)
TARGET=yippy

$(TARGET) : $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(DFLAGS)
