.PHONY: asm bench test clean

AR     := ar
CC     := clang
CFLAGS := -Wall -O2 -fPIC

# PHONY targets

bench: lmp_bench.out
	./$<

test: lmp_test.out
	./$<

asm: lmp.o
	gdb $< -batch -ex 'disassemble ${FUNCTION}'

clean:
	rm -rf *.o *.out

# Other targets

lmp.o: lmp.c lmp.h
	$(CC) $(CFLAGS) -c $<

lmp.a: lmp.o
	$(AR) rcs $@ $<

lmp.so: lmp.c lmp.h
	$(CC) $(CFLAGS) -shared $< -o $@

lmp_test.out: lmp_test.c lmp.o
	$(CC) $(CFLAGS) $^ -o $@

lmp_bench.out: lmp_bench.c lmp.o
	$(CC) $(CFLAGS) $^ -lbsdnt -lgmp -o $@
