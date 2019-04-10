#include <stdio.h>
#include <stdlib.h>
#include <bsdnt/nn.h>
#include "lmp.h"

#define ITERATIONS 100000
#define CYCLES(c) unsigned long long c = __builtin_readcyclecounter(); 
#define BENCH(impl, run) {\
        for (size_t i = 0; i < ITERATIONS; i++) run; \
        CYCLES(c0); \
        for (size_t i = 0; i < ITERATIONS; i++) run; \
        CYCLES(c1); \
        printf("%-20s %-20s %lld\n", __FUNCTION__, impl, (c1 - c0) / ITERATIONS); \
    }

static void bench_mul_nm(void)
{
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI
                      , LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI
                      , LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MAX, LIMB_MIN, LIMB_MIN, LIMB_MAX, LIMB_MAX, LIMB_MAX };
    lmp_limb_t bp[] = { LIMB_MAX, LIMB_MIN, LIMB_MIN, LIMB_MAX, LIMB_MAX };
    BENCH("LMP",               lmp_mul_nm      (rp, ap, 6, bp, 5));
    BENCH("BSDNT (classic)",   nn_mul_classical(rp, ap, 6, bp, 5));
    BENCH("BSDNT (karatsuba)", nn_mul_kara     (rp, ap, 6, bp, 5));
    BENCH("BSDNT (toom-33)",   nn_mul_toom33   (rp, ap, 6, bp, 5));
    BENCH("BSDNT (toom-32)",   nn_mul_toom32   (rp, ap, 6, bp, 5));
}

int main()
{
    bench_mul_nm();
}