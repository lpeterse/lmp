#include "lmp.h"

size_t lmp_mul_size(
    lmp_limb_t *ap, size_t an,
    lmp_limb_t *bp, size_t bn)
{
    lmp_limb_t a = ap[an - 1];
    lmp_limb_t b = bp[bn - 1];

    return an + bn - (LMP_MSB(a) + LMP_MSB(b) + 1 < LMP_LIMB_W);
}

void lmp_mul_n1(
    lmp_limb_t *rp,
    lmp_limb_t *ap, size_t an,
    lmp_limb_t b)
{
    // TODO: asm is different when carry is lmp_limb_t.
    // Figure out which is better.
    lmp_dlimb_t carry = 0;
    for (size_t i = 0; i < an; i++)
    {
        // Extended integer multiplication (128bit on x86_64)
        lmp_dlimb_t x = (lmp_dlimb_t) ap[i] * b + carry;
        carry = (x >> LMP_LIMB_W);
        rp[i] = (lmp_limb_t) x;
    }
    if (carry) {
        rp[an] = (lmp_limb_t) carry;
    }
}

void lmp_mul_nm(
    lmp_limb_t *rp,
    lmp_limb_t *ap, size_t an,
    lmp_limb_t *bp, size_t bn)
{
    lmp_limb_t c = 0;
    for (size_t bi = 0; bi < bn; bi++) {
        lmp_dlimb_t ab = (lmp_dlimb_t) ap[0] * bp[bi] + c;
        c = (lmp_limb_t) (ab >> LMP_LIMB_W);
        rp[bi] = (lmp_limb_t) ab;
    }
    if (an > 1) {
        rp[bn] = c;
        for (size_t ai = 1; ai < an; ai++)
        {
            lmp_limb_t addc = 0;
            lmp_limb_t mulc = 0;
            for (size_t bi = 0; bi < bn; bi++)
            {
                lmp_dlimb_t ab = (lmp_dlimb_t) ap[ai] * bp[bi] + mulc;
                mulc = (lmp_limb_t) (ab >> LMP_LIMB_W);
                rp[ai+bi] = LMP_ADDC(rp[ai+bi], (lmp_limb_t) ab, addc, &addc);
            }
            if (ai < an - 1 || addc || mulc)
            {
                rp[ai+bn] = addc + mulc;
            }
        }
    }
}
