CC=g++
CFLAGS=-W -Wall -O3
LDFLAGS=
EXEC=strassen benchmark

all: $(EXEC)

strassen: src/strassen.o src/matrix.o
	$(CC) $(LDFLAGS) $^ -o $@

benchmark: src/benchmark.o src/matrix.o
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean distclean

clean:
	rm -rf src/*.o

distclean: clean
	rm -rf $(EXEC)