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

#define LMP_XOR_MN_SIZE_TESTS() { \
        lmp_xor_mn_size_test_0001(); \
        lmp_xor_mn_size_test_0002(); \
        lmp_xor_mn_size_test_0003(); \
    }

static void lmp_xor_mn_size_test_0001(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_INI };
    lmp_limb_t bp[] = { LMP_LIMB_INI };
    size_t rn = lmp_xor_mn_size(ap, 0L, bp, 0L);
    ASSERT_SIZE_EQUAL(rn, 0L);
}

static void lmp_xor_mn_size_test_0002(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN,     LMP_LIMB_MAX, LMP_LIMB_MIN + 2, LMP_LIMB_MIN + 3 };
    lmp_limb_t bp[] = { LMP_LIMB_MIN + 1, LMP_LIMB_MAX, LMP_LIMB_MIN + 2, LMP_LIMB_MIN + 3 };
    size_t rn = lmp_xor_mn_size(ap, 4L, bp, 4L);
    ASSERT_SIZE_EQUAL(rn, 1L);
}

static void lmp_xor_mn_size_test_0003(void)
{
    lmp_limb_t ap[] = { LMP_LIMB_MIN, LMP_LIMB_MIN, LMP_LIMB_MIN };
    lmp_limb_t bp[] = { LMP_LIMB_MIN, LMP_LIMB_MIN, LMP_LIMB_MIN, LMP_LIMB_MIN };
    size_t rn = lmp_xor_mn_size(ap, 3L, bp, 4L);
    ASSERT_SIZE_EQUAL(rn, 4L);
}
