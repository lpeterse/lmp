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
        "bt      $0, %[carry];"
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
