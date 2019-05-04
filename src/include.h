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

#ifndef LMP_SRC_INCLUDE_H
#define LMP_SRC_INCLUDE_H

#include "../lmp.h"

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

#endif
