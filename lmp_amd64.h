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
#include "lmp_include.h"

#if !defined(LMP_NOASM) && defined(__amd64__)

/*****************************************************************************
 * CPU feature detection
 *****************************************************************************/

#define LMP_CPU_FEATURE_POPCNT (1UL << 23)

static inline unsigned long lmp_cpu_get_features()
{
    unsigned long r;

    __asm__ (
        // Save clobbered registers on stack in order to not having to
        // declare them which turned out to confuse the register allocator.
        "pushq   %%rbx;"
        "pushq   %%rcx;"
        "pushq   %%rdx;"
        "xor     %%rcx, %%rcx;"
        "xor     %%rdx, %%rdx;"
        // Call CPUID function 1.
        "mov     $1, %%eax;"
        "cpuid;"
        "mov     %%edx, %%eax;"
        "shl     $32, %%rax;"
        "xor     %%rcx, %%rax;"
        // Restore registers.
        "popq    %%rdx;"
        "popq    %%rcx;"
        "popq    %%rbx;"
        : "=a" (r)
        :: "cc"
    );

    return r;
}

static inline unsigned long lmp_cpu_has_feature(unsigned long feature)
{
    static unsigned long lmp_cpu_features;

    if (!lmp_cpu_features) {
        lmp_cpu_features = lmp_cpu_get_features();
    }

    return lmp_cpu_features & feature;
}

#define LMP_CMP_MM
int lmp_cmp_mm(
    const lmp_limb_t *restrict ap,
    const lmp_limb_t *restrict bp, size_t m)
{
    long res;
    long tmp;

    __asm__ (
        "xorq    %[res], %[res];"
        "test    %[m], %[m];"
        "je      3f;"
        "xchg    %k[res], %k[res];" // align loop
    "1:;"
        "dec     %[m];"
        "jz      2f;" // fuses with dec
        "movq    (%[bp],%[m],8), %[tmp];"
        "cmpq    (%[ap],%[m],8), %[tmp];"
        "je      1b;" // fuses with cmp
    "2:;"
        "movq    (%[bp],%[m],8), %[tmp];"
        "cmpq    (%[ap],%[m],8), %[tmp];"
        "je      3f;"
        "sbb     %k[res], %k[res];"
        "and     $2, %k[res];"
        "dec     %k[res];"
    "3:;"
        : [m] "+r" (m), [res] "=a" (res), [tmp] "=r" (tmp)
        : [ap] "r" (ap), [bp] "r" (bp)
        : "cc"
    );

    return (int) res;
}

/*****************************************************************************
 * Addition
 *****************************************************************************/

#define LMP_ADD_MMC
lmp_limb_t lmp_add_mmc(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap,
    const lmp_limb_t *restrict bp, size_t m, lmp_limb_t carry)
{
    lmp_limb_t res;

    __asm__ (
        // Immediate return if m == 0
        "movq    %[carry], %[res];"
        "jrcxz   4f;"
        // Address increment shall not interfere with carry flag
        "leaq    (%[rp],%[m],8), %[rp];"
        "leaq    (%[ap],%[m],8), %[ap];"
        "leaq    (%[bp],%[m],8), %[bp];"
        "negq    %[m];"
        // Skip initial addition if m is even
        "bt      $0, %[m];"
        "jnc     1f;"
        // Do one addition in advance if m is odd
        "addq    (%[ap],%[m],8), %[carry];"
        "adcq    (%[bp],%[m],8), %[carry];"
        "movq    %[carry], (%[rp],%[m],8);"
        "leaq    1(%[m]), %[m];"
        "jrcxz   3f;"
        "jmp     2f;"
    "1:;"
        // Initialize carry flag
        "bt      $0, %[carry];"
    "2:;"
        // Do two additions per loop iteration
        "movq    (%[ap],%[m],8), %[carry];"
        "adcq    (%[bp],%[m],8), %[carry];"
        "movq    %[carry], (%[rp],%[m],8);"
        "movq    8(%[ap],%[m],8), %[carry];"
        "adcq    8(%[bp],%[m],8), %[carry];"
        "movq    %[carry], 8(%[rp],%[m],8);"
        "leaq    2(%[m]), %[m];"
        "jrcxz   3f;"
        "jmp     2b;"
    "3:;"
        // Store carry flag as final carry
        "movl    $0, %k[res];"
        "setb    %b[res];"
    "4:;"
        : [rp] "+r" (rp), [ap] "+r" (ap), [bp] "+r" (bp),
          [m] "+c" (m), [carry] "+r" (carry), [res] "=r" (res)
        :
        : "cc", "memory"
   );

   return res;
}

/*****************************************************************************
 * SUBTRACTION
 *****************************************************************************/

#define LMP_SUB_MMB
lmp_limb_t lmp_sub_mmb(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap,
    const lmp_limb_t *restrict bp, size_t m, lmp_limb_t borrow)
{
    lmp_limb_t res;

    __asm__ (
        // Immediate return if m == 0
        "movq    %[borrow], %[res];"
        "jrcxz   4f;"
        // Address increment shall not interfere with carry flag
        "leaq    (%[rp],%[m],8), %[rp];"
        "leaq    (%[ap],%[m],8), %[ap];"
        "leaq    (%[bp],%[m],8), %[bp];"
        "negq    %[m];"
        // Skip initial subtraction if m is even
        "bt      $0, %[m];"
        "jnc     1f;"
        // Do one subtraction in advance if m is odd
        "movq    (%[ap],%[m],8), %[borrow];"
        "sbbq    (%[bp],%[m],8), %[borrow];"
        "movq    %[borrow], (%[rp],%[m],8);"
        "leaq    1(%[m]), %[m];"
        "jrcxz   3f;"
        "jmp     2f;"
    "1:;"
        // Initialize carry flag
        "bt      $0, %[borrow];"
    "2:;"
        // Do two subtractions per loop iteration
        "movq    (%[ap],%[m],8), %[borrow];"
        "sbbq    (%[bp],%[m],8), %[borrow];"
        "movq    %[borrow], (%[rp],%[m],8);"
        "movq    8(%[ap],%[m],8), %[borrow];"
        "sbbq    8(%[bp],%[m],8), %[borrow];"
        "movq    %[borrow], 8(%[rp],%[m],8);"
        "leaq    2(%[m]), %[m];"
        "jrcxz   3f;"
        "jmp     2b;"
    "3:;"
        // Store carry flag as final borrow
        "movl    $0, %k[res];"
        "setb    %b[res];"
    "4:;"
        : [rp] "+r" (rp), [ap] "+r" (ap), [bp] "+r" (bp),
          [m] "+c" (m), [borrow] "+r" (borrow), [res] "=a" (res)
        :
        : "cc", "memory"
    );

    return res;
}

/*****************************************************************************
 * Bitwise operations 
 *****************************************************************************/

static inline size_t lmp_popcount_1(lmp_limb_t a)
{
    size_t r;

    __asm__ (
        "popcnt %[r], %[a];"
        : [r] "=r" (r)
        : [a] "r" (a)
        : "cc"
    );

    return r;
}

#define LMP_POPCOUNT
size_t lmp_popcount(
    const lmp_limb_t *restrict ap, size_t an)
{
    size_t r = 0;

    if (lmp_cpu_has_feature(LMP_CPU_FEATURE_POPCNT)) {
        for (; an > 0; an--) {
            r += lmp_popcount_1(ap[an - 1]);
        }
    } else {
        for (; an > 0; an--) {
            r += POPCOUNT(ap[an - 1]);
        }
    }

    return r;
}

#endif
