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
subSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE. 

 */

#define LMP_SUB_MN_SIZE_TESTS() { \
        lmp_sub_mn_size_test_0001(); \
        lmp_sub_mn_size_test_0002(); \
        lmp_sub_mn_size_test_0003(); \
        lmp_sub_mn_size_test_0004(); \
        lmp_sub_mn_size_test_0005(); \
        lmp_sub_mn_size_test_0006(); \
        lmp_sub_mn_size_test_0007(); \
        lmp_sub_mn_size_test_0008(); \
        lmp_sub_mn_size_test_0009(); \
        lmp_sub_mn_size_test_0010(); \
        lmp_sub_mn_size_test_0011(); \
    }

static void lmp_sub_mn_size_test_0001(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_C(1) };
    lmp_limb_t bp[] = { LMP_LIMB_C(1) };
    size_t an = SZ(ap);
    size_t bn = SZ(bp);
    size_t rn = lmp_sub_mn_size(ap, an, bp, bn);
    ASSERT_SIZE_EQUAL(rn, 0L);
}

static void lmp_sub_mn_size_test_0002(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_C(2) };
    lmp_limb_t bp[] = { LMP_LIMB_C(1) };
    size_t an = SZ(ap);
    size_t bn = SZ(bp);
    size_t rn = lmp_sub_mn_size(ap, an, bp, bn);
    ASSERT_SIZE_EQUAL(rn, 1L);
    lmp_sub_mn(rp, rn, ap, an, bp, bn);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_C(1));
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_INI);
}

static void lmp_sub_mn_size_test_0003(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_C(1), LMP_LIMB_C(1) };
    lmp_limb_t bp[] = { LMP_LIMB_C(0), LMP_LIMB_C(1) };
    size_t an = SZ(ap);
    size_t bn = SZ(bp);
    size_t rn = lmp_sub_mn_size(ap, an, bp, bn);
    ASSERT_SIZE_EQUAL(rn, 1L);
    lmp_sub_mn(rp, rn, ap, an, bp, bn);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_C(1));
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_INI);
}

static void lmp_sub_mn_size_test_0004(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_C(1), LMP_LIMB_C(2) };
    lmp_limb_t bp[] = { LMP_LIMB_C(1), LMP_LIMB_C(1) };
    size_t an = SZ(ap);
    size_t bn = SZ(bp);
    size_t rn = lmp_sub_mn_size(ap, an, bp, bn);
    ASSERT_SIZE_EQUAL(rn, 2L);
    lmp_sub_mn(rp, rn, ap, an, bp, bn);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_C(0));
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_C(1));
    ASSERT_LIMB_EQUAL(rp[2], LMP_LIMB_INI);
}

static void lmp_sub_mn_size_test_0005(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_C(2), LMP_LIMB_C(2) };
    lmp_limb_t bp[] = { LMP_LIMB_C(1), LMP_LIMB_C(1) };
    size_t an = SZ(ap);
    size_t bn = SZ(bp);
    size_t rn = lmp_sub_mn_size(ap, an, bp, bn);
    ASSERT_SIZE_EQUAL(rn, 2L);
    lmp_sub_mn(rp, rn, ap, an, bp, bn);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_C(1));
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_C(1));
    ASSERT_LIMB_EQUAL(rp[2], LMP_LIMB_INI);
}

static void lmp_sub_mn_size_test_0006(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_C(1), LMP_LIMB_C(2) };
    lmp_limb_t bp[] = { LMP_LIMB_C(2), LMP_LIMB_C(1) };
    size_t an = SZ(ap);
    size_t bn = SZ(bp);
    size_t rn = lmp_sub_mn_size(ap, an, bp, bn);
    ASSERT_SIZE_EQUAL(rn, 1L);
    lmp_sub_mn(rp, rn, ap, an, bp, bn);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_MAX);
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_INI);
}

static void lmp_sub_mn_size_test_0007(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_C(1), LMP_LIMB_C(2), LMP_LIMB_C(3) };
    lmp_limb_t bp[] = { LMP_LIMB_C(1), LMP_LIMB_C(0), LMP_LIMB_C(3) };
    size_t an = SZ(ap);
    size_t bn = SZ(bp);
    size_t rn = lmp_sub_mn_size(ap, an, bp, bn);
    ASSERT_SIZE_EQUAL(rn, 2L);
    lmp_sub_mn(rp, rn, ap, an, bp, bn);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_C(0));
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_C(2));
    ASSERT_LIMB_EQUAL(rp[2], LMP_LIMB_INI);
}

static void lmp_sub_mn_size_test_0008(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_C(1), LMP_LIMB_C(2), LMP_LIMB_C(3) };
    lmp_limb_t bp[] = { LMP_LIMB_C(1), LMP_LIMB_C(1), LMP_LIMB_C(3) };
    size_t an = SZ(ap);
    size_t bn = SZ(bp);
    size_t rn = lmp_sub_mn_size(ap, an, bp, bn);
    ASSERT_SIZE_EQUAL(rn, 2L);
    lmp_sub_mn(rp, rn, ap, an, bp, bn);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_C(0));
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_C(1));
    ASSERT_LIMB_EQUAL(rp[2], LMP_LIMB_INI);
}

static void lmp_sub_mn_size_test_0009(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_C(1), LMP_LIMB_C(2), LMP_LIMB_C(3) };
    lmp_limb_t bp[] = { LMP_LIMB_C(2), LMP_LIMB_C(1), LMP_LIMB_C(3) };
    size_t an = SZ(ap);
    size_t bn = SZ(bp);
    size_t rn = lmp_sub_mn_size(ap, an, bp, bn);
    ASSERT_SIZE_EQUAL(rn, 1L);
    lmp_sub_mn(rp, rn, ap, an, bp, bn);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_MAX);
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_INI);
}

static void lmp_sub_mn_size_test_0010(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_C(1), LMP_LIMB_C(1), LMP_LIMB_C(1), LMP_LIMB_C(1) };
    lmp_limb_t bp[] = { LMP_LIMB_C(1), LMP_LIMB_C(2), LMP_LIMB_C(0), LMP_LIMB_C(1) };
    size_t an = SZ(ap);
    size_t bn = SZ(bp);
    size_t rn = lmp_sub_mn_size(ap, an, bp, bn);
    ASSERT_SIZE_EQUAL(rn, 2L);
    lmp_sub_mn(rp, rn, ap, an, bp, bn);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_MIN);
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_MAX);
    ASSERT_LIMB_EQUAL(rp[2], LMP_LIMB_INI);
}

static void lmp_sub_mn_size_test_0011(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_C(1), LMP_LIMB_C(0), LMP_LIMB_C(0), LMP_LIMB_C(3) };
    lmp_limb_t bp[] = { LMP_LIMB_C(2) };
    size_t an = SZ(ap);
    size_t bn = SZ(bp);
    size_t rn = lmp_sub_mn_size(ap, an, bp, bn);
    ASSERT_SIZE_EQUAL(rn, 4L);
    lmp_sub_mn(rp, rn, ap, an, bp, bn);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_MAX);
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_MAX);
    ASSERT_LIMB_EQUAL(rp[2], LMP_LIMB_MAX);
    ASSERT_LIMB_EQUAL(rp[3], LMP_LIMB_C(2));
    ASSERT_LIMB_EQUAL(rp[4], LMP_LIMB_INI);
}
