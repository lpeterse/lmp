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

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define LIMB_MIN         LMP_LIMB_C(0x0000000000000000)
#define LIMB_MIN_PP      LMP_LIMB_C(0x0000000000000001)
#define LIMB_MAX         LMP_LIMB_C(0xFFFFFFFFFFFFFFFF)
#define LIMB_MAX_MM      LMP_LIMB_C(0xFFFFFFFFFFFFFFFE)
#define LIMB_HALF_FULL   LMP_LIMB_C(0x00000000FFFFFFFF)
#define LIMB_HALF_FULL_1 LMP_LIMB_C(0x0000000100000000)
#define LIMB_INI         LMP_LIMB_C(0x2323232323232323)

// It is considered necessary to check the value of ULLONG_MAX
// here as a precondition for all overflow related operations.
#if __WORDSIZE == 64 && ULONG_MAX == 18446744073709551615ULL
typedef unsigned long lmp_limb_t;
typedef __uint128_t lmp_dlimb_t;
#define LMP_LIMB_S 8
#define LMP_LIMB_W __WORDSIZE
#define LMP_LIMB_C(c) c ## UL
#define LMP_LIMB_MAX ULONG_MAX
#define LMP_MSB(x) (LMP_LIMB_W - 1 - __builtin_clzl(x))
#define LMP_ADDC(x,y,ci,co) __builtin_addcl(x,y,ci,co)

#elif __WORDSIZE == 32 && ULONG_MAX == 4294967295UL
typedef unsigned long lmp_limb_t;
typedef unsigned int lmp_dlimb_t;
#define LMP_LIMB_S 4
#define LMP_LIMB_W __WORDSIZE
#define LMP_LIMB_C(c) c ## UL
#define LMP_LIMB_MAX ULONG_MAX
#define LMP_MSB(x) (LMP_LIMB_W - 1 - __builtin_clzl(x))

#else
#error "unknown machine word size"
#endif

/*****************************************************************************
 * Multiplication
 *****************************************************************************/

void lmp_mul_m1(
          lmp_limb_t *const restrict rp,
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t b);

size_t lmp_mul_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn);

void lmp_mul_mn(
          lmp_limb_t *const restrict rp,
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn);

/*****************************************************************************
 * Shift operations
 *****************************************************************************/

size_t lmp_lshift_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bits);

void lmp_lshift(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bits);

size_t lmp_rshift_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bits);

void lmp_rshift(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bits);

/*****************************************************************************
 * Bitwise operations 
 *****************************************************************************/

size_t lmp_ior_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn);

void lmp_ior_mn(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp);

size_t lmp_xor_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn);

void lmp_xor_mn(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp);

size_t lmp_and_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn);

void lmp_and_mn(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp);

size_t lmp_testbit(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi);

size_t lmp_setbit_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi);

void lmp_setbit(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi);

size_t lmp_clearbit_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi);

void lmp_clearbit(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi);

size_t lmp_popcount(
    const lmp_limb_t *const restrict ap, const size_t an);

