#include "lmp.h"

size_t lmp_mul_size(
    lmp_limb_t *ap, size_t an,
    lmp_limb_t *bp, size_t bn
) {
    lmp_limb_t a = ap[an - 1];
    lmp_limb_t b = bp[bn - 1];
    ASSERT(a);
    ASSERT(b);
    return an + bn - (LMP_MSB(a) + LMP_MSB(b) + 1 < LMP_LIMB_W);
}

void lmp_mul_n1(
    lmp_limb_t *rp,
    lmp_limb_t *ap, size_t an,
    lmp_limb_t b
) {
    lmp_dlimb_t carry = 0;
    for (size_t i = 0; i < an; i++)
    {
        lmp_dlimb_t x = (lmp_dlimb_t) ap[i] * b + carry;
        carry = (x >> LMP_LIMB_W);
        rp[i] = (lmp_limb_t) x;
    }
    if (carry) {
        rp[an] = (lmp_limb_t) carry;
    }
}

