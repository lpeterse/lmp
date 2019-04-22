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

#define ITERATIONS 1000
#define BENCH(impl, run) {\
    for (size_t i = 0; i < ITERATIONS; i++) run; \
        struct timespec td, t1, t2; \
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1); \
        for (size_t i = 0; i < ITERATIONS; i++) { run; }; \
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
#define ASSERT_LIMB_EQUAL(i, n1, v1, n2, v2) {\
        if ((v1) != (v2)) {\
            printf ("%s failed at limb %ld:\n  %s: 0x%016lx\n  %s: 0x%016lx\n", __FUNCTION__, i, n1, v1, n2, v2); \
            exit(1); \
        }\
    }

static void bench_mul_mn_0001(void)
{
    size_t an = 300;
    size_t bn = 210;

    lmp_limb_t ap[an];
    lmp_limb_t bp[bn];
    rand_t rnd;
    randinit(&rnd);
    nn_random(ap, rnd, an);
    nn_random(bp, rnd, bn);
    size_t rn = lmp_mul_mn_size(ap, an, bp, bn);
    lmp_limb_t rp0[rn], rp1[rn + 1], rp2[rn + 1], rp3[rn + 1], rp4[rn + 1], rp5[rn + 1];

    BENCH("LMP", {
        lmp_mul_mn(rp0, ap, an, bp, bn);
    });
    BENCH("GMP", {
        mpn_mul(rp1, ap, an, bp, bn);
    });
    BENCH("BSDNT (best)", {
        nn_mul(rp2, ap, an, bp, bn);
    });
    BENCH("BSDNT (classic)", {
        nn_mul_classical(rp3, ap, an, bp, bn);
    });
    // Assumes bn > 1 and an >= bn >= (an + 1)/2
    assert(bn > 1);
    assert(an >= bn);
    assert(bn >= (an + 1) / 2);
    BENCH("BSDNT (karatsuba)", {
        nn_mul_kara(rp4, ap, an, bp, bn);
    });
    // Assumes bn > 2 and an >= bn > 2*((an + 2)/3)
    assert(bn > 2);
    assert(an >= bn);
    assert(bn > 2*((an + 2)/3));
    BENCH("BSDNT (toom-33)", {
        nn_mul_toom33(rp5, ap, an, bp, bn);
    });

    for (size_t i = 0; i < rn; i++) {
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT (best)",      rp1[i]);
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT (best)",      rp2[i]);
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT (classic)",   rp3[i]);
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT (karatsuba)", rp4[i]);
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT (toom-33)",   rp5[i]);
    }
}

static void bench_lshift_0001(void)
{
    size_t an = 300;
    size_t bits = 23;

    rand_t rnd;
    randinit(&rnd);
    lmp_limb_t ap[an];
    nn_random(ap, rnd, an);
    size_t rn = lmp_lshift_size(ap, an, bits);
    lmp_limb_t rp0[rn];
    lmp_limb_t rp1[rn];
    lmp_limb_t rp2[rn];

    BENCH("LMP", {
        lmp_lshift(rp0, rn, ap, an, bits);
    });
    BENCH("GMP", {
        lmp_limb_t x = mpn_lshift(rp1, ap, an, bits);
        if (x) rp1[rn - 1] = x;
    });
    BENCH("BSDNT", {
        lmp_limb_t x = nn_shl(rp2, ap, an, bits);
        if (x) rp2[rn - 1] = x;
    });

    for (size_t i = 0; i < rn; i++) {
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "GMP",   rp1[i]);
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT", rp2[i]);
    }
}

static void bench_lshift_0002(void)
{
    size_t an = 300;
    size_t bits = 16;

    rand_t rnd;
    randinit(&rnd);
    lmp_limb_t ap[an];
    nn_random(ap, rnd, an);
    size_t rn = lmp_lshift_size(ap, an, bits);
    lmp_limb_t rp0[rn];
    lmp_limb_t rp1[rn];
    lmp_limb_t rp2[rn];

    BENCH("LMP", {
        lmp_lshift(rp0, rn, ap, an, bits);
    });
    BENCH("GMP", {
        lmp_limb_t x = mpn_lshift(rp1, ap, an, bits);
        if (x) rp1[rn - 1] = x;
    });
    BENCH("BSDNT", {
        lmp_limb_t x = nn_shl(rp2, ap, an, bits);
        if (x) rp2[rn - 1] = x;
    });

    for (size_t i = 0; i < rn; i++) {
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "GMP",   rp1[i]);
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT", rp2[i]);
    }
}

static void bench_rshift_0001(void)
{
    size_t an = 300;
    size_t bits = 23;

    rand_t rnd;
    randinit(&rnd);
    lmp_limb_t ap[an];
    nn_random(ap, rnd, an);
    size_t rn = lmp_rshift_size(ap, an, bits);
    lmp_limb_t rp0[rn];
    lmp_limb_t rp1[rn];

    BENCH("LMP", {
        lmp_rshift(rp0, rn, ap, an, bits);
    });
    BENCH("BSDNT", {
        nn_shr(rp1, ap, an, bits);
    });

    for (size_t i = 0; i < rn; i++) {
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT", rp1[i]);
    }
}

static void bench_rshift_0002(void)
{
    size_t an = 300;
    size_t bits = 16;

    rand_t rnd;
    randinit(&rnd);
    lmp_limb_t ap[an];
    nn_random(ap, rnd, an);
    size_t rn = lmp_rshift_size(ap, an, bits);
    lmp_limb_t rp0[rn];
    lmp_limb_t rp1[rn];

    BENCH("LMP", {
        lmp_rshift(rp0, rn, ap, an, bits);
    });
    BENCH("BSDNT", {
        nn_shr(rp1, ap, an, bits);
    });

    for (size_t i = 0; i < rn; i++) {
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT", rp1[i]);
    }
}

static void bench_xor_0001(void)
{
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

    BENCH("LMP", {
        lmp_xor_mn(rp0, n, ap, bp);
    });
    BENCH("GMP", {
        mpn_xor_n(rp1, ap, bp, n);
    });
    BENCH("BSDNT", {
        nn_xor_m(rp2, ap, bp, n);
    });

    for (size_t i = 0; i < n; i++) {
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "GMP",   rp1[i]);
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT", rp2[i]);
    }
}

int main()
{
    bench_mul_mn_0001();
    puts("");
    bench_lshift_0001();
    puts("");
    bench_lshift_0002();
    puts("");
    bench_rshift_0001();
    puts("");
    bench_rshift_0002();
    puts("");
    bench_xor_0001();
    puts("");
}