.PHONY: all bench bench-noasm test dump clean

AR     := ar
CC     := gcc
CFLAGS := -Wall -std=c99 -O2 #-fno-unroll-loops

# PHONY targets

all: test bench

bench: lmp_bench.out
	./$<

bench-noasm: lmp_bench_noasm.out
	./$<

test: lmp_test.out lmp_test_noasm.out
	./lmp_test_noasm.out
	./lmp_test.out

dump: lmp.o
	gdb $< -batch -ex 'disassemble ${FUNCTION}'

clean:
	rm -rf *.o *.out *.a *.so

# Other targets

lmp.c: lmp.h src/*.h src/*/*.h
	touch lmp.c

lmp.s: lmp.c
	$(CC) $(CFLAGS) -S $<

lmp.o: lmp.c
	$(CC) $(CFLAGS) -c $<

lmp_noasm.o: lmp.c
	$(CC) $(CFLAGS) -DLMP_NOASM -c $< -o $@

lmp.a: lmp.o
	$(AR) rcs $@ $<

lmp.so: lmp.c
	$(CC) $(CFLAGS) -shared $< -o $@

lmp_test.c: test/*.h
	touch $@

lmp_test.out: lmp_test.c lmp.c
	$(CC) $(CFLAGS) -DLMP_ASSERT $< lmp.c -o $@

lmp_test_noasm.out: lmp_test.c lmp.c
	$(CC) $(CFLAGS) -DLMP_ASSERT -DLMP_NOASM $< lmp.c -o $@

lmp_bench.out: lmp_bench.c lmp.o
	$(CC) $(CFLAGS) $^ -fno-unroll-loops -s -lbsdnt -lgmp -o $@

lmp_bench_noasm.out: lmp_bench.c lmp_noasm.o
	$(CC) $(CFLAGS) $^ -fno-unroll-loops -s -lbsdnt -lgmp -o $@
