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
diffSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE. 

 */

#include <stdio.h>
#include <stdlib.h>

#include "lmp.h"

#define TESTCASE(x,y) {\
        if (!(y)) {\
            printf ("Test failure in '%s': %s\n", __FUNCTION__, x); \
            exit(1);\
        }\
    }

#define ASSERT_SIZE_EQUAL(val, expected, actual) {\
        if ((expected) != (actual)) {\
            printf ("%s failed: %s:\n  expected: %ld\n  actual:   %ld\n", __FUNCTION__, val, expected, actual); \
            exit(1); \
        }\
    }
#define ASSERT_LIMB_EQUAL(val, expected, actual) {\
        if ((expected) != (actual)) {\
            printf ("%s failed: %s:\n  expected: 0x%016lx\n  actual:   0x%016lx\n", __FUNCTION__, val, expected, actual); \
            exit(1); \
        }\
    }

/*****************************************************************************
 * Addition & difftraction
 *****************************************************************************/

static void lmp_test_diff_mn_size_0001(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_C(1) };
    lmp_limb_t bp[] = { LMP_LIMB_C(1) };
    ASSERT_SIZE_EQUAL("rn", 0L, lmp_diff_mn_size(ap, 1, bp, 1));
}

static void lmp_test_diff_mn_size_0002(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_C(2) };
    lmp_limb_t bp[] = { LMP_LIMB_C(1) };
    ASSERT_SIZE_EQUAL("rn", 1L, lmp_diff_mn_size(ap, 1, bp, 1));
    ASSERT_SIZE_EQUAL("qn", 1L, lmp_diff_mn_size(bp, 1, ap, 1));
}

static void lmp_test_diff_mn_size_0003(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_C(1), LMP_LIMB_C(1) };
    lmp_limb_t bp[] = { LMP_LIMB_C(0), LMP_LIMB_C(1) };
    ASSERT_SIZE_EQUAL("rn", 1L, lmp_diff_mn_size(ap, 2, bp, 2));
    ASSERT_SIZE_EQUAL("qn", 1L, lmp_diff_mn_size(bp, 2, ap, 2));
}

static void lmp_test_diff_mn_size_0004(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_C(1), LMP_LIMB_C(2) };
    lmp_limb_t bp[] = { LMP_LIMB_C(1), LMP_LIMB_C(1) };
    ASSERT_SIZE_EQUAL("rn", 2L, lmp_diff_mn_size(ap, 2, bp, 2));
    ASSERT_SIZE_EQUAL("qn", 2L, lmp_diff_mn_size(bp, 2, ap, 2));
}

static void lmp_test_diff_mn_size_0005(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_C(2), LMP_LIMB_C(2) };
    lmp_limb_t bp[] = { LMP_LIMB_C(1), LMP_LIMB_C(1) };
    ASSERT_SIZE_EQUAL("rn", 2L, lmp_diff_mn_size(ap, 2, bp, 2));
}

static void lmp_test_diff_mn_size_0006(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_C(1), LMP_LIMB_C(2) };
    lmp_limb_t bp[] = { LMP_LIMB_C(2), LMP_LIMB_C(1) };
    ASSERT_SIZE_EQUAL("rn", 1L, lmp_diff_mn_size(ap, 2, bp, 2));
    ASSERT_SIZE_EQUAL("qn", 1L, lmp_diff_mn_size(bp, 2, ap, 2));
}

static void lmp_test_diff_mn_size_0007(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_C(1), LMP_LIMB_C(2), LMP_LIMB_C(3) };
    lmp_limb_t bp[] = { LMP_LIMB_C(1), LMP_LIMB_C(0), LMP_LIMB_C(3) };
    ASSERT_SIZE_EQUAL("rn", 2L, lmp_diff_mn_size(ap, 3, bp, 3));
    ASSERT_SIZE_EQUAL("qn", 2L, lmp_diff_mn_size(bp, 3, ap, 3));
}

static void lmp_test_diff_mn_size_0008(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_C(1), LMP_LIMB_C(2), LMP_LIMB_C(3) };
    lmp_limb_t bp[] = { LMP_LIMB_C(1), LMP_LIMB_C(1), LMP_LIMB_C(3) };
    ASSERT_SIZE_EQUAL("rn", 2L, lmp_diff_mn_size(ap, 3, bp, 3));
    ASSERT_SIZE_EQUAL("qn", 2L, lmp_diff_mn_size(bp, 3, ap, 3));
}

static void lmp_test_diff_mn_size_0009(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_C(1), LMP_LIMB_C(2), LMP_LIMB_C(3) };
    lmp_limb_t bp[] = { LMP_LIMB_C(2), LMP_LIMB_C(1), LMP_LIMB_C(3) };
    ASSERT_SIZE_EQUAL("rn", 1L, lmp_diff_mn_size(ap, 3, bp, 3));
    ASSERT_SIZE_EQUAL("qn", 1L, lmp_diff_mn_size(bp, 3, ap, 3));
}


/*****************************************************************************
 * Multiplication
 *****************************************************************************/

static void lmp_test_mul_mn_0001(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MAX };
    lmp_limb_t bp[] = { LMP_LIMB_MAX, LMP_LIMB_MAX };
    lmp_mul_mn(rp, ap, 2, bp, 2);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN + 1,  rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MIN,      rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_MAX - 1,  rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_MAX,      rp[3]);
    ASSERT_LIMB_EQUAL("rp[4]", LMP_LIMB_INI,      rp[4]);
}

static void lmp_test_mul_mn_0002(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MIN + 1 };
    lmp_limb_t bp[] = { LMP_LIMB_MAX, LMP_LIMB_MAX };
    lmp_mul_mn(rp, ap, 2, bp, 2);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN + 1,  rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MAX - 1,  rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_MAX - 1,  rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_MIN + 1,  rp[3]);
    ASSERT_LIMB_EQUAL("rp[4]", LMP_LIMB_INI,      rp[4]);
}

static void lmp_test_mul_mn_0003(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MIN + 1 };
    lmp_limb_t bp[] = { LMP_LIMB_MAX, LMP_LIMB_MIN + 1 };
    lmp_mul_mn(rp, ap, 2, bp, 2);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN + 1,  rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MAX - 3,  rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_MIN + 3,  rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_INI,      rp[3]);
}

/*****************************************************************************
 * Bitwise operations 
 *****************************************************************************/

/* Shift 1 by 0 bits. */
static void lmp_test_lshift_size_0001(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    size_t rn = lmp_lshift_size(ap, 1, 0);
    ASSERT_SIZE_EQUAL("rn", 1L, rn);
}

/* Shift 1 by 1 bit. */
static void lmp_test_lshift_size_0002(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    size_t rn = lmp_lshift_size(ap, 1, 1);
    ASSERT_SIZE_EQUAL("rn", 1L, rn);
}

/* Shift 1 by 8 bit. */
static void lmp_test_lshift_size_0003(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    size_t rn = lmp_lshift_size(ap, 1, 8);
    ASSERT_SIZE_EQUAL("rn", 1L, rn);
}

/* Shift 1 by WORDSIZE - 1 bits. */
static void lmp_test_lshift_size_0004(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    size_t rn = lmp_lshift_size(ap, 1, LMP_LIMB_W - 1);
    ASSERT_SIZE_EQUAL("rn", 1L, rn);
}

/* Shift 1 by WORDSIZE bits. */
static void lmp_test_lshift_size_0005(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1, LMP_LIMB_MIN };
    size_t rn = lmp_lshift_size(ap, 1, LMP_LIMB_W);
    ASSERT_SIZE_EQUAL("rn", 2L, rn);
}

/* Shift 1 by WORDSIZE  + 1 bits. */
static void lmp_test_lshift_size_0006(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1, LMP_LIMB_MIN };
    size_t rn = lmp_lshift_size(ap, 1, LMP_LIMB_W + 1);
    ASSERT_SIZE_EQUAL("rn", 2L, rn);
}

/* Shift 1 << (WORDSIZE - 1) by 1 bit. */
static void lmp_test_lshift_size_0007(void)
{
    lmp_limb_t ap[] = { (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1), LMP_LIMB_MIN };
    size_t rn = lmp_lshift_size(ap, 1, 1);
    ASSERT_SIZE_EQUAL("rn", 2L, rn);
}

/* Shift 1 << (WORDSIZE - 1) by 8 bits. */
static void lmp_test_lshift_size_0008(void)
{
    lmp_limb_t ap[] = { (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1), LMP_LIMB_MIN };
    size_t rn = lmp_lshift_size(ap, 1, 8);
    ASSERT_SIZE_EQUAL("rn", 2L, rn);
}

/* Shift 1 << (WORDSIZE - 1) by WORDSIZE bits. */
static void lmp_test_lshift_size_0009(void)
{
    lmp_limb_t ap[] = { (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1), LMP_LIMB_MIN };
    size_t rn = lmp_lshift_size(ap, 1, LMP_LIMB_W);
    ASSERT_SIZE_EQUAL("rn", 2L, rn);
}

/* Shift 1 << (WORDSIZE - 1) by WORDSIZE + 1 bits. */
static void lmp_test_lshift_size_0010(void)
{
    lmp_limb_t ap[] = { (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1), LMP_LIMB_MIN };
    size_t rn = lmp_lshift_size(ap, 1, LMP_LIMB_W + 1);
    ASSERT_SIZE_EQUAL("rn", 3L, rn);
}

/* Shift several words by 16 bits. */
static void lmp_test_lshift_size_0011(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MAX, LMP_LIMB_MAX >> 8 };
    size_t rn = lmp_lshift_size(ap, 3, 16);
    ASSERT_SIZE_EQUAL("rn", 4L, rn);
}

/* Shift 1 by 0 bits. */
static void lmp_test_lshift_0001(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    lmp_lshift(rp, 1, ap, 1, 0);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN + 1, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_INI,     rp[1]);
}

/* Shift 1 by 1 bit. */
static void lmp_test_lshift_0002(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    lmp_lshift(rp, 1, ap, 1, 1);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN + 2, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_INI,     rp[1]);
}

/* Shift 1 by 8 bits. */
static void lmp_test_lshift_0003(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    lmp_lshift(rp, 1, ap, 1, 8);
    ASSERT_LIMB_EQUAL("rp[0]", (LMP_LIMB_MIN + 1) << 8, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_INI,            rp[1]);
}

/* Shift 1 by WORDSIZE - 1 bits. */
static void lmp_test_lshift_0004(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    lmp_lshift(rp, 2, ap, 1, LMP_LIMB_W - 1);
    ASSERT_LIMB_EQUAL("rp[0]", (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1), rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_INI,                           rp[1]);
}

/* Shift 1 by WORDSIZE bits. */
static void lmp_test_lshift_0005(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    lmp_lshift(rp, 2, ap, 1, LMP_LIMB_W);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN,     rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MIN + 1, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_INI,     rp[2]);
}

/* Shift 1 by WORDSIZE + 1 bits. */
static void lmp_test_lshift_0006(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    lmp_lshift(rp, 2, ap, 1, LMP_LIMB_W + 1);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN,     rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MIN + 2, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_INI,     rp[2]);
}

/* Shift 1 << (WORDSIZE - 1) by 1 bit. */
static void lmp_test_lshift_0007(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1) };
    lmp_lshift(rp, 2, ap, 1, 1);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN,     rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MIN + 1, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_INI,     rp[2]);
}

/* Shift 1 << (WORDSIZE - 1) by 8 bits. */
static void lmp_test_lshift_0008(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1), LMP_LIMB_INI };
    lmp_lshift(rp, 2, ap, 1, 8);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN,            rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", (LMP_LIMB_MIN + 1) << 7, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_INI,            rp[2]);
}

/* Shift 1 << (WORDSIZE - 1) by WORDSIZE bits. */
static void lmp_test_lshift_0009(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1) };
    lmp_lshift(rp, 2, ap, 1, LMP_LIMB_W);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN,                           rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1), rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_INI,                           rp[2]);
}

/* Shift 1 << (WORDSIZE - 1) by WORDSIZE + 1 bits. */
static void lmp_test_lshift_0010(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { (LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1) };
    lmp_lshift(rp, 3, ap, 1, LMP_LIMB_W + 1);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN,     rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MIN,     rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_MIN + 1, rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_INI,     rp[3]);
}

/* Shift several words by 16 bits. */
static void lmp_test_lshift_0011(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MAX, LMP_LIMB_MAX >> 8 };
    lmp_lshift(rp, 4, ap, 3, 16);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MAX << 16,               rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MAX,                     rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_MAX,                     rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_MAX >> (LMP_LIMB_W - 8), rp[3]);
    ASSERT_LIMB_EQUAL("rp[4]", LMP_LIMB_INI,                     rp[4]);
}

/* Shift several words by WORDSIZE bits. */
static void lmp_test_lshift_0012(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1, LMP_LIMB_MIN + 2, LMP_LIMB_MIN + 3 };
    lmp_lshift(rp, 4, ap, 3, LMP_LIMB_W);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN,     rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MIN + 1, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_MIN + 2, rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_MIN + 3, rp[3]);
    ASSERT_LIMB_EQUAL("rp[4]", LMP_LIMB_INI,     rp[4]);
}

/* Shift 1 by 0 bits. */
static void lmp_test_rshift_0001(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    size_t rn = lmp_rshift_size(ap, 1, 0);
    lmp_rshift(rp, rn, ap, 1, 0);
    ASSERT_SIZE_EQUAL("rn", 1L, rn);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN + 1, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_INI,     rp[1]);
}

/* Shift 1 by 1 bit. */
static void lmp_test_rshift_0002(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    size_t rn = lmp_rshift_size(ap, 1, 1);
    lmp_rshift(rp, rn, ap, 1, 1);
    ASSERT_SIZE_EQUAL("rn", 0L, rn);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_INI, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_INI, rp[1]);
}

/* Shift 3 words by 0 bits. */
static void lmp_test_rshift_0003(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MAX, (LMP_LIMB_MIN + 1) << 16 };
    size_t rn = lmp_rshift_size(ap, 3, 0);
    lmp_rshift(rp, rn, ap, 3, 0);
    ASSERT_SIZE_EQUAL("rn", 3L, rn);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MAX,             rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MAX,             rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", (LMP_LIMB_MIN + 1) << 16, rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_INI,             rp[3]);
}

/* Shift 3 words by 1 bit. */
static void lmp_test_rshift_0004(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MAX, (LMP_LIMB_MIN + 1) << 16 };
    size_t rn = lmp_rshift_size(ap, 3, 1);
    lmp_rshift(rp, rn, ap, 3, 1);
    ASSERT_SIZE_EQUAL("rn", 3L, rn);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MAX,             rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MAX >> 1,        rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", (LMP_LIMB_MIN + 1) << 15, rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_INI,             rp[3]);
}

/* Shift 3 words by 15 bits. */
static void lmp_test_rshift_0005(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MAX, (LMP_LIMB_MIN + 1) << 16 };
    size_t rn = lmp_rshift_size(ap, 3, 15);
    lmp_rshift(rp, rn, ap, 3, 15);
    ASSERT_SIZE_EQUAL("rn", 3L, rn);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MAX,             rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MAX >> 15,       rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", (LMP_LIMB_MIN + 1) << 1,  rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_INI,             rp[3]);
}

/* Shift 3 words by 16 bits. */
static void lmp_test_rshift_0006(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MAX, (LMP_LIMB_MIN + 1) << 16 };
    size_t rn = lmp_rshift_size(ap, 3, 16);
    lmp_rshift(rp, rn, ap, 3, 16);
    ASSERT_SIZE_EQUAL("rn", 3L, rn);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MAX,             rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MAX >> 16,       rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_MIN + 1,         rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_INI,             rp[3]);
}

/* Shift 3 words by 17 bits. */
static void lmp_test_rshift_0007(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MAX, (LMP_LIMB_MIN + 1) << 16 };
    size_t rn = lmp_rshift_size(ap, 3, 17);
    lmp_rshift(rp, rn, ap, 3, 17);
    ASSERT_SIZE_EQUAL("rn", 2L, rn);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MAX,             rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", (LMP_LIMB_MAX >> 17) | ((LMP_LIMB_MIN + 1) << (LMP_LIMB_W - 1)), rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_INI,             rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_INI,             rp[3]);
}

/* Shift 3 words by WORDSIZE - 1 bits. */
static void lmp_test_rshift_0008(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MAX, (LMP_LIMB_MIN + 1) << 16 };
    size_t rn = lmp_rshift_size(ap, 3, LMP_LIMB_W - 1);
    lmp_rshift(rp, rn, ap, 3, LMP_LIMB_W - 1);
    ASSERT_SIZE_EQUAL("rn", 2L, rn);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MAX,             rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", ((LMP_LIMB_MIN + 1) << 17) + 1, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_INI,             rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_INI,             rp[3]);
}

/* Shift 3 words by WORDSIZE bits. */
static void lmp_test_rshift_0009(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MAX, (LMP_LIMB_MIN + 1) << 16 };
    size_t rn = lmp_rshift_size(ap, 3, LMP_LIMB_W);
    lmp_rshift(rp, rn, ap, 3, LMP_LIMB_W);
    ASSERT_SIZE_EQUAL("rn", 2L, rn);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MAX,             rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", (LMP_LIMB_MIN + 1) << 16, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_INI,             rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_INI,             rp[3]);
}

/* Shift 3 words by WORDSIZE + 1 bits. */
static void lmp_test_rshift_0010(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MAX, (LMP_LIMB_MIN + 1) << 16 };
    size_t rn = lmp_rshift_size(ap, 3, LMP_LIMB_W + 1);
    lmp_rshift(rp, rn, ap, 3, LMP_LIMB_W + 1);
    ASSERT_SIZE_EQUAL("rn", 2L, rn);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MAX >> 1,        rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", (LMP_LIMB_MIN + 1) << 15, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_INI,             rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_INI,             rp[3]);
}

/*****************************************************************************
 * Bitwise operations 
 *****************************************************************************/

static void lmp_test_ior_mn_size_0001(void)
{
    lmp_limb_t ap[] = {};
    lmp_limb_t bp[] = {};
    size_t rn = lmp_ior_mn_size(ap, 0L, bp, 0L);
    ASSERT_SIZE_EQUAL("rn", 0L, rn);
}

static void lmp_test_ior_mn_size_0002(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1 };
    lmp_limb_t bp[] = { LMP_LIMB_MIN + 1, LMP_LIMB_MIN + 1 };
    size_t rn = lmp_ior_mn_size(ap, 1L, bp, 2L);
    ASSERT_SIZE_EQUAL("rn", 1L, rn);
}

static void lmp_test_ior_mn_size_0003(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN, LMP_LIMB_MIN, LMP_LIMB_MIN + 1};
    lmp_limb_t bp[] = { LMP_LIMB_MIN, LMP_LIMB_MIN + 1 };
    size_t rn = lmp_ior_mn_size(ap, 3L, bp, 2L);
    ASSERT_SIZE_EQUAL("rn", 2L, rn);
}

static void lmp_test_ior_mn_0001(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI,     LMP_LIMB_INI,     LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MIN,     LMP_LIMB_MIN + 1, LMP_LIMB_MAX };
    lmp_limb_t bp[] = { LMP_LIMB_MIN + 4, LMP_LIMB_MIN + 2, LMP_LIMB_MAX };
    lmp_ior_mn(rp, 2, ap, bp);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN + 4, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MIN + 3, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_INI,     rp[2]);
}

static void lmp_test_xor_mn_size_0001(void)
{
    lmp_limb_t ap[] = {};
    lmp_limb_t bp[] = {};
    size_t rn = lmp_xor_mn_size(ap, 0L, bp, 0L);
    ASSERT_SIZE_EQUAL("rn", 0L, rn);
}

static void lmp_test_xor_mn_size_0002(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN,     LMP_LIMB_MAX, LMP_LIMB_MIN + 2, LMP_LIMB_MIN + 3 };
    lmp_limb_t bp[] = { LMP_LIMB_MIN + 1, LMP_LIMB_MAX, LMP_LIMB_MIN + 2, LMP_LIMB_MIN + 3 };
    size_t rn = lmp_xor_mn_size(ap, 4L, bp, 4L);
    ASSERT_SIZE_EQUAL("rn", 1L, rn);
}

static void lmp_test_xor_mn_size_0003(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN, LMP_LIMB_MIN, LMP_LIMB_MIN };
    lmp_limb_t bp[] = { LMP_LIMB_MIN, LMP_LIMB_MIN, LMP_LIMB_MIN, LMP_LIMB_MIN };
    size_t rn = lmp_xor_mn_size(ap, 3L, bp, 4L);
    ASSERT_SIZE_EQUAL("rn", 4L, rn);
}

static void lmp_test_xor_mn_0001(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI,     LMP_LIMB_INI,     LMP_LIMB_INI     };
    lmp_limb_t ap[] = { LMP_LIMB_MAX,     LMP_LIMB_MIN + 1, LMP_LIMB_MIN + 1 };
    lmp_limb_t bp[] = { LMP_LIMB_MAX - 1, LMP_LIMB_MIN + 3, LMP_LIMB_MIN + 2 };
    lmp_xor_mn(rp, 2, ap, bp);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN + 1, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MIN + 2, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_INI,     rp[2]);
}

static void lmp_test_and_mn_size_0001(void)
{
    lmp_limb_t ap[] = {};
    lmp_limb_t bp[] = {};
    size_t rn = lmp_and_mn_size(ap, 0L, bp, 0L);
    ASSERT_SIZE_EQUAL("rn", 0L, rn);
}

static void lmp_test_and_mn_size_0002(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN,     LMP_LIMB_MAX, LMP_LIMB_MIN + 2, LMP_LIMB_MIN + 3 };
    lmp_limb_t bp[] = { LMP_LIMB_MIN + 1, LMP_LIMB_MAX, LMP_LIMB_MIN + 2, LMP_LIMB_MIN + 3 };
    size_t rn = lmp_xor_mn_size(ap, 4L, bp, 4L);
    ASSERT_SIZE_EQUAL("rn", 1L, rn);
}

static void lmp_test_and_mn_0001(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI,     LMP_LIMB_INI,     LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MAX,     LMP_LIMB_MIN + 1, LMP_LIMB_MIN };
    lmp_limb_t bp[] = { LMP_LIMB_MAX - 1, LMP_LIMB_MIN + 3, LMP_LIMB_MIN };
    lmp_and_mn(rp, 2, ap, bp);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MAX - 1, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MIN + 1, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_INI,     rp[2]);
}

static void lmp_test_testbit_0001(void)
{
    lmp_limb_t ap[] = {};
    size_t rn = lmp_testbit(ap, 0L, 123L);
    ASSERT_SIZE_EQUAL("rn", 0L, rn);
}

static void lmp_test_testbit_0002(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN, LMP_LIMB_MIN };
    size_t rn = lmp_testbit(ap, 2L, LMP_LIMB_W + 1 );
    ASSERT_SIZE_EQUAL("rn", 0L, rn);
}

static void lmp_test_testbit_0003(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN, LMP_LIMB_MIN + 2 };
    size_t rn = lmp_testbit(ap, 2L, LMP_LIMB_W + 1 );
    ASSERT_SIZE_EQUAL("rn", 1L, rn);
}

static void lmp_test_setbit_size_0001(void)
{
    size_t rn = lmp_setbit_size(NULL, 2L, LMP_LIMB_W * 0 );
    ASSERT_SIZE_EQUAL("rn", 2L, rn);
}

static void lmp_test_setbit_size_0002(void)
{
    size_t rn = lmp_setbit_size(NULL, 2L, LMP_LIMB_W * 2 );
    ASSERT_SIZE_EQUAL("rn", 3L, rn);
}

static void lmp_test_setbit_0001(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_setbit(rp, 2L, NULL, 0L, LMP_LIMB_W + 1);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN,     rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MIN + 2, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_INI,     rp[2]);
}

static void lmp_test_setbit_0002(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI,     LMP_LIMB_INI,     LMP_LIMB_INI,     LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1, LMP_LIMB_MIN + 4, LMP_LIMB_MIN + 1};
    lmp_setbit(rp, 3L, ap, 3L, LMP_LIMB_W + 1);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MIN + 1, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MIN + 6, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_MIN + 1, rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_INI,     rp[3]);
}

static void lmp_test_clearbit_size_0001(void)
{
    size_t rn = lmp_clearbit_size(NULL, 2L, LMP_LIMB_W * 0 );
    ASSERT_SIZE_EQUAL("rn", 2L, rn);
}

static void lmp_test_clearbit_size_0002(void)
{
    size_t rn = lmp_clearbit_size(NULL, 2L, LMP_LIMB_W * 2 );
    ASSERT_SIZE_EQUAL("rn", 2L, rn);
}

static void lmp_test_clearbit_size_0003(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1, LMP_LIMB_MIN, LMP_LIMB_MIN + 2 };
    size_t rn = lmp_clearbit_size(ap, 3L, LMP_LIMB_W * 3 );
    ASSERT_SIZE_EQUAL("rn", 3L, rn);
}

static void lmp_test_clearbit_size_0004(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1, LMP_LIMB_MIN, LMP_LIMB_MIN + 2 };
    size_t rn = lmp_clearbit_size(ap, 3L, LMP_LIMB_W * 2 );
    ASSERT_SIZE_EQUAL("rn", 3L, rn);
}

static void lmp_test_clearbit_size_0005(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1, LMP_LIMB_MIN, LMP_LIMB_MIN + 2 };
    size_t rn = lmp_clearbit_size(ap, 3L, LMP_LIMB_W * 2 + 1 );
    ASSERT_SIZE_EQUAL("rn", 1L, rn);
}

static void lmp_test_clearbit_size_0006(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN + 1, LMP_LIMB_MIN, LMP_LIMB_MIN + 2 };
    size_t rn = lmp_clearbit_size(ap, 3L, LMP_LIMB_W );
    ASSERT_SIZE_EQUAL("rn", 3L, rn);
}

static void lmp_test_clearbit_0001(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI, LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MIN, LMP_LIMB_MIN + 2 };
    lmp_clearbit(rp, 0L, ap, 2L, LMP_LIMB_W + 1);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_INI, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_INI, rp[1]);
}

static void lmp_test_clearbit_0002(void)
{
    lmp_limb_t rp[] = { LMP_LIMB_INI,     LMP_LIMB_INI,     LMP_LIMB_INI,      LMP_LIMB_INI };
    lmp_limb_t ap[] = { LMP_LIMB_MAX - 8, LMP_LIMB_MAX - 1, LMP_LIMB_MAX - 4 };
    lmp_clearbit(rp, 3L, ap, 3L, LMP_LIMB_W + 1);
    ASSERT_LIMB_EQUAL("rp[0]", LMP_LIMB_MAX - 8, rp[0]);
    ASSERT_LIMB_EQUAL("rp[1]", LMP_LIMB_MAX - 3, rp[1]);
    ASSERT_LIMB_EQUAL("rp[2]", LMP_LIMB_MAX - 4, rp[2]);
    ASSERT_LIMB_EQUAL("rp[3]", LMP_LIMB_INI,     rp[3]);
}

static void lmp_test_popcount_0001(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MAX };
    size_t rn = lmp_popcount(ap, 1L);
    ASSERT_SIZE_EQUAL("rn", (size_t) LMP_LIMB_W, rn);
}

static void lmp_test_popcount_0002(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MAX, LMP_LIMB_MIN + 3 };
    size_t rn = lmp_popcount(ap, 2L);
    ASSERT_SIZE_EQUAL("rn", (size_t) LMP_LIMB_W + 2, rn);
}

/*****************************************************************************
 * main
 *****************************************************************************/

int main(void) {
    lmp_test_diff_mn_size_0001();
    lmp_test_diff_mn_size_0002();
    lmp_test_diff_mn_size_0003();
    lmp_test_diff_mn_size_0004();
    lmp_test_diff_mn_size_0005();
    lmp_test_diff_mn_size_0006();
    lmp_test_diff_mn_size_0007();
    lmp_test_diff_mn_size_0008();
    lmp_test_diff_mn_size_0009();

    lmp_test_mul_mn_0001();
    lmp_test_mul_mn_0002();
    lmp_test_mul_mn_0003();

/*
    lmp_test_lshift_size_0001();
    lmp_test_lshift_size_0002();
    lmp_test_lshift_size_0003();
    lmp_test_lshift_size_0004();
    lmp_test_lshift_size_0005();
    lmp_test_lshift_size_0006();
    lmp_test_lshift_size_0007();
    lmp_test_lshift_size_0008();
    lmp_test_lshift_size_0009();
    lmp_test_lshift_size_0010();
    lmp_test_lshift_size_0011();
    lmp_test_lshift_0001();
    lmp_test_lshift_0002();
    lmp_test_lshift_0003();
    lmp_test_lshift_0004();
    lmp_test_lshift_0005();
    lmp_test_lshift_0006();
    lmp_test_lshift_0007();
    lmp_test_lshift_0008();
    lmp_test_lshift_0009();
    lmp_test_lshift_0010();
    lmp_test_lshift_0011();
    lmp_test_lshift_0012();

    lmp_test_rshift_0001();
    lmp_test_rshift_0002();
    lmp_test_rshift_0003();
    lmp_test_rshift_0004();
    lmp_test_rshift_0005();
    lmp_test_rshift_0006();
    lmp_test_rshift_0007();
    lmp_test_rshift_0008();
    lmp_test_rshift_0009();
    lmp_test_rshift_0010();

    lmp_test_ior_mn_size_0001();
    lmp_test_ior_mn_size_0002();
    lmp_test_ior_mn_size_0003();
    lmp_test_ior_mn_0001();

    lmp_test_xor_mn_size_0001();
    lmp_test_xor_mn_size_0002();
    lmp_test_xor_mn_size_0003();
    lmp_test_xor_mn_0001();

    lmp_test_and_mn_size_0001();
    lmp_test_and_mn_size_0002();
    lmp_test_and_mn_0001();

    lmp_test_testbit_0001();
    lmp_test_testbit_0002();
    lmp_test_testbit_0003();

    lmp_test_setbit_size_0001();
    lmp_test_setbit_size_0002();
    lmp_test_setbit_0001();
    lmp_test_setbit_0002();

    lmp_test_clearbit_size_0001();
    lmp_test_clearbit_size_0002();
    lmp_test_clearbit_size_0003();
    lmp_test_clearbit_size_0004();
    lmp_test_clearbit_size_0005();
    lmp_test_clearbit_size_0006();
    lmp_test_clearbit_0001();
    lmp_test_clearbit_0002();

    lmp_test_popcount_0001();
    lmp_test_popcount_0002(); */
}
