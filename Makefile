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

WASM_CC=./wasi-sdk/bin/clang
WASM_CXX=./wasi-sdk/bin/clang++
WASM_DFLAGS=-D_WASI_EMULATED_SIGNAL
WASM_CFLAGS=-Wall -nostartfiles -Wl,--import-memory -Wl,--no-entry -Wl,--export-all

wasm: src/builtins.c src/mpc.c src/types.c src/error.c src/eval.c src/wasm.c src/parser.c
	$(WASM_CC) $(WASM_CFLAGS) -o $(TARGET).$@ $^ $(DFLAGS) $(IDIR)

clean:
	@rm $(TARGET) $(TARGET).wasm *.o
