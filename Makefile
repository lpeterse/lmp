.PHONY: asm clean bench

CC     := clang
CFLAGS := -Wall -O2

bench: lmp.o bench.c test test-bsdnt
	$(CC) $(CFLAGS) bench.c lmp.o -lbsdnt -o bench
	./test
	./test-bsdnt
	./bench

test: lmp.o test.o
	$(CC) $(CFLAGS) lmp.o test.c -o test

test-bsdnt: lmp.o test-bsdnt.o
	$(CC) $(CFLAGS) lmp.o test-bsdnt.c -lbsdnt -o test-bsdnt

lmp.o: lmp.c lmp.h
	$(CC) $(CFLAGS) -c lmp.c

test.o: test.c
	$(CC) $(CFLAGS) -c test.c

asm: lmp.o
	gdb lmp.o -batch -ex 'disassemble ${FUNCTION}'

clean:
	rm -rf *.o test

