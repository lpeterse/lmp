.PHONY: all asm bench test clean cpuid

AR     := ar
CC     := clang
CFLAGS := -pedantic -fPIC -g -Wall -O2 -fno-unroll-loops

# PHONY targets

all: test bench

bench: lmp_bench.out
	./$<

test: lmp_test.out
	./$<

asm: lmp.o
	gdb $< -batch -ex 'disassemble ${FUNCTION}'

clean:
	rm -rf *.o *.out

cpuid: lmp_cpuid.c
	$(CC) $(CFLAGS) $< -o lmp_cpuid
	./lmp_cpuid

# Other targets

lmp.c: lmp.h

lmp.o: lmp.c
	$(CC) $(CFLAGS) -c $<

lmp.a: lmp.o
	$(AR) rcs $@ $<

lmp.so: lmp.c
	$(CC) $(CFLAGS) -shared $< -o $@

lmp_test.out: lmp_test.c lmp.c
	$(CC) $(CFLAGS) -DLMP_ASSERT $< lmp.c -o $@

lmp_bench.out: lmp_bench.c lmp.o
	$(CC) $(CFLAGS) $^ -s -lbsdnt -lgmp -o $@
