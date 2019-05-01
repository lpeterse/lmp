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

/******************************************************************************
 * Compare
 *****************************************************************************/

int lmp_cmp_mm_gen(
    const lmp_limb_t *restrict ap,
    const lmp_limb_t *restrict bp, size_t m)
{
    while (m--) {
        if (ap[m] != bp[m]) {
            return ap[m] > bp[m] ? 1 : -1;
        }
    }
    return 0;
}

int lmp_cmp_mn_gen(
    const lmp_limb_t *restrict ap, size_t an,
    const lmp_limb_t *restrict bp, size_t bn)
{
    if (an > bn) {
        return 1;
    }
    if (an < bn) {
        return -1;
    }
    return lmp_cmp_mm(ap, bp, an);
}

/******************************************************************************
 * Addition
 *****************************************************************************/

lmp_limb_t lmp_addc_m_gen(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap, size_t m, lmp_limb_t c)
{
    size_t i = 0;
    for (; i < m && c; i++) {
        lmp_limb_t a = rp[i] = ap[i] + 1;
        if (a) break;
    }
    for (++i; i < m; i++) {
        rp[i] = ap[i];
        c = 0;
    }
    return c;
}

lmp_limb_t lmp_addc_mm_gen(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap,
    const lmp_limb_t *restrict bp, size_t m, lmp_limb_t c)
{
    for (size_t i = 0; i < m; i++) {
        lmp_dlimb_t x = c;
        x += ap[i];
        x += bp[i];
        rp[i] = (lmp_limb_t) x;
        c = (lmp_limb_t) (x >> LMP_LIMB_W);
    }
    return c;
}
