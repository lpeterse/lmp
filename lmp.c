#include "lmp.h"

#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)
#define MIN(x,y)       (x > y ? y : x)
#define MAX(x,y)       (x > y ? x : y)

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
          lmp_limb_t *const restrict rp,
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    __builtin_assume(an > 1);
    __builtin_assume(bn > 1);

    lmp_dlimb_t ab;
    lmp_limb_t addc, mulc = 0;
    for (size_t bi = 0; bi < bn; bi++)
    {
        ab = (lmp_dlimb_t) ap[0] * bp[bi] + mulc;
        mulc = (lmp_limb_t) (ab >> LMP_LIMB_W);
        rp[bi] = (lmp_limb_t) ab;
    }
    rp[bn] = mulc;
    for (size_t ai = 1; ai < an; ai++)
    {
        addc = mulc = 0;
        for (size_t bi = 0; bi < bn; bi++)
        {
            ab = (lmp_dlimb_t) ap[ai] * bp[bi] + mulc;
            mulc = (lmp_limb_t) (ab >> LMP_LIMB_W);
            rp[ai + bi] = LMP_ADDC(rp[ai+bi], (lmp_limb_t) ab, addc, &addc);
        }
        if (ai + 1 < an || mulc || addc)
        {
            rp[ai + bn] = addc + mulc;
        }
    }
}

/*****************************************************************************
 * Bitwise operations 
 *****************************************************************************/

size_t lmp_ior_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    size_t rn = MIN(an, bn);
    while (rn > 0 && !(ap[rn - 1] | bp[rn - 1])) rn--;
    return rn;
}

void lmp_ior_mn(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp)
{
    __builtin_assume(rn > 1);

    for(size_t ri = 0;  ri < rn; ri++) {
        rp[ri] = ap[ri] | bp[ri];
    }
}

size_t lmp_xor_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    size_t rn = an;
    if (an != bn) return MAX(an, bn);
    while (rn > 0 && !(ap[rn - 1] ^ bp[rn - 1])) rn--;
    return rn;
}

void lmp_xor_mn(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp)
{
    __builtin_assume(rn > 1);

    for(size_t ri = 0;  ri < rn; ri++) {
        rp[ri] = ap[ri] ^ bp[ri];
    }
}

size_t lmp_and_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    size_t rn = MIN(an, bn);
    while (rn > 0 && !(ap[rn - 1] & bp[rn - 1])) rn--;
    return rn;
}

void lmp_and_mn(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp)
{
    __builtin_assume(rn > 1);

    for(size_t ri = 0;  ri < rn; ri++) {
        rp[ri] = ap[ri] & bp[ri];
    }
}

size_t lmp_popcount(
    const lmp_limb_t *const restrict ap, const size_t an)
{
    size_t count = 0;
    for (size_t ai = 0; ai < an; ai++)
    {
        count += __builtin_popcount(ap[ai]);
    }
    return count;
}

size_t lmp_testbit(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi)
{
    size_t ai = bi / LMP_LIMB_S;
    size_t wi = bi % LMP_LIMB_W;
    return ai < an && (ap[ai] & (1 << wi));
}
