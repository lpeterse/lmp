#include <stdio.h>
#include <stdlib.h>
#include <bsdnt/nn.h>
#include <time.h>
#include <assert.h>

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
    lmp_limb_t rp0[rn], rp1[rn + 1], rp2[rn + 1], rp3[rn + 1], rp4[rn + 1];

    BENCH("LMP", {
        lmp_mul_mn(rp0, ap, an, bp, bn);
    });
    BENCH("BSDNT (best)", {
        nn_mul(rp1, ap, an, bp, bn);
    });
    BENCH("BSDNT (classic)", {
        nn_mul_classical(rp2, ap, an, bp, bn);
    });
    // Assumes bn > 1 and an >= bn >= (an + 1)/2
    assert(bn > 1);
    assert(an >= bn);
    assert(bn >= (an + 1) / 2);
    BENCH("BSDNT (karatsuba)", {
        nn_mul_kara(rp3, ap, an, bp, bn);
    });
    // Assumes bn > 2 and an >= bn > 2*((an + 2)/3)
    assert(bn > 2);
    assert(an >= bn);
    assert(bn > 2*((an + 2)/3));
    BENCH("BSDNT (toom-33)", {
        nn_mul_toom33(rp4, ap, an, bp, bn);
    });

    for (size_t i = 0; i < rn; i++) {
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT (best)",      rp1[i]);
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT (classic)",   rp2[i]);
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT (karatsuba)", rp3[i]);
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT (toom-33)",   rp4[i]);
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

    BENCH("LMP", {
        lmp_lshift(rp0, rn, ap, an, bits);
    });
    BENCH("BSDNT", {
        lmp_limb_t x = nn_shl(rp1, ap, an, bits);
        if (x) rp1[rn - 1] = x;
    });

    for (size_t i = 0; i < rn; i++) {
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT", rp1[i]);
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

    BENCH("LMP", {
        lmp_lshift(rp0, rn, ap, an, bits);
    });
    BENCH("BSDNT", {
        lmp_limb_t x = nn_shl(rp1, ap, an, bits);
        if (x) rp1[rn - 1] = x;
    });

    for (size_t i = 0; i < rn; i++) {
        ASSERT_LIMB_EQUAL(i, "LMP", rp0[i], "BSDNT", rp1[i]);
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
}