#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "lmp.h"

#define LIMB_EMPTY       LMP_LIMB_C(0x0000000000000000)
#define LIMB_FULL        LMP_LIMB_C(0xFFFFFFFFFFFFFFFF)
#define LIMB_HALF_FULL   LMP_LIMB_C(0x00000000FFFFFFFF)
#define LIMB_HALF_FULL_1 LMP_LIMB_C(0x0000000100000000)
#define LIMB_LOWEST      LMP_LIMB_C(0x0000000000000001)
#define LIMB_HIGHEST     LMP_LIMB_C(0x1000000000000000)
#define LIMB_UNTOUCHED   LMP_LIMB_C(0x2323232323232323)

#define TESTCASE(x,y) {\
        if (!(y)) {\
            printf ("Test failure in '%s': %s\n", __FUNCTION__, x); \
            exit(1);\
        }\
    }

#define ASSERT_LIMB_EQUAL(val, expected, actual) {\
        if (expected != actual) {\
            printf ("%s failed: %s:\n  expected: 0x%016llx\n  actual:   0x%016llx\n", __FUNCTION__, val, expected, actual); \
            exit(1); \
        }\
    }

static void lmp_test_mul_size(void) {
    TESTCASE(
        "multiplying 2 words with all bits set shall require 2 words",
        lmp_mul_size( (lmp_limb_t[]){ LIMB_FULL }, 1, (lmp_limb_t[]){ LIMB_FULL }, 1) == 2);
    TESTCASE(
        "multiplying 2 words with only the lower half bits set shall require 1 word",
        lmp_mul_size( (lmp_limb_t[]){ LIMB_HALF_FULL}, 1, (lmp_limb_t[]){ LIMB_HALF_FULL }, 1) == 1);
    TESTCASE(
        "multiplying 2 words (half-full and half-full + 1) shall require 2 words",
        lmp_mul_size( (lmp_limb_t[]){ LIMB_HALF_FULL}, 1, (lmp_limb_t[]){ LIMB_HALF_FULL_1 }, 1) == 2);
    TESTCASE(
        "multiplying 2 and 3 full words shall require 5 words",
        lmp_mul_size( (lmp_limb_t[]){ LIMB_FULL, LIMB_FULL }, 2, (lmp_limb_t[]){ LIMB_FULL, LIMB_FULL, LIMB_FULL }, 3) == 5);
}

static void lmp_test_mul_n1_0001(void) {
    lmp_limb_t rp[] = { LIMB_UNTOUCHED, LIMB_UNTOUCHED, LIMB_UNTOUCHED };
    lmp_limb_t ap[] = { LIMB_FULL };
    lmp_mul_n1(rp, ap, 1, LIMB_FULL);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_C(0x0000000000000001), rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_C(0xFFFFFFFFFFFFFFFE), rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_UNTOUCHED,                 rp[2]);
}

static void lmp_test_mul_n1_0002(void) {
    lmp_limb_t rp[] = { LIMB_UNTOUCHED, LIMB_UNTOUCHED, LIMB_UNTOUCHED };
    lmp_limb_t ap[] = { LIMB_HALF_FULL };
    lmp_mul_n1(rp, ap, 1, LIMB_HALF_FULL );
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_C(0xFFFFFFFE00000001), rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LIMB_UNTOUCHED,                 rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LIMB_UNTOUCHED,                 rp[2]);
}

int main(void) {
    lmp_test_mul_size();
    lmp_test_mul_n1_0001();
    lmp_test_mul_n1_0002();
}
