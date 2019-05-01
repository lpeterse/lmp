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

#if !defined(LMP_DISABLE_ASM) && defined(__amd64__)

#define LMP_ADDC_MM_ASM
static inline lmp_limb_t lmp_addc_mm_asm(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap,
    const lmp_limb_t *restrict bp, size_t m, lmp_limb_t c)
{
   lmp_limb_t res;

   __asm__ (
    "   xor %[res], %[res]; \
        jrcxz 4f; \
        leaq (%[rp],%[m],8), %[rp]; \
        leaq (%[ap],%[m],8), %[ap]; \
        leaq (%[bp],%[m],8), %[bp]; \
        neg %[m]; \
        bt $0, %[m]; \
        jnc 1f; \
        \
        add (%[ap],%[m],8), %[c]; \
        adc (%[bp],%[m],8), %[c]; \
        mov %[c], (%[rp],%[m],8); \
        leaq 1(%[m]), %[m]; \
        jrcxz 3f; \
        jmp 2f; \
    1:; \
        bt $0, %[c]; \
    2:; \
        movq (%[ap],%[m],8), %[c]; \
        adc (%[bp],%[m],8), %[c]; \
        mov %[c], (%[rp],%[m],8); \
        movq 8(%[ap],%[m],8), %[c]; \
        adc 8(%[bp],%[m],8), %[c]; \
        mov %[c], 8(%[rp],%[m],8); \
        leaq 2(%[m]), %[m]; \
        jrcxz 3f; \
        jmp 2b; \
    3:; \
        setb %b[res]; \
    4:;"

   : [res] "=r" (res), [rp] "+r" (rp), [ap] "+r" (ap), [bp] "+r" (bp), [m] "+c" (m), [c] "+r" (c)
   :: "cc", "memory"
   );

   return res;
}

#endif
