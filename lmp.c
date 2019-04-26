/*

Copyright (c) 2019, Lars Petersen 
All rights reserved. 

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met: 

 * Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer. 
 * Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in the 
   documentation and/or other materials provided with the distribution. 
 * Neither the name of  nor the names of its contributors may be used to 
   endorse or promote products derived from this software without specific 
   prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE. 

 */

#include "lmp.h"

#include "stdio.h"

#define likely(x)               __builtin_expect(!!(x), 1)
#define unlikely(x)             __builtin_expect(!!(x), 0)
#define MIN(x,y)                (x > y ? y : x)
#define MAX(x,y)                (x > y ? x : y)

#ifdef LMP_ASSERT
    #include <assert.h>
    #define ASSERT(x)           assert(x)
#elif defined(__clang__)
    #define ASSERT(x)           __builtin_assume(x)
#else
    #define ASSERT(x)
#endif

#if __WORDSIZE == 64 || __WORDSIZE == 32
    #define POPCOUNT(x)         __builtin_popcountl(x)
    #define CLZ(x)              __builtin_clzl(x)
    #define ADDC(x,y,ci,co)     __builtin_addcl(x,y,ci,co)
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

/******************************************************************************
 * Compare
 *****************************************************************************/

int lmp_cmp_mn(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    if (an > bn) {
        return 1;
    }
    if (an < bn) {
        return -1;
    }
    for (size_t i = an - 1; i >= 0; i--) {
        const lmp_limb_t a = ap[i];
        const lmp_limb_t b = bp[i];
        if (a > b) {
            return 1;
        }
        if (a < b) {
            return -1;
        }
    }
    return 0;
}

/******************************************************************************
 * Addition & Subtraction
 *****************************************************************************/

size_t lmp_add_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    ASSERT(an > 0);
    ASSERT(bn > 0);

    if (bn > an) {
        return lmp_add_mn_size(bp, bn, ap, an);
    }

    size_t i = an - 1;
    for (; i >= bn; i--) {
        if (ap[i] != LMP_LIMB_MAX) {
            // Overflow impossible independant of other limbs.
            return an;
        }
        // Overflow depends on carry from less signifcant limbs. Continue...
    }
    for (; i > 0; i--) {
        lmp_limb_t a = ap[i];
        lmp_limb_t b = bp[i];
        lmp_limb_t c = a + b;
        if (c < a) {
            // Overflow inevitable independant of less significant limbs.
            return an + 1;
        }
        if (c != LMP_LIMB_MAX) {
            // Overflow impossible independant of other limbs.
            return an;
        }
        // Overflow depends on carry from less signifcant limbs. Continue...
    }
    return an + (ap[0] + bp[0] < ap[0]);
}

void lmp_add_mn(
          lmp_limb_t *const restrict rp,
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    ASSERT(an > 0);
    ASSERT(bn > 0);

    if (bn > an) {
        lmp_add_mn(rp, bp, bn, ap, an);
        return;
    }

    size_t i = 0;
    lmp_dlimb_t t = 0;
    for (; i < bn; i++) {
        t += (lmp_dlimb_t) ap[i];
        t += (lmp_dlimb_t) bp[i];
        rp[i] = (lmp_limb_t) t;
        t >>= LMP_LIMB_W;
    }
    for (; i < an && t; i++) {
        t += (lmp_dlimb_t) ap[i];
        rp[i] = (lmp_limb_t) t;
        t >>= LMP_LIMB_W;
    }
    if (t) {
        rp[an] = 1;
    } else {
        for (; i < an; i++) {
            rp[i] = ap[i];
        }
    }
}

static inline size_t lmp_diff_xx_size(
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp, const size_t n)
{
    for (size_t i = n; i > 0; i--) {
        const lmp_limb_t a = ap[i - 1];
        const lmp_limb_t b = bp[i - 1];
        if (a > b) return n + 1;
        if (a < b) return i;
    }
    return n + 1;
}

static inline size_t lmp_diff_nn_size(
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp, const size_t n)
{
    ASSERT(n > 0);
    for (size_t i = n; i > 1; i--) {
        const lmp_limb_t a = ap[i - 1];
        const lmp_limb_t b = bp[i - 1];
        if (a == b + 1) return lmp_diff_xx_size(ap, bp, i - 1);
        if (b == a + 1) return lmp_diff_xx_size(bp, ap, i - 1);
        if (a != b)     return i;
    }
    return ap[0] != bp[0];
}

size_t lmp_diff_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    ASSERT(an > 0);
    ASSERT(bn > 0);

    if (an > bn + 1) {
        return an;
    }
    if (bn > an + 1) {
        return bn;
    }
    if (an == bn + 1) {
        return ap[an - 1] > 1 ? an : lmp_diff_xx_size(ap, bp, bn);
    }
    if (bn == an + 1) {
        return bp[bn - 1] > 1 ? bn : lmp_diff_xx_size(bp, ap, an);
    }

    ASSERT(an == bn);

    return lmp_diff_nn_size(ap, bp, an);
}


/******************************************************************************
 * Multiplication
 *****************************************************************************/

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

    return an + bn - ((LMP_LIMB_W - 1 - CLZ(a)) + (LMP_LIMB_W - 1 - CLZ(b)) + 1 < LMP_LIMB_W);
}

void lmp_mul_mn(
          lmp_limb_t *const restrict rp,
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    ASSERT(an > 1);
    ASSERT(bn > 1);

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
            rp[ai + bi] = ADDC(rp[ai+bi],     (lmp_limb_t) ab, addc, &addc);
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
    ASSERT(rn > 0);
    ASSERT(an > 0);
    ASSERT(rn >= an);

    const int is_shift_by_words = !(bits % LMP_LIMB_W);
    const int is_shift_by_bytes = !(bits % 8) && is_little_endian();

    // This optimisation does a wordsize aligned memcpy and
    // is eventually vectorized.
    if (is_shift_by_words) {
        memset(rp, 0, bits / 8);
        memcpy((char*)rp + bits / 8, ap, an * LMP_LIMB_S);
        return;
    }

    // Doing a memcpy to implement the shift is only allowed if
    // the machine is little endian (due to memory representation).
    if (is_shift_by_bytes) {
        const lmp_limb_t z = ap[an - 1];
        const lmp_limb_t x = z << (bits % LMP_LIMB_W);
        const lmp_limb_t y = z >> (LMP_LIMB_W - bits % LMP_LIMB_W);
        if (y) {
            rp[rn - 2] = x;
            rp[rn - 1] = y;
        } else {
            rp[rn - 1] = x;
        }
        memset(rp, 0, bits / 8);
        memcpy((char*)rp + bits / 8, ap, (an - 1) * LMP_LIMB_S);
        return;
    }

    // Generic case:
    //   - initialise the lower limbs with 0
    //   - word-wise copy and shift from ap
    //   - the highest limb is only assigned when its non-zero
    const size_t sl = bits / LMP_LIMB_W;
    const size_t sb = bits % LMP_LIMB_W;
    for (size_t ri = 0; ri < sl; ri++) {
        rp[ri] = 0;
    }
    lmp_limb_t carry = 0;
    for (size_t ai = 0; ai < an; ai++) {
        rp[ai - sl] = (ap[ai] << sb) | carry;
        carry = ap[ai] >> (LMP_LIMB_W - sb);
    }
    if (carry) {
        rp[rn - 1] = carry;
    }
    return;
}

size_t lmp_rshift_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bits)
{
    const size_t sl = bits / LMP_LIMB_W;
    const size_t sb = bits % LMP_LIMB_W;
    return an - MIN(an, sl + !(ap[an - 1] >> sb));
}

void lmp_rshift(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bits)
{
    const int is_shift_by_words = !(bits % LMP_LIMB_W);
    const int is_shift_by_bytes = !(bits % 8) && is_little_endian();
    if (!rn) {
        // Do nothing.
    } else if (is_shift_by_words) {
        memcpy(rp, ap + bits / LMP_LIMB_W, rn * LMP_LIMB_S);
    } else if (is_shift_by_bytes) {
        const size_t len = an * LMP_LIMB_S - bits / 8 - CLZ(ap[an - 1]) / 8;
        rp[rn - 1] = 0;
        memcpy(rp, ((char*) ap) + bits / 8, len);
    } else {
        const size_t sl = bits / LMP_LIMB_W;
        const size_t sb = bits % LMP_LIMB_W;
        lmp_limb_t x = ap[sl] >> sb;
        for (size_t ai = sl + 1; ai < an; ai++) {
            rp[ai - sl - 1] = x | (ap[ai] << (LMP_LIMB_W - sb));
            x = ap[ai] >> sb;
        }
        if (x) {
            rp[rn - 1] = x;
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
    ASSERT(rn > 0);

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
