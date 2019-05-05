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

#include <stdio.h>
#include <stdlib.h>

#include "lmp.h"

#define SZ(x) (sizeof(x) / LMP_LIMB_S)

#define RP { LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, \
             LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, \
             LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI, LMP_LIMB_INI }
             
#define ASSERT_SIZE_EQUAL(value, expected) {\
        if ((expected) != (value)) {\
            printf ("%s failed: %s:\n  expected: %ld\n  actual:   %ld\n", \
                __func__, #value, expected, value); \
            exit(1); \
        }\
    }

#define ASSERT_INT_EQUAL(value, expected) {\
        if ((expected) != (value)) {\
            printf ("%s failed: %s:\n  expected: %d\n  actual:   %d\n", \
                __func__, #value, expected, value); \
            exit(1); \
        }\
    }

#define ASSERT_LIMB_EQUAL(value, expected) {\
        if ((expected) != (value)) {\
            printf ("%s failed: %s:\n  expected: 0x%016lx\n  actual:   0x%016lx\n", \
                __func__, #value, expected, value); \
            exit(1); \
        }\
    }

#include "test/lmp_add_1c.h"
#include "test/lmp_and_mn.h"
#include "test/lmp_and_mn_size.h"
#include "test/lmp_clearbit_size.h"
#include "test/lmp_clearbit.h"
#include "test/lmp_cmp_mm.h"
#include "test/lmp_cmp_mn.h"
#include "test/lmp_ior_mn.h"
#include "test/lmp_ior_mn_size.h"
#include "test/lmp_lshift_size.h"
#include "test/lmp_lshift.h"
#include "test/lmp_mul_m1.h"
#include "test/lmp_mul_mn.h"
#include "test/lmp_rshift.h"
#include "test/lmp_setbit.h"
#include "test/lmp_setbit_size.h"
#include "test/lmp_sub_mmb.h"
#include "test/lmp_sub_mn_size.h"
#include "test/lmp_testbit.h"
#include "test/lmp_xor_mn_size.h"
#include "test/lmp_xor_mn.h"
#include "test/lmp_popcount_m.h"

int main(void) {
    LMP_ADD_1C_TESTS();
    LMP_AND_MN_SIZE_TESTS();
    LMP_AND_MN_TESTS();
    LMP_CLEARBIT_SIZE_TESTS();
    LMP_CLEARBIT_TESTS();
    LMP_CMP_MM_TESTS();
    LMP_CMP_MN_TESTS();
    LMP_IOR_MN_TESTS();
    LMP_IOR_MN_SIZE_TESTS();
    LMP_LSHIFT_TESTS();
    LMP_LSHIFT_SIZE_TESTS();
    LMP_MUL_M1_TESTS();
    LMP_MUL_MN_TESTS();
    LMP_RSHIFT_TESTS();
    LMP_SETBIT_TESTS();
    LMP_SETBIT_SIZE_TESTS();
    LMP_SUB_MMB_TESTS();
    LMP_SUB_MN_SIZE_TESTS();
    LMP_TESTBIT_TESTS();
    LMP_XOR_MN_SIZE_TESTS();
    LMP_XOR_MN_TESTS();
    LMP_POPCOUNT_M_TESTS();
}
