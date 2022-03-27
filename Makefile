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

WASM_CC=./wasi-sdk/bin/clang
WASM_CXX=./wasi-sdk/bin/clang++
WASM_DFLAGS=-D_WASI_EMULATED_SIGNAL
WASM_CFLAGS=-Wall
WASM_CXXFLAGS=-O3 -std=c++11 -Wall -Wextra -DNDEBUG -fomit-frame-pointer
WASM_LDFLAGS=-lwasi-emulated-signal

wasm: linenoise-ng_wasm.o
	$(WASM_CC) $(WASM_CFLAGS) -o $@ $(SOURCES) $(LDFLAGS)  $(DFLAGS) $(IDIR) *.o

linenoise-ng_wasm.o:
	$(WASM_CXX) $(WASM_CXXFLAGS) $(WASM_DFLAGS) $(WASM_LDFLAGS) -c linenoise-ng/src/*.cpp $(IDIR)  

clean:
	@rm $(TARGET) *.o
