OPENMP=1

CC=g++

ifeq ($(OPENMP), 1)
CFLAGS=-O3 -march=native -pipe -std=c++0x -W -Wall -fopenmp
LDFLAGS=-fopenmp
else
CFLAGS=-O3 -march=native -pipe -std=c++0x -W -Wall
LDFLAGS=
endif

EXEC=strassen benchmark generate

all: $(EXEC)

strassen: src/strassen.o src/matrix.o
	$(CC) $(LDFLAGS) $^ -o $@

benchmark: src/benchmark.o src/matrix.o
	$(CC) $(LDFLAGS) $^ -o $@

generate: src/generate.o src/matrix.o
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: doc clean distclean

doc:
	doxygen doxygen.conf

clean:
	rm -rf src/*.o

distclean: clean
	rm -rf $(EXEC)