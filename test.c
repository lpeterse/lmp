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

#define ASSERT_LIMB_EQUAL(val, expected, actual) {\
        if (expected != actual) {\
            printf ("%s failed: %s:\n  expected: 0x%016lx\n  actual:   0x%016lx\n", __FUNCTION__, val, expected, actual); \
            exit(1); \
        }\
    }

static void lmp_test_mul_size(void) {
    TESTCASE(
        "multiplying 2 words with all bits set shall require 2 words",
        lmp_mul_size( (lmp_limb_t[]){ LIMB_MAX }, 1, (lmp_limb_t[]){ LIMB_MAX }, 1) == 2);
    TESTCASE(
        "multiplying 2 words with only the lower half bits set shall require 1 word",
        lmp_mul_size( (lmp_limb_t[]){ LIMB_HALF_FULL}, 1, (lmp_limb_t[]){ LIMB_HALF_FULL }, 1) == 1);
    TESTCASE(
        "multiplying 2 words (half-full and half-full + 1) shall require 2 words",
        lmp_mul_size( (lmp_limb_t[]){ LIMB_HALF_FULL}, 1, (lmp_limb_t[]){ LIMB_HALF_FULL_1 }, 1) == 2);
    TESTCASE(
        "multiplying 2 and 3 full words shall require 5 words",
        lmp_mul_size( (lmp_limb_t[]){ LIMB_MAX, LIMB_MAX }, 2, (lmp_limb_t[]){ LIMB_MAX, LIMB_MAX, LIMB_MAX }, 3) == 5);
}

static void lmp_test_mul_n1_0001(void) {
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MAX };
    lmp_mul_n1(rp, ap, 1, LIMB_MAX);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_C(0x0000000000000001), rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_C(0xFFFFFFFFFFFFFFFE), rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_INI,                 rp[2]);
}

static void lmp_test_mul_n1_0002(void) {
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_HALF_FULL };
    lmp_mul_n1(rp, ap, 1, LIMB_HALF_FULL );
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_C(0xFFFFFFFE00000001), rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_INI,                 rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_INI,                 rp[2]);
}

static void lmp_test_mul_nm_0001(void) {
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MAX, LIMB_MAX };
    lmp_limb_t bp[] = { LIMB_MAX, LIMB_MAX };
    lmp_mul_nm(rp, ap, 2, bp, 2);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MIN + 1,  rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_MIN,      rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_MAX - 1,  rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LIMB_MAX,      rp[3]);
    ASSERT_LIMB_EQUAL("rp[4]", LIMB_INI,      rp[4]);
}

static void lmp_test_mul_nm_0002(void) {
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MAX, LIMB_MIN + 1 };
    lmp_limb_t bp[] = { LIMB_MAX, LIMB_MAX };
    lmp_mul_nm(rp, ap, 2, bp, 2);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MIN + 1,  rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_MAX - 1,  rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_MAX - 1,  rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LIMB_MIN + 1,  rp[3]);
    ASSERT_LIMB_EQUAL("rp[4]", LIMB_INI,      rp[4]);
}

static void lmp_test_mul_nm_0003(void) {
    lmp_limb_t rp[] = { LIMB_INI, LIMB_INI, LIMB_INI, LIMB_INI };
    lmp_limb_t ap[] = { LIMB_MAX, LIMB_MIN + 1 };
    lmp_limb_t bp[] = { LIMB_MAX, LIMB_MIN + 1 };
    lmp_mul_nm(rp, ap, 2, bp, 2);
    ASSERT_LIMB_EQUAL("rp[0]", LIMB_MIN + 1,  rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_MAX - 3,  rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_MIN + 3,  rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LIMB_INI,      rp[3]);
}

int main(void) {
    lmp_test_mul_size();
    lmp_test_mul_n1_0001();
    lmp_test_mul_n1_0002();
    lmp_test_mul_nm_0001();
    lmp_test_mul_nm_0002();
    lmp_test_mul_nm_0003();
}
