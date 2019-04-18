#include "lmp.h"

#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)
#define MIN(x,y)       (x > y ? y : x)
#define MAX(x,y)       (x > y ? x : y)


#if __WORDSIZE == 64
#define POPCOUNT(x) __builtin_popcountl(x)
#define CLZ(x) __builtin_clzl(x)
#define FFS(x) __builtin_ffsl(x)

#endif

// This determines in pure C whether the machine is little endian.
// With any sufficiently smart compiler (-O2) this gets optimized away.
//
// Dump of assembler code for function is_little_endian:
//   0x0000000000000250 <+0>:     mov    $0x1,%eax
//   0x0000000000000255 <+5>:     retq
static inline int is_little_endian() {
    lmp_limb_t t = 0x01020304050607UL;
    uint8_t *p   = (uint8_t *) &t;
    lmp_limb_t q =((lmp_limb_t) p[0] <<  0)
                | ((lmp_limb_t) p[1] <<  8)
                | ((lmp_limb_t) p[2] << 16)
                | ((lmp_limb_t) p[3] << 24)
                | ((lmp_limb_t) p[4] << 32)
                | ((lmp_limb_t) p[5] << 40)
                | ((lmp_limb_t) p[6] << 48)
                | ((lmp_limb_t) p[7] << 56);
    return t == q;
}

void lmp_mul_m1(
          lmp_limb_t *const restrict rp,
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t b)
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

size_t lmp_mul_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    lmp_limb_t a = ap[an - 1];
    lmp_limb_t b = bp[bn - 1];

    return an + bn - (LMP_MSB(a) + LMP_MSB(b) + 1 < LMP_LIMB_W);
}

void lmp_mul_mn(
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
 * Shift operations
 *****************************************************************************/

size_t lmp_lshift_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bits)
{
    size_t x = bits / LMP_LIMB_W;
    size_t y = bits % LMP_LIMB_W;
    return an
        ? an + x + (LMP_LIMB_W - CLZ(ap[an - 1]) + y > LMP_LIMB_W)
        : 0;
}

void lmp_lshift(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bits)
{
    if (unlikely(!rn)) {
        return;
    }
    // Byte-wise copy is a nice optimisation that only works on little-endian
    if (!(bits % 8) && is_little_endian()) {
        memset(rp, 0, bits / 8); // check
        memcpy(((uint8_t *)rp) + bits / 8, ap, an * LMP_LIMB_S); // check
        size_t j = CLZ(ap[an - 1]); // leading 0s
        if (j) {
            rp[rn - 1];
        }
        //if (bits % LMP_LIMB_W) {
        //    rp[rn - 1] = ap[an - 1] >> (LMP_LIMB_W - bits % LMP_LIMB_W); 
        //} else {
        //    rp[rn - 1] = ap[an - 1];
        //}
        return;
    }
    // Another optimisation only applies when shift is a multiple of word size
    if (!(bits % LMP_LIMB_W)) {
        memset(rp, 0, bits / LMP_LIMB_S);
        memcpy(((uint8_t *)rp) + bits / LMP_LIMB_S, ap, an * LMP_LIMB_S);
        return;
    }

    //if (bits & 7)
    //{
    //}
    //else 
    //{
    //    memset(rp, 0, );
    //    memcpy(rp, ap + );
    //}
}

/*****************************************************************************
 * Bitwise operations 
 *****************************************************************************/

size_t lmp_ior_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    return MIN(an, bn);
}

void lmp_ior_mn(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp)
{
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
    for(size_t ri = 0;  ri < rn; ri++) {
        rp[ri] = ap[ri] & bp[ri];
    }
}

size_t lmp_testbit(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi)
{
    size_t ai = bi / LMP_LIMB_W;
    size_t wi = bi % LMP_LIMB_W;
    return ai < an && (ap[ai] & (1 << wi));
}

size_t lmp_setbit_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi)
{
    size_t ai = bi / LMP_LIMB_W;
    return MAX(an, ai + 1);
}

void lmp_setbit(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi)
{
    size_t ri = bi / LMP_LIMB_W;
    size_t wi = bi % LMP_LIMB_W;
    memcpy(rp, ap, an * LMP_LIMB_S);
    memset(rp + an, 0, (rn - an) * LMP_LIMB_S);
    rp[ri] = rp[ri] | (1 << wi);
}

size_t lmp_clearbit_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi)
{
    size_t ai = bi / LMP_LIMB_W;
    size_t wi = bi % LMP_LIMB_W;
    if (ai + 1 == an && !(ap[ai] & ~(1 << wi)))
    {
        size_t rn = an - 1;
        while (rn > 0 && !ap[rn - 1]) rn--;
        return rn;
    }
    return an;
}

void lmp_clearbit(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi)
{
    size_t ai = bi / LMP_LIMB_W;
    size_t wi = bi % LMP_LIMB_W;
    memcpy(rp, ap, rn * LMP_LIMB_S);
    if (ai < rn) rp[ai] = rp[ai] & ~(1 << wi);
}

size_t lmp_popcount(
    const lmp_limb_t *const restrict ap, const size_t an)
{
    size_t count = 0;
    for (size_t ai = 0; ai < an; ai++)
    {
        count += POPCOUNT(ap[ai]);
    }
    return count;
}



