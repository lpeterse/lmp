.PHONY: all bench bench-noasm test dump clean

AR     := ar
CC     := gcc
CFLAGS := -Wall -std=c99 -O2 #-fno-unroll-loops

LD_LIBRARY_PATH := ./:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH

# PHONY targets

all: test bench-noasm bench

test: lmp_test.out lmp_test_noasm.out
	./lmp_test_noasm.out
	./lmp_test.out

bench: lmp_bench.out
	./$<

bench-noasm: lmp_bench_noasm.out
	./$<

dump: lmp.s
	cat $<

clean:
	rm -rf *.o *.out *.a *.so

# Intermediate targets

lmp.c: lmp.h src/*.h src/*/*.h
	touch lmp.c

lmp.s: lmp.c
	$(CC) $(CFLAGS) -S $<

lmp.o: lmp.c
	$(CC) $(CFLAGS) -c $<

lmp_noasm.o: lmp.c
	$(CC) $(CFLAGS) -DLMP_NOASM -c $< -o $@

lmp_test.c: test/*.h
	touch $@

lmp_test.out: lmp_test.c lmp.c
	$(CC) $(CFLAGS) -DLMP_ASSERT $< lmp.c -o $@

lmp_test_noasm.out: lmp_test.c lmp.c
	$(CC) $(CFLAGS) -DLMP_ASSERT -DLMP_NOASM $< lmp.c -o $@

lmp_bench.out: lmp_bench.c liblmp.so
	$(CC) $(CFLAGS) $^ -fno-unroll-loops -s -lbsdnt -lgmp -L . -llmp -o $@

lmp_bench_noasm.out: lmp_bench.c liblmp_noasm.so
	$(CC) $(CFLAGS) $^ -fno-unroll-loops -s -lbsdnt -lgmp -L . -llmp_noasm -o $@

# Libraries

liblmp.a: lmp.o
	$(AR) rcs $@ $<

liblmp_noasm.a: lmp_noasm.o
	$(AR) rcs $@ $<

liblmp.so: lmp.c
	$(CC) $(CFLAGS) -shared -fPIC $< -o $@

liblmp_noasm.so: lmp.c
	$(CC) $(CFLAGS) -shared -fPIC -DLMP_NOASM $< -o $@
