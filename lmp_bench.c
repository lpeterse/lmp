/*

Copyright (c) 2019, Lars Petersen 
All rights reserved. 

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met: 

 * Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer. 
 * Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in the 
   documentation and/or other materials provided with the distribution. 
 * Neither the name of  nor the names of its contributors may be used to 
   endorse or promote products derived from this software without specific 
   prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE. 

 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <bsdnt/nn.h>
#include <gmp.h>

#include "lmp.h"

#ifdef  LMP_NOASM
#define LMP   "LMP (no asm)"
#else
#define LMP   "LMP"
#endif
#define GMP   "GMP"
#define BSDNT "BSDNT"

typedef unsigned long long cycles_t;

cycles_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((cycles_t)hi << 32) | lo;
}

#define ITERATIONS 1000
#define BENCH(c, run) {\
        cycles_t x,y; c = -1;\
        for (size_t i = 0; i < ITERATIONS; i++) { \
            x = rdtsc(); \
            run; \
            y = rdtsc(); \
            if (y - x < c) c = y - x; \
        }; \
    }
#define COMPARE(gmp, lmp, bsdnt) {\
        printf("%10lld", gmp); \
        if (lmp <= gmp && lmp <= bsdnt) { \
            printf("\x1b[32;1m%10lld\x1b[0m",lmp); \
        } else if (lmp <= gmp * 105 / 100 && lmp <= bsdnt * 105 / 100) { \
            printf("\x1b[34;1m%10lld\x1b[0m", lmp); \
        } else if (lmp <= gmp || lmp <= bsdnt) { \
            printf("\x1b[33;1m%10lld\x1b[0m", lmp); \
        } else { \
            printf("\x1b[31;1m%10lld\x1b[0m", lmp); \
        } \
        printf("%10lld\n", bsdnt); \
    }
#define ASSERT_LIMB_EQUAL(i, n1, v1, n2, v2) {\
        if ((v1) != (v2)) {\
            printf ("%s failed at limb %ld:\n  %s: 0x%016lx\n  %s: 0x%016lx\n", __func__, i, n1, v1, n2, v2); \
            exit(1); \
        }\
    }
#define ASSERT_SIZE_EQUAL(value, expected) {\
        if ((expected) != (value)) {\
            printf ("%s failed: %s:\n  expected: %ld\n  actual:   %ld\n", \
                __func__, #value, expected, value); \
            exit(1); \
        }\
    }

static void bench_cmp_mm_0001(void)
{
    printf("%-30s", __func__);
    size_t n = 3000;
    
    lmp_limb_t ap[n];
    lmp_limb_t bp[n];
    rand_t rnd;
    randinit(&rnd);
    nn_random(ap, rnd, n);
    nn_random(bp, rnd, n);
    size_t r1, r2, r3;
    cycles_t c1, c2, c3;

    BENCH(c1, {
        r1 = mpn_cmp(ap, bp, n);
    });
    BENCH(c2, {
        r2 = lmp_cmp_mm(ap, bp, n);
    });
    BENCH(c3, {
        r3 = nn_cmp_m(ap, bp, n);
    });

    COMPARE(c1, c2, c3);

    for (size_t i = 0; i < n; i++) {
        ASSERT_SIZE_EQUAL(r1, r2);
        ASSERT_SIZE_EQUAL(r1, r3);
    }
}

static void bench_cmp_mm_0002(void)
{
    printf("%-30s", __func__);
    size_t n = 3000;
    
    lmp_limb_t ap[n];
    lmp_limb_t bp[n];
    for (size_t i = 0; i < n; i++) {
        ap[i] = bp[i] = 123;
    }
    size_t r1, r2, r3;
    cycles_t c1, c2, c3;

    BENCH(c1, {
        r1 = mpn_cmp(ap, bp, n);
    });
    BENCH(c2, {
        r2 = lmp_cmp_mm(ap, bp, n);
    });
    BENCH(c3, {
        r3 = nn_cmp_m(ap, bp, n);
    });

    COMPARE(c1, c2, c3);

    for (size_t i = 0; i < n; i++) {
        ASSERT_SIZE_EQUAL(r1, r2);
        ASSERT_SIZE_EQUAL(r1, r3);
    }
}

static void bench_add_mmc_0001(void)
{
    printf("%-30s", __func__);
    size_t n = 3000;
    
    lmp_limb_t ap[n];
    lmp_limb_t bp[n];
    rand_t rnd;
    randinit(&rnd);
    nn_random(ap, rnd, n);
    nn_random(bp, rnd, n);
    lmp_limb_t rp0[n], rp1[n], rp2[n];
    cycles_t c1, c2, c3;

    BENCH(c1, {
        mpn_add(rp0, ap, n, bp, n);
    });
    BENCH(c2, {
        lmp_add_mmc(rp1, ap, bp, n, 0);
    });
    BENCH(c3, {
        nn_add_mc(rp2, ap, bp, n, 0);
    });

    COMPARE(c1, c2, c3);

    for (size_t i = 0; i < n; i++) {
        ASSERT_LIMB_EQUAL(i, GMP, rp0[i], LMP,   rp1[i]);
        ASSERT_LIMB_EQUAL(i, GMP, rp0[i], BSDNT, rp2[i]);
    }
}

static void bench_add_mn_0001(void)
{
    printf("%-30s", __func__);
    size_t an = 3000;
    size_t bn = 2500;

    lmp_limb_t ap[an];
    lmp_limb_t bp[bn];
    rand_t rnd;
    randinit(&rnd);
    nn_random(ap, rnd, an);
    nn_random(bp, rnd, bn);
    size_t rn = lmp_add_mn_size(ap, an, bp, bn);
    lmp_limb_t rp0[rn], rp1[rn], rp2[rn];
    cycles_t c1, c2, c3;

    BENCH(c1, {
        lmp_limb_t carry = mpn_add(rp0, ap, an, bp, bn);
        if (carry) rp0[rn - 1] = carry;
    });
    BENCH(c2, {
        lmp_add_mn(rp1, ap, an, bp, bn);
    });
    BENCH(c3, {
        lmp_limb_t carry = nn_add(rp2, ap, an, bp, bn);
        if (carry) rp1[rn - 1] = carry;
    });

    COMPARE(c1, c2, c3);

    for (size_t i = 0; i < rn; i++) {
        ASSERT_LIMB_EQUAL(i, GMP, rp0[i], LMP,   rp1[i]);
        ASSERT_LIMB_EQUAL(i, GMP, rp0[i], BSDNT, rp2[i]);
    }
}

static void bench_sub_mb_0001(void)
{
    printf("%-30s", __func__);
    size_t m = 3000;
    
    lmp_limb_t ap[m];
    rand_t rnd;
    randinit(&rnd);
    nn_random(ap, rnd, m);
    lmp_limb_t rp0[m], rp1[m], rp2[m];
    lmp_limb_t b = 1, b0, b1, b2;
    cycles_t c1, c2, c3;

    BENCH(c1, {
        b0 = mpn_sub_1(rp0, ap, m, b);
    });
    BENCH(c2, {
        b1 = lmp_sub_mb(rp1, ap, m, b);
    });
    BENCH(c3, {
        b2 = nn_sub1(rp2, ap, m, b);
    });

    COMPARE(c1, c2, c3);

    ASSERT_SIZE_EQUAL(b0, b1);
    ASSERT_SIZE_EQUAL(b0, b2);

    for (size_t i = 0; i < m; i++) {
        ASSERT_LIMB_EQUAL(i, GMP, rp0[i], LMP,   rp1[i]);
        ASSERT_LIMB_EQUAL(i, GMP, rp0[i], BSDNT, rp2[i]);
    }
}

static void bench_sub_mmb_0001(void)
{
    printf("%-30s", __func__);
    size_t m = 3000;
    
    lmp_limb_t ap[m], bp[m];
    rand_t rnd;
    randinit(&rnd);
    nn_random(ap, rnd, m);
    nn_random(bp, rnd, m);
    lmp_limb_t rp0[m], rp1[m], rp2[m];
    lmp_limb_t b0, b1, b2;
    cycles_t c1, c2, c3;

    BENCH(c1, {
        b0 = mpn_sub_n(rp0, ap, bp, m);
    });
    BENCH(c2, {
        b1 = lmp_sub_mmb(rp1, ap, bp, m, 0);
    });
    BENCH(c3, {
        b2 = nn_sub_mc(rp2, ap, bp, m, 0);
    });

    COMPARE(c1, c2, c3);

    for (size_t i = 0; i < m; i++) {
        ASSERT_LIMB_EQUAL(i, GMP, rp0[i], LMP,   rp1[i]);
        ASSERT_LIMB_EQUAL(i, GMP, rp0[i], BSDNT, rp2[i]);
    }

    ASSERT_SIZE_EQUAL(b0, b1);
    ASSERT_SIZE_EQUAL(b0, b2);
}

static void bench_sub_mn_0001(void)
{
    printf("%-30s", __func__);
    size_t an = 3000;
    size_t bn = 3000;

    lmp_limb_t ap[an];
    lmp_limb_t bp[bn];
    rand_t rnd;
    randinit(&rnd);
    nn_random(ap, rnd, an);
    nn_random(bp, rnd, bn);
    size_t rn = 3000; //lmp_diff_mn_size(ap, an, bp, bn);
    lmp_limb_t rp1[rn], rp2[rn], rp3[rn];
    cycles_t c1, c2, c3;

    BENCH(c1, {
        mpn_sub(rp1, ap, an, bp, bn);
        //if (carry) rp1[rn - 1] = carry;
    });
    BENCH(c2, {
        lmp_sub_mn(rp2, rn, ap, an, bp, bn);
    });
    BENCH(c3, {
        nn_sub(rp3, ap, an, bp, bn);
        //if (carry) rp1[rn - 1] = carry;
    });

    COMPARE(c1, c2, c3);

    for (size_t i = 0; i < rn - 1; i++) {
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], GMP,   rp2[i]);
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], BSDNT, rp3[i]);
    }
}

static void bench_mul_m1_0001(void)
{
    printf("%-30s", __func__);
    size_t m = 300;
    lmp_limb_t ap[m];
    lmp_limb_t b;
    rand_t rnd;
    randinit(&rnd);
    nn_random(ap, rnd, m);
    nn_random(&b, rnd, 1);
    lmp_limb_t rp1[m], rp2[m], rp3[m];
    cycles_t c1, c2, c3;

    BENCH(c1, {
        mpn_mul_1(rp1, ap, m, b);
    });
    BENCH(c2, {
        lmp_mul_m1(rp2, ap, m, b);
    });
    BENCH(c3, {
        nn_mul1(rp3, ap, m, b);
    });

    COMPARE(c1, c2, c3);

    for (size_t i = 0; i < m; i++) {
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], LMP,   rp2[i]);
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], BSDNT, rp3[i]);
    }
}

static void bench_mul_m1_add_0001(void)
{
    printf("%-30s", __func__);
    size_t m = 300;
    lmp_limb_t ap[m];
    lmp_limb_t b;
    rand_t rnd;
    randinit(&rnd);
    nn_random(ap, rnd, m);
    nn_random(&b, rnd, 1);
    lmp_limb_t rp1[m], rp2[m], rp3[m];
    cycles_t c1, c2, c3;

    BENCH(c1, {
        mpn_addmul_1(rp1, ap, m, b);
    });
    BENCH(c2, {
        lmp_mul_m1_add(rp2, ap, m, b);
    });
    BENCH(c3, {
        nn_addmul1(rp3, ap, m, b);
    });

    COMPARE(c1, c2, c3);

    for (size_t i = 0; i < m; i++) {
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], LMP,   rp2[i]);
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], BSDNT, rp3[i]);
    }
}


static void bench_mul_mn_0001(void)
{
    printf("%-30s", __func__);
    size_t an = 300;
    size_t bn = 210;

    lmp_limb_t ap[an];
    lmp_limb_t bp[bn];
    rand_t rnd;
    randinit(&rnd);
    nn_random(ap, rnd, an);
    nn_random(bp, rnd, bn);
    size_t rn = lmp_mul_mn_size(ap, an, bp, bn);
    lmp_limb_t rp1[rn + 1], rp2[rn + 1], rp3[rn + 1];
    cycles_t c1, c2, c3;

    BENCH(c1, {
        mpn_mul(rp1, ap, an, bp, bn);
    });
    BENCH(c2, {
        lmp_mul_mn(rp2, ap, an, bp, bn);
    });
    BENCH(c3, {
        nn_mul_classical(rp3, ap, an, bp, bn);
    });

    COMPARE(c1, c2, c3);

    for (size_t i = 0; i < rn; i++) {
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], LMP,   rp2[i]);
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], BSDNT, rp3[i]);
    }
}

static void bench_lshift_0001(void)
{
    printf("%-30s", __func__);
    size_t an = 300;
    size_t bits = 23;

    rand_t rnd;
    randinit(&rnd);
    lmp_limb_t ap[an];
    nn_random(ap, rnd, an);
    size_t rn = lmp_lshift_size(ap, an, bits);
    lmp_limb_t rp1[rn];
    lmp_limb_t rp2[rn];
    lmp_limb_t rp3[rn];
    cycles_t c1, c2, c3;

    BENCH(c1, {
        lmp_limb_t x = mpn_lshift(rp1, ap, an, bits);
        if (x) rp1[rn - 1] = x;
    });
    BENCH(c2, {
        lmp_lshift(rp2, rn, ap, an, bits);
    });
    BENCH(c3, {
        lmp_limb_t x = nn_shl(rp3, ap, an, bits);
        if (x) rp3[rn - 1] = x;
    });

    COMPARE(c1, c2, c3);

    for (size_t i = 0; i < rn; i++) {
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], GMP,   rp2[i]);
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], BSDNT, rp3[i]);
    }
}

static void bench_lshift_0002(void)
{
    printf("%-30s", __func__);
    size_t an = 300;
    size_t bits = 16;

    rand_t rnd;
    randinit(&rnd);
    lmp_limb_t ap[an];
    nn_random(ap, rnd, an);
    size_t rn = lmp_lshift_size(ap, an, bits);
    lmp_limb_t rp1[rn];
    lmp_limb_t rp2[rn];
    lmp_limb_t rp3[rn];
    cycles_t c1, c2, c3;

    BENCH(c1, {
        lmp_limb_t x = mpn_lshift(rp1, ap, an, bits);
        if (x) rp1[rn - 1] = x;
    });
    BENCH(c2, {
        lmp_lshift(rp2, rn, ap, an, bits);
    });
    BENCH(c3, {
        lmp_limb_t x = nn_shl(rp3, ap, an, bits);
        if (x) rp3[rn - 1] = x;
    });

    COMPARE(c1, c2, c3);

    for (size_t i = 0; i < rn; i++) {
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], GMP,   rp2[i]);
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], BSDNT, rp3[i]);
    }
}

static void bench_rshift_0001(void)
{
    printf("%-30s", __func__);
    size_t an = 300;
    size_t bits = 23;

    rand_t rnd;
    randinit(&rnd);
    lmp_limb_t ap[an];
    nn_random(ap, rnd, an);
    size_t rn = lmp_rshift_size(ap, an, bits);
    lmp_limb_t rp1[rn];
    lmp_limb_t rp2[rn];
    lmp_limb_t rp3[rn];
    cycles_t c1, c2, c3;

    BENCH(c1, {
        mpn_rshift(rp1, ap, an, bits);
    });
    BENCH(c2, {
        lmp_rshift(rp2, rn, ap, an, bits);
    });
    BENCH(c3, {
        nn_shr(rp3, ap, an, bits);
    });

    COMPARE(c1, c2, c3);

    for (size_t i = 0; i < rn; i++) {
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], LMP,   rp2[i]);
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], BSDNT, rp3[i]);
    }
}

static void bench_rshift_0002(void)
{
    printf("%-30s", __func__);
    size_t an = 300;
    size_t bits = 16;

    rand_t rnd;
    randinit(&rnd);
    lmp_limb_t ap[an];
    nn_random(ap, rnd, an);
    size_t rn = lmp_rshift_size(ap, an, bits);
    lmp_limb_t rp1[rn], rp2[rn], rp3[rn];
    cycles_t c1, c2, c3;

    BENCH(c1, {
        lmp_rshift(rp1, rn, ap, an, bits);
    });
    BENCH(c2, {
        lmp_rshift(rp2, rn, ap, an, bits);
    });
    BENCH(c3, {
        nn_shr(rp3, ap, an, bits);
    });

    COMPARE(c1, c2, c3);

    for (size_t i = 0; i < rn; i++) {
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], LMP,   rp2[i]);
        ASSERT_LIMB_EQUAL(i, GMP, rp1[i], BSDNT, rp3[i]);
    }
}

static void bench_xor_0001(void)
{
    printf("%-30s", __func__);
    size_t n = 300;

    lmp_limb_t ap[n];
    lmp_limb_t bp[n];
    lmp_limb_t rp0[n];
    lmp_limb_t rp1[n];
    lmp_limb_t rp2[n];

    rand_t rnd;
    randinit(&rnd);
    nn_random(ap, rnd, n);
    nn_random(bp, rnd, n);
    cycles_t c1, c2, c3;

    BENCH(c1, {
        mpn_xor_n(rp1, ap, bp, n);
    });
    BENCH(c2, {
        lmp_xor_mn(rp0, n, ap, bp);
    });
    BENCH(c3, {
        nn_xor_m(rp2, ap, bp, n);
    });

    COMPARE(c1, c2, c3);

    for (size_t i = 0; i < n; i++) {
        ASSERT_LIMB_EQUAL(i, LMP, rp0[i], GMP,   rp1[i]);
        ASSERT_LIMB_EQUAL(i, LMP, rp0[i], BSDNT, rp2[i]);
    }
}

static void bench_popcount_m_0001(void)
{
    printf("%-30s", __func__);
    size_t n = 3;

    lmp_limb_t ap[n];
    size_t p1,p2,p3;

    rand_t rnd;
    randinit(&rnd);
    nn_random(ap, rnd, n);
    cycles_t c1, c2, c3;

    BENCH(c1, {
        p1 = mpn_popcount(ap, n);
    });
    BENCH(c2, {
        p2 = lmp_popcount_m(ap, n);
    });
    BENCH(c3, {
        p3 = nn_popcount(ap, n);
    });

    COMPARE(c1, c2, c3);

    ASSERT_SIZE_EQUAL(p1, p2);
    ASSERT_SIZE_EQUAL(p1, p3);
}

static void bench_popcount_m_0002(void)
{
    printf("%-30s", __func__);
    size_t n = 300;

    lmp_limb_t ap[n];
    size_t p1,p2,p3;

    rand_t rnd;
    randinit(&rnd);
    nn_random(ap, rnd, n);
    cycles_t c1, c2, c3;

    BENCH(c1, {
        p1 = mpn_popcount(ap, n);
    });
    BENCH(c2, {
        p2 = lmp_popcount_m(ap, n);
    });
    BENCH(c3, {
        p3 = nn_popcount(ap, n);
    });

    COMPARE(c1, c2, c3);

    ASSERT_SIZE_EQUAL(p1, p2);
    ASSERT_SIZE_EQUAL(p1, p3);
}

int main()
{
    printf("Benchmarking %s...\n", LMP);
    printf("____________________________________________________________\n");
    printf("                                     GMP       LMP     BSDNT\n\n");

    bench_cmp_mm_0001();
    bench_cmp_mm_0002();
    bench_add_mmc_0001();
    bench_add_mn_0001();

    bench_sub_mb_0001();
    bench_sub_mmb_0001();
    bench_sub_mn_0001();
    bench_mul_m1_0001();
    bench_mul_m1_add_0001();
    bench_mul_mn_0001();
    bench_lshift_0001();
    bench_lshift_0002();
    bench_rshift_0001();
    bench_rshift_0002();
    bench_xor_0001();
    bench_popcount_m_0001();
    bench_popcount_m_0002();
}
