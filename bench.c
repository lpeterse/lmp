#include <stdio.h>
#include <stdlib.h>
#include <bsdnt/nn.h>
#include <time.h>

#include "lmp.h"

#define ITERATIONS 1000
#define CYCLES(c) unsigned long long c = __builtin_readcyclecounter(); 
#define BENCH(impl, run) {\
    for (size_t i = 0; i < ITERATIONS; i++) run; \
        struct timespec td, t1, t2; \
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1); \
        for (size_t i = 0; i < ITERATIONS; i++) run; \
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2); \
        if ((t2.tv_nsec - t1.tv_nsec) < 0) { \
            td.tv_sec = t2.tv_sec - t1.tv_sec - 1; \
            td.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000; \
        } else { \
            td.tv_sec = t2.tv_sec - t1.tv_sec; \
            td.tv_nsec = t2.tv_nsec - t1.tv_nsec; \
        } \
        printf("%-20s %-20s %lld.%.9lds\n", __FUNCTION__, impl, (long long) td.tv_sec, td.tv_nsec); \
    }

static void bench_mul_mn(void)
{
    size_t an = 300;
    size_t bn = 200;
    lmp_limb_t rp[an+bn+1];
    lmp_limb_t ap[an];
    lmp_limb_t bp[bn];
    BENCH("LMP",               lmp_mul_mn      (rp, ap, an, bp, bn));
    BENCH("BSDNT (classic)",   nn_mul_classical(rp, ap, an, bp, bn));
    BENCH("BSDNT (karatsuba)", nn_mul_kara     (rp, ap, an, bp, bn));
    BENCH("BSDNT (toom-33)",   nn_mul_toom33   (rp, ap, an, bp, bn));
    BENCH("BSDNT (toom-32)",   nn_mul_toom32   (rp, ap, an, bp, bn));
}

int main()
{
    bench_mul_mn();
}