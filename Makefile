.PHONY: all test asm clean

CC     := clang
CFLAGS := -Wall -O2 -fno-unroll-loops

test: lmp.o test.o
	$(CC) $(CFLAGS) lmp.o test.c -o test
	./test

lmp.o: lmp.c lmp.h
	$(CC) $(CFLAGS) -c lmp.c

test.o: test.c
	$(CC) $(CFLAGS) -c test.c

asm: test
	gdb test -batch -ex 'disassemble ${FUNCTION}'

clean:
	rm -rf *.o test

