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

#define LMP_LSHIFT_TESTS() { \
        lmp_lshift_test_0001(); \
        lmp_lshift_test_0002(); \
        lmp_lshift_test_0003(); \
        lmp_lshift_test_0004(); \
        lmp_lshift_test_0005(); \
        lmp_lshift_test_0006(); \
        lmp_lshift_test_0007(); \
        lmp_lshift_test_0008(); \
        lmp_lshift_test_0009(); \
        lmp_lshift_test_0010(); \
        lmp_lshift_test_0011(); \
        lmp_lshift_test_0012(); \
    }

/* Shift 1 by 0 bits. */
static void lmp_lshift_test_0001(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    lmp_lshift(rp, 1, ap, 1, 0);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_MIN + 1);
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_INI);
}

/* Shift 1 by 1 bit. */
static void lmp_lshift_test_0002(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    lmp_lshift(rp, 1, ap, 1, 1);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_MIN + 2);
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_INI);
}

/* Shift 1 by 8 bits. */
static void lmp_lshift_test_0003(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    lmp_lshift(rp, 1, ap, 1, 8);
    ASSERT_LIMB_EQUAL(rp[0], (LMP_LIMB_MIN + 1) << 8);
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_INI);
}

/* Shift 1 by WORDSIZE - 1 bits. */
static void lmp_lshift_test_0004(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    lmp_lshift(rp, 2, ap, 1, LMP_LIMB_W - 1);
    ASSERT_LIMB_EQUAL(rp[0], (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1));
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_INI);
}

/* Shift 1 by WORDSIZE bits. */
static void lmp_lshift_test_0005(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    lmp_lshift(rp, 2, ap, 1, LMP_LIMB_W);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_MIN);
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_MIN + 1);
    ASSERT_LIMB_EQUAL(rp[2], LMP_LIMB_INI);
}

/* Shift 1 by WORDSIZE + 1 bits. */
static void lmp_lshift_test_0006(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    lmp_lshift(rp, 2, ap, 1, LMP_LIMB_W + 1);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_MIN);
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_MIN + 2);
    ASSERT_LIMB_EQUAL(rp[2], LMP_LIMB_INI);
}

/* Shift 1 << (WORDSIZE - 1) by 1 bit. */
static void lmp_lshift_test_0007(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1) };
    lmp_lshift(rp, 2, ap, 1, 1);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_MIN);
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_MIN + 1);
    ASSERT_LIMB_EQUAL(rp[2], LMP_LIMB_INI);
}

/* Shift 1 << (WORDSIZE - 1) by 8 bits. */
static void lmp_lshift_test_0008(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1), LMP_LIMB_INI };
    lmp_lshift(rp, 2, ap, 1, 8);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_MIN);
    ASSERT_LIMB_EQUAL(rp[1], (LMP_LIMB_MIN + 1) << 7);
    ASSERT_LIMB_EQUAL(rp[2], LMP_LIMB_INI);
}

/* Shift 1 << (WORDSIZE - 1) by WORDSIZE bits. */
static void lmp_lshift_test_0009(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1) };
    lmp_lshift(rp, 2, ap, 1, LMP_LIMB_W);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_MIN);
    ASSERT_LIMB_EQUAL(rp[1], (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1));
    ASSERT_LIMB_EQUAL(rp[2], LMP_LIMB_INI);
}

/* Shift 1 << (WORDSIZE - 1) by WORDSIZE + 1 bits. */
static void lmp_lshift_test_0010(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1) };
    lmp_lshift(rp, 3, ap, 1, LMP_LIMB_W + 1);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_MIN);
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_MIN);
    ASSERT_LIMB_EQUAL(rp[2], LMP_LIMB_MIN + 1);
    ASSERT_LIMB_EQUAL(rp[3], LMP_LIMB_INI);
}

/* Shift several words by 16 bits. */
static void lmp_lshift_test_0011(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MAX, LMP_LIMB_MAX >> 8 };
    lmp_lshift(rp, 4, ap, 3, 16);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_MAX << 16);
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_MAX);
    ASSERT_LIMB_EQUAL(rp[2], LMP_LIMB_MAX);
    ASSERT_LIMB_EQUAL(rp[3], LMP_LIMB_MAX >> (LMP_LIMB_W - 8));
    ASSERT_LIMB_EQUAL(rp[4], LMP_LIMB_INI);
}

/* Shift several words by WORDSIZE bits. */
static void lmp_lshift_test_0012(void)
{
    lmp_limb_t rp[] = RP;
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1, LMP_LIMB_MIN + 2, LMP_LIMB_MIN + 3 };
    lmp_lshift(rp, 4, ap, 3, LMP_LIMB_W);
    ASSERT_LIMB_EQUAL(rp[0], LMP_LIMB_MIN);
    ASSERT_LIMB_EQUAL(rp[1], LMP_LIMB_MIN + 1);
    ASSERT_LIMB_EQUAL(rp[2], LMP_LIMB_MIN + 2);
    ASSERT_LIMB_EQUAL(rp[3], LMP_LIMB_MIN + 3);
    ASSERT_LIMB_EQUAL(rp[4], LMP_LIMB_INI);
}
