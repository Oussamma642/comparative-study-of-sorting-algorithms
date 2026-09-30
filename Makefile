CC      = gcc
CFLAGS  = -O2 -Wall -Wextra -std=c11 -Iinclude
SRC     = src/main.c src/tris.c src/donnees.c
BIN     = bin/benchmark

all: $(BIN)

$(BIN): $(SRC) include/tris.h include/donnees.h
	mkdir -p bin data plots
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

run: $(BIN)
	./$(BIN) data/resultats.csv

test: $(BIN)
	./$(BIN) data/resultats.csv 10000

plot:
	gnuplot plots/tracer.gp

clean:
	rm -rf bin data/resultats.csv plots/*.png

.PHONY: all run test plot clean
