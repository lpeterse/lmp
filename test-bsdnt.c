#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <bsdnt/nn.h>
#include <bsdnt/rand.h>

#include "lmp.h"

#define ASSERT_LIMB_EQUAL(ti, tn, idx, lmp, bsdnt) {\
        if (lmp != bsdnt) {\
            printf ("%s failed after %ld of %ld tests: diff at index %ld:\n  lmp:   0x%016lx\n  bsdnt: 0x%016lx\n", __FUNCTION__, ti, tn, idx, lmp, bsdnt); \
            exit(1); \
        }\
    }

static void lmp_test_mul_mn_0001(void) {
    rand_t rnd;
    randinit(&rnd);
    const size_t tn = 100000;
    const size_t an = 137;
    const size_t bn = 53;
    const size_t rn = an + bn + 1;
    for (size_t ti = 0; ti < tn; ti++) {
        lmp_limb_t *rp = malloc(rn * LMP_LIMB_S);
        lmp_limb_t *sp = malloc(rn * LMP_LIMB_S);
        lmp_limb_t *ap = malloc(an * LMP_LIMB_S);
        lmp_limb_t *bp = malloc(bn * LMP_LIMB_S);
        nn_random(ap, rnd, an);
        nn_random(bp, rnd, bn);
        if (randword(rnd) % 2) {
            ap[an-1] &= LIMB_HALF_FULL;
            bp[bn-1] &= LIMB_HALF_FULL;
        }
        for (size_t ri = 0; ri < rn; ri++) {
            rp[ri] = sp[ri] = LIMB_INI;
        }
        lmp_mul_mn(rp, ap, an, bp, bn);
        nn_mul_classical(sp, ap, an, bp, bn);
        if (!sp[rn-2]) {
            sp[rn-2] = LMP_LIMB_S;
        }
        for (size_t ri = 0; ri < rn; ri++) {
            ASSERT_LIMB_EQUAL(ti, tn, ri, rp[ri], sp[ri]);
        }
    }
}

int main(void) {
    //lmp_test_mul_nm_0001();
}
