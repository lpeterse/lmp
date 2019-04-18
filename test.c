#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "lmp.h"

#define TESTCASE(x,y) {\
        if (!(y)) {\
            printf ("Test failure in '%s': %s\n", __FUNCTION__, x); \
            exit(1);\
        }\
    }

#define ASSERT_SIZE_EQUAL(val, expected, actual) {\
        if (expected != actual) {\
            printf ("%s failed: %s:\n  expected: %ld\n  actual:   %ld\n", __FUNCTION__, val, expected, actual); \
            exit(1); \
        }\
    }
#define ASSERT_LIMB_EQUAL(val, expected, actual) {\
        if (expected != actual) {\
            printf ("%s failed: %s:\n  expected: 0x%016lx\n  actual:   0x%016lx\n", __FUNCTION__, val, expected, actual); \
            exit(1); \
        }\
    }

/*****************************************************************************
 * Multiplication
 *****************************************************************************/

static void lmp_test_mul_mn_size(void) {
    TESTCASE(
        "multiplying 2 words with all bits set shall require 2 words",
        lmp_mul_mn_size( (lmp_limb_t[]){ LIMB_MAX }, 1, (lmp_limb_t[]){ LIMB_MAX }, 1) == 2);
    TESTCASE(
        "multiplying 2 words with only the lower half bits set shall require 1 word",
        lmp_mul_mn_size( (lmp_limb_t[]){ LIMB_HALF_FULL}, 1, (lmp_limb_t[]){ LIMB_HALF_FULL }, 1) == 1);
    TESTCASE(
        "multiplying 2 words (half-full and half-full + 1) shall require 2 words",
        lmp_mul_mn_size( (lmp_limb_t[]){ LIMB_HALF_FULL}, 1, (lmp_limb_t[]){ LIMB_HALF_FULL_1 }, 1) == 2);
    TESTCASE(
        "multiplying 2 and 3 full words shall require 5 words",
        lmp_mul_mn_size( (lmp_limb_t[]){ LIMB_MAX, LIMB_MAX }, 2, (lmp_limb_t[]){ LIMB_MAX, LIMB_MAX, LIMB_MAX }, 3) == 5);
}

static void lmp_test_mul_m1_0001(void)
{
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MAX };
    lmp_mul_m1(rp, ap, 1, LIMB_MAX);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_C(0x0000000000000001), rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_C(0xFFFFFFFFFFFFFFFE), rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_INI,                 rp[2]);
}

static void lmp_test_mul_m1_0002(void)
{
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_HALF_FULL };
    lmp_mul_m1(rp, ap, 1, LIMB_HALF_FULL );
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_C(0xFFFFFFFE00000001), rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_INI,                 rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_INI,                 rp[2]);
}

static void lmp_test_mul_mn_0001(void)
{
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MAX, LIMB_MAX };
    lmp_limb_t bp[] = { LIMB_MAX, LIMB_MAX };
    lmp_mul_mn(rp, ap, 2, bp, 2);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MIN + 1,  rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_MIN,      rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_MAX - 1,  rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LIMB_MAX,      rp[3]);
    ASSERT_LIMB_EQUAL("rp[4]", LIMB_INI,      rp[4]);
}

static void lmp_test_mul_mn_0002(void)
{
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MAX, LIMB_MIN + 1 };
    lmp_limb_t bp[] = { LIMB_MAX, LIMB_MAX };
    lmp_mul_mn(rp, ap, 2, bp, 2);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MIN + 1,  rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_MAX - 1,  rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_MAX - 1,  rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LIMB_MIN + 1,  rp[3]);
    ASSERT_LIMB_EQUAL("rp[4]", LIMB_INI,      rp[4]);
}

static void lmp_test_mul_mn_0003(void)
{
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MAX, LIMB_MIN + 1 };
    lmp_limb_t bp[] = { LIMB_MAX, LIMB_MIN + 1 };
    lmp_mul_mn(rp, ap, 2, bp, 2);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MIN + 1,  rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_MAX - 3,  rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_MIN + 3,  rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LIMB_INI,      rp[3]);
}

/*****************************************************************************
 * Bitwise operations 
 *****************************************************************************/

static void lmp_test_lshift_size_0001(void)
{
    size_t result = lmp_lshift_size(NULL, 0L, 0L);
    ASSERT_SIZE_EQUAL("result", 0L, result);
}

static void lmp_test_lshift_size_0002(void)
{
    lmp_limb_t ap[] = { LIMB_MIN + 1 };
    size_t result = lmp_lshift_size(ap, 1L, 0L);
    ASSERT_SIZE_EQUAL("result", 1L, result);
}

static void lmp_test_lshift_size_0003(void)
{
    lmp_limb_t ap[] = { LIMB_MIN + 2 };
    size_t result = lmp_lshift_size(ap, 1L, LMP_LIMB_W - 2);
    ASSERT_SIZE_EQUAL("result", 1L, result);
}

static void lmp_test_lshift_size_0004(void)
{
    lmp_limb_t ap[] = { LIMB_MIN + 2 };
    size_t result = lmp_lshift_size(ap, 1L, LMP_LIMB_W - 1);
    ASSERT_SIZE_EQUAL("result", 2L, result);
}

static void lmp_test_lshift_size_0005(void)
{
    lmp_limb_t ap[] = { LIMB_MIN, LIMB_MAX };
    size_t result = lmp_lshift_size(ap, 2L, LMP_LIMB_W * 2 + 1);
    ASSERT_SIZE_EQUAL("result", 5L, result);
}

static void lmp_test_lshift_size_0006(void)
{
    lmp_limb_t ap[] = { LIMB_MIN, LIMB_MAX };
    size_t result = lmp_lshift_size(ap, 2L, LMP_LIMB_W * 2);
    ASSERT_SIZE_EQUAL("result", 4L, result);
}

static void lmp_test_lshift_0001(void)
{
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MIN + 1, LIMB_MIN + 2, LIMB_MIN + 3 };
    lmp_lshift(rp, 4, ap, 3, LMP_LIMB_W);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MIN,      rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_MIN + 1,  rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_MIN + 2,  rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LIMB_MIN + 3,  rp[3]);
    ASSERT_LIMB_EQUAL("rp[4]", LIMB_INI,      rp[4]);
}

static void lmp_test_lshift_0002(void)
{
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MAX, LIMB_MAX, LIMB_MAX >> 8 };
    lmp_lshift(rp, 4, ap, 3, 16);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MAX << 16,               rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_MAX,                     rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_MAX,                     rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LIMB_MAX >> (LMP_LIMB_W - 8), rp[3]);
    ASSERT_LIMB_EQUAL("rp[4]", LIMB_INI,                     rp[4]);
}

static void lmp_test_lshift_0003(void)
{
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MAX >> 8 };
    lmp_lshift(rp, 1, ap, 1, 8);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MAX << 8,                rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_INI,                     rp[1]);
}

/*****************************************************************************
 * Bitwise operations 
 *****************************************************************************/

static void lmp_test_ior_mn_size_0001(void)
{
    lmp_limb_t ap[] = {};
    lmp_limb_t bp[] = {};
    size_t result = lmp_ior_mn_size(ap, 0L, bp, 0L);
    ASSERT_SIZE_EQUAL("result", 0L, result);
}

static void lmp_test_ior_mn_size_0002(void)
{
    lmp_limb_t ap[] = { LIMB_MIN + 1 };
    lmp_limb_t bp[] = { LIMB_MIN + 1, LIMB_MIN + 1 };
    size_t result = lmp_ior_mn_size(ap, 1L, bp, 2L);
    ASSERT_SIZE_EQUAL("result", 1L, result);
}

static void lmp_test_ior_mn_size_0003(void)
{
    lmp_limb_t ap[] = { LIMB_MIN, LIMB_MIN, LIMB_MIN + 1};
    lmp_limb_t bp[] = { LIMB_MIN, LIMB_MIN + 1 };
    size_t result = lmp_ior_mn_size(ap, 3L, bp, 2L);
    ASSERT_SIZE_EQUAL("result", 2L, result);
}

static void lmp_test_ior_mn_0001(void)
{
    lmp_limb_t rp[] = { LIMB_INI,     LIMB_INI,     LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MIN,     LIMB_MIN + 1, LIMB_MAX };
    lmp_limb_t bp[] = { LIMB_MIN + 4, LIMB_MIN + 2, LIMB_MAX };
    lmp_ior_mn(rp, 2, ap, bp);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MIN + 4, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_MIN + 3, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_INI,     rp[2]);
}

static void lmp_test_xor_mn_size_0001(void)
{
    lmp_limb_t ap[] = {};
    lmp_limb_t bp[] = {};
    size_t result = lmp_xor_mn_size(ap, 0L, bp, 0L);
    ASSERT_SIZE_EQUAL("result", 0L, result);
}

static void lmp_test_xor_mn_size_0002(void)
{
    lmp_limb_t ap[] = { LIMB_MIN,     LIMB_MAX, LIMB_MIN + 2, LIMB_MIN + 3 };
    lmp_limb_t bp[] = { LIMB_MIN + 1, LIMB_MAX, LIMB_MIN + 2, LIMB_MIN + 3 };
    size_t result = lmp_xor_mn_size(ap, 4L, bp, 4L);
    ASSERT_SIZE_EQUAL("result", 1L, result);
}

static void lmp_test_xor_mn_size_0003(void)
{
    lmp_limb_t ap[] = { LIMB_MIN, LIMB_MIN, LIMB_MIN };
    lmp_limb_t bp[] = { LIMB_MIN, LIMB_MIN, LIMB_MIN, LIMB_MIN };
    size_t result = lmp_xor_mn_size(ap, 3L, bp, 4L);
    ASSERT_SIZE_EQUAL("result", 4L, result);
}

static void lmp_test_xor_mn_0001(void)
{
    lmp_limb_t rp[] = { LIMB_INI,     LIMB_INI,     LIMB_INI     };
    lmp_limb_t ap[] = { LIMB_MAX,     LIMB_MIN + 1, LIMB_MIN + 1 };
    lmp_limb_t bp[] = { LIMB_MAX - 1, LIMB_MIN + 3, LIMB_MIN + 2 };
    lmp_xor_mn(rp, 2, ap, bp);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MIN + 1, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_MIN + 2, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_INI,     rp[2]);
}

static void lmp_test_and_mn_size_0001(void)
{
    lmp_limb_t ap[] = {};
    lmp_limb_t bp[] = {};
    size_t result = lmp_and_mn_size(ap, 0L, bp, 0L);
    ASSERT_SIZE_EQUAL("result", 0L, result);
}

static void lmp_test_and_mn_size_0002(void)
{
    lmp_limb_t ap[] = { LIMB_MIN,     LIMB_MAX, LIMB_MIN + 2, LIMB_MIN + 3 };
    lmp_limb_t bp[] = { LIMB_MIN + 1, LIMB_MAX, LIMB_MIN + 2, LIMB_MIN + 3 };
    size_t result = lmp_xor_mn_size(ap, 4L, bp, 4L);
    ASSERT_SIZE_EQUAL("result", 1L, result);
}

static void lmp_test_and_mn_0001(void)
{
    lmp_limb_t rp[] = { LIMB_INI,     LIMB_INI,     LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MAX,     LIMB_MIN + 1, LIMB_MIN };
    lmp_limb_t bp[] = { LIMB_MAX - 1, LIMB_MIN + 3, LIMB_MIN };
    lmp_and_mn(rp, 2, ap, bp);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MAX - 1, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_MIN + 1, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_INI,     rp[2]);
}

static void lmp_test_testbit_0001(void)
{
    lmp_limb_t ap[] = {};
    size_t result = lmp_testbit(ap, 0L, 123L);
    ASSERT_SIZE_EQUAL("result", 0L, result);
}

static void lmp_test_testbit_0002(void)
{
    lmp_limb_t ap[] = { LIMB_MIN, LIMB_MIN };
    size_t result = lmp_testbit(ap, 2L, LMP_LIMB_W + 1 );
    ASSERT_SIZE_EQUAL("result", 0L, result);
}

static void lmp_test_testbit_0003(void)
{
    lmp_limb_t ap[] = { LIMB_MIN, LIMB_MIN + 2 };
    size_t result = lmp_testbit(ap, 2L, LMP_LIMB_W + 1 );
    ASSERT_SIZE_EQUAL("result", 1L, result);
}

static void lmp_test_setbit_size_0001(void)
{
    size_t result = lmp_setbit_size(NULL, 2L, LMP_LIMB_W * 0 );
    ASSERT_SIZE_EQUAL("result", 2L, result);
}

static void lmp_test_setbit_size_0002(void)
{
    size_t result = lmp_setbit_size(NULL, 2L, LMP_LIMB_W * 2 );
    ASSERT_SIZE_EQUAL("result", 3L, result);
}

static void lmp_test_setbit_0001(void)
{
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI, LIMB_INI };
    lmp_setbit(rp, 2L, NULL, 0L, LMP_LIMB_W + 1);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MIN,     rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_MIN + 2, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_INI,     rp[2]);
}

static void lmp_test_setbit_0002(void)
{
    lmp_limb_t rp[] = { LIMB_INI,     LIMB_INI,     LIMB_INI,     LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MIN + 1, LIMB_MIN + 4, LIMB_MIN + 1};
    lmp_setbit(rp, 3L, ap, 3L, LMP_LIMB_W + 1);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MIN + 1, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_MIN + 6, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_MIN + 1, rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LIMB_INI,     rp[3]);
}

static void lmp_test_clearbit_size_0001(void)
{
    size_t result = lmp_clearbit_size(NULL, 2L, LMP_LIMB_W * 0 );
    ASSERT_SIZE_EQUAL("result", 2L, result);
}

static void lmp_test_clearbit_size_0002(void)
{
    size_t result = lmp_clearbit_size(NULL, 2L, LMP_LIMB_W * 2 );
    ASSERT_SIZE_EQUAL("result", 2L, result);
}

static void lmp_test_clearbit_size_0003(void)
{
    lmp_limb_t ap[] = { LIMB_MIN + 1, LIMB_MIN, LIMB_MIN + 2 };
    size_t result = lmp_clearbit_size(ap, 3L, LMP_LIMB_W * 3 );
    ASSERT_SIZE_EQUAL("result", 3L, result);
}

static void lmp_test_clearbit_size_0004(void)
{
    lmp_limb_t ap[] = { LIMB_MIN + 1, LIMB_MIN, LIMB_MIN + 2 };
    size_t result = lmp_clearbit_size(ap, 3L, LMP_LIMB_W * 2 );
    ASSERT_SIZE_EQUAL("result", 3L, result);
}

static void lmp_test_clearbit_size_0005(void)
{
    lmp_limb_t ap[] = { LIMB_MIN + 1, LIMB_MIN, LIMB_MIN + 2 };
    size_t result = lmp_clearbit_size(ap, 3L, LMP_LIMB_W * 2 + 1 );
    ASSERT_SIZE_EQUAL("result", 1L, result);
}

static void lmp_test_clearbit_size_0006(void)
{
    lmp_limb_t ap[] = { LIMB_MIN + 1, LIMB_MIN, LIMB_MIN + 2 };
    size_t result = lmp_clearbit_size(ap, 3L, LMP_LIMB_W );
    ASSERT_SIZE_EQUAL("result", 3L, result);
}

static void lmp_test_clearbit_0001(void)
{
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MIN, LIMB_MIN + 2 };
    lmp_clearbit(rp, 0L, ap, 2L, LMP_LIMB_W + 1);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_INI, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_INI, rp[1]);
}

static void lmp_test_clearbit_0002(void)
{
    lmp_limb_t rp[] = { LIMB_INI,     LIMB_INI,     LIMB_INI,      LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MAX - 8, LIMB_MAX - 1, LIMB_MAX - 4 };
    lmp_clearbit(rp, 3L, ap, 3L, LMP_LIMB_W + 1);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MAX - 8, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_MAX - 3, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_MAX - 4, rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LIMB_INI,     rp[3]);
}

static void lmp_test_popcount_0001(void)
{
    lmp_limb_t ap[] = { LIMB_MAX };
    size_t result = lmp_popcount(ap, 1L);
    ASSERT_SIZE_EQUAL("result", (size_t) LMP_LIMB_W, result);
}

static void lmp_test_popcount_0002(void)
{
    lmp_limb_t ap[] = { LIMB_MAX, LIMB_MIN + 3 };
    size_t result = lmp_popcount(ap, 2L);
    ASSERT_SIZE_EQUAL("result", (size_t) LMP_LIMB_W + 2, result);
}

/*****************************************************************************
 * main
 *****************************************************************************/

int main(void) {
    lmp_test_mul_mn_size();
    lmp_test_mul_m1_0001();
    lmp_test_mul_m1_0002();
    lmp_test_mul_mn_0001();
    lmp_test_mul_mn_0002();
    lmp_test_mul_mn_0003();

    lmp_test_lshift_size_0001();
    lmp_test_lshift_size_0002();
    lmp_test_lshift_size_0003();
    lmp_test_lshift_size_0004();
    lmp_test_lshift_size_0005();
    lmp_test_lshift_size_0006();
    lmp_test_lshift_0001();
    lmp_test_lshift_0002();
    lmp_test_lshift_0003();

    lmp_test_ior_mn_size_0001();
    lmp_test_ior_mn_size_0002();
    lmp_test_ior_mn_size_0003();
    lmp_test_ior_mn_0001();

    lmp_test_xor_mn_size_0001();
    lmp_test_xor_mn_size_0002();
    lmp_test_xor_mn_size_0003();
    lmp_test_xor_mn_0001();

    lmp_test_and_mn_size_0001();
    lmp_test_and_mn_size_0002();
    lmp_test_and_mn_0001();

    lmp_test_testbit_0001();
    lmp_test_testbit_0002();
    lmp_test_testbit_0003();

    lmp_test_setbit_size_0001();
    lmp_test_setbit_size_0002();
    lmp_test_setbit_0001();
    lmp_test_setbit_0002();

    lmp_test_clearbit_size_0001();
    lmp_test_clearbit_size_0002();
    lmp_test_clearbit_size_0003();
    lmp_test_clearbit_size_0004();
    lmp_test_clearbit_size_0005();
    lmp_test_clearbit_size_0006();
    lmp_test_clearbit_0001();
    lmp_test_clearbit_0002();

    lmp_test_popcount_0001();
    lmp_test_popcount_0002();
}
