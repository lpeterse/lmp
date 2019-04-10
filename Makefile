.PHONY: asm clean bench

CC     := clang
CFLAGS := -Wall -O2 -fno-unroll-loops

bench: lmp.o bench.c test
	$(CC) $(CFLAGS) bench.c lmp.o -lbsdnt -o bench
	./test
	./bench

test: lmp.o test.o
	$(CC) $(CFLAGS) lmp.o test.c -o test

lmp.o: lmp.c lmp.h
	$(CC) $(CFLAGS) -c lmp.c

test.o: test.c
	$(CC) $(CFLAGS) -c test.c

asm: test
	gdb test -batch -ex 'disassemble ${FUNCTION}'

clean:
	rm -rf *.o test

