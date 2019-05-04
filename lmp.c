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

#include "src/include.h"

#if !defined(LMP_NOASM) && defined(__amd64__)
#include "src/amd64/lmp_cpuid.h"
#include "src/amd64/lmp_add_1c.h"
#include "src/amd64/lmp_add_mmc.h"
#include "src/amd64/lmp_sub_mmb.h"
#include "src/amd64/lmp_cmp_mm.h"
#include "src/amd64/lmp_popcount_m.h"
#endif

#include "src/generic/lmp_add_1c.h"
#include "src/generic/lmp_add_mc.h"
#include "src/generic/lmp_add_mmc.h"
#include "src/generic/lmp_add_mnc.h"
#include "src/generic/lmp_add_mn.h"
#include "src/generic/lmp_add_mn_size.h"

#include "src/generic/lmp_sub_mb.h"
#include "src/generic/lmp_sub_mmb.h"
#include "src/generic/lmp_sub_mn.h"
#include "src/generic/lmp_sub_mn_size.h"

#include "src/generic/lmp_lshift.h"
#include "src/generic/lmp_lshift_size.h"

#include "src/generic/lmp_rshift.h"
#include "src/generic/lmp_rshift_size.h"

#include "src/generic/lmp_mul_m1.h"
#include "src/generic/lmp_mul_mn.h"
#include "src/generic/lmp_mul_mn_size.h"

#include "src/generic/lmp_cmp_mm.h"
#include "src/generic/lmp_cmp_mn.h"

#include "src/generic/lmp_and_mn.h"
#include "src/generic/lmp_and_mn_size.h"

#include "src/generic/lmp_ior_mn.h"
#include "src/generic/lmp_ior_mn_size.h"

#include "src/generic/lmp_xor_mn.h"
#include "src/generic/lmp_xor_mn_size.h"

#include "src/generic/lmp_clearbit.h"
#include "src/generic/lmp_clearbit_size.h"

#include "src/generic/lmp_setbit.h"
#include "src/generic/lmp_setbit_size.h"

#include "src/generic/lmp_testbit.h"

#include "src/generic/lmp_popcount_1.h"
#include "src/generic/lmp_popcount_m.h"
