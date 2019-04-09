#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "lmp.h"

#define LIMB_EMPTY       0x0000000000000000ULL
#define LIMB_FULL        0xFFFFFFFFFFFFFFFFULL
#define LIMB_HALF_FULL   0x00000000FFFFFFFFULL
#define LIMB_HALF_FULL_1 0x0000000100000000ULL
#define LIMB_LOWEST      0x0000000000000001ULL
#define LIMB_HIGHEST     0x1000000000000000ULL

#define TESTCASE(x,y) {\
        if (!(y)) {\
            printf ("Test failure in '%s': %s\n", __FUNCTION__, x); \
            exit(1);\
        }\
    }

static void lmp_test_mul_size(void) {
    TESTCASE(
        "multiplying two words with all bits set shall require 2 words",
        lmp_mul_size( (lmp_limb_t[]){ LIMB_FULL }, 1, (lmp_limb_t[]){ LIMB_FULL }, 1) == 2);
    TESTCASE(
        "multiplying two words with only the lower half bits set shall require 1 word",
        lmp_mul_size( (lmp_limb_t[]){ LIMB_HALF_FULL}, 1, (lmp_limb_t[]){ LIMB_HALF_FULL }, 1) == 1);
    TESTCASE(
        "multiplying two words (half-full and half-full + 1) shall require 2 words",
        lmp_mul_size( (lmp_limb_t[]){ LIMB_HALF_FULL}, 1, (lmp_limb_t[]){ LIMB_HALF_FULL_1 }, 1) == 2);
}

int main(void) {
    lmp_test_mul_size();
}
