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

#define LMP_ADD_1C_TESTS() { \
        lmp_add_1c_test_0001(); \
        lmp_add_1c_test_0002(); \
        lmp_add_1c_test_0003(); \
        lmp_add_1c_test_0004(); \
        lmp_add_1c_test_0005(); \
    }

static void lmp_add_1c_test_0001(void)
{
    lmp_limb_t c = 0;
    lmp_limb_t r = lmp_add_1c(LMP_LIMB_C(1), LMP_LIMB_C(1), &c);
    ASSERT_LIMB_EQUAL(r, LMP_LIMB_C(2));
    ASSERT_LIMB_EQUAL(c, LMP_LIMB_C(0));
}

static void lmp_add_1c_test_0002(void)
{
    lmp_limb_t c = 0;
    lmp_limb_t r = lmp_add_1c(LMP_LIMB_MAX, LMP_LIMB_C(1), &c);
    ASSERT_LIMB_EQUAL(r, LMP_LIMB_C(0));
    ASSERT_LIMB_EQUAL(c, LMP_LIMB_C(1));
}

static void lmp_add_1c_test_0003(void)
{
    lmp_limb_t c = 1;
    lmp_limb_t r = lmp_add_1c(LMP_LIMB_MAX, LMP_LIMB_C(0), &c);
    ASSERT_LIMB_EQUAL(r, LMP_LIMB_C(0));
    ASSERT_LIMB_EQUAL(c, LMP_LIMB_C(1));
}

static void lmp_add_1c_test_0004(void)
{
    lmp_limb_t c = 0;
    lmp_limb_t r = lmp_add_1c(LMP_LIMB_MAX, LMP_LIMB_C(4), &c);
    ASSERT_LIMB_EQUAL(r, LMP_LIMB_C(3));
    ASSERT_LIMB_EQUAL(c, LMP_LIMB_C(1));
}

static void lmp_add_1c_test_0005(void)
{
    lmp_limb_t c = 0;
    lmp_limb_t r = lmp_add_1c(LMP_LIMB_C(0), LMP_LIMB_C(0), &c);
    ASSERT_LIMB_EQUAL(r, LMP_LIMB_C(0));
    ASSERT_LIMB_EQUAL(c, LMP_LIMB_C(0));
}