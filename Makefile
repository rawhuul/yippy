DFLAGS=-DVERSION=\"0.6\"
CFLAGS=-g -Wall
CXXFLAGS=-O3 -std=c++11 -Wall -Wextra -DNDEBUG -fomit-frame-pointer
LDFLAGS=-lm -lstdc++
IDIR=-I include/ -I linenoise-ng/include/ -I linenoise-ng/src/

SOURCES=$(wildcard src/*.c)
TARGET=yippy

$(TARGET): linenoise-ng.o
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS) $(DFLAGS) $(IDIR) *.o

linenoise-ng.o:
	$(CXX) $(CXXFLAGS) -c linenoise-ng/src/*.cpp $(IDIR)
# ld -r ConvertUTF.o wcwidth.o linenoise.o -o linenoise-ng.o

clean:
	@rm $(TARGET) *.o
