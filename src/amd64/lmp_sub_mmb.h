#define LMP_SUB_MMB
inline lmp_limb_t lmp_sub_mmb(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap,
    const lmp_limb_t *restrict bp, size_t m, lmp_limb_t borrow)
{
    lmp_limb_t res;

    __asm__ volatile (
        // Immediate return if m == 0
        "movq    %[borrow], %[res];\n\t"
        "jrcxz   4f;\n\t"
        // Address increment shall not interfere with carry flag
        "leaq    (%[rp],%[m],8), %[rp];\n\t"
        "leaq    (%[ap],%[m],8), %[ap];\n\t"
        "leaq    (%[bp],%[m],8), %[bp];\n\t"
        "negq    %[m];\n\t"
        // Skip initial subtraction if m is even
        "bt      $0, %[m];\n\t"
        "jnc     1f;\n\t"
        // Do one subtraction in advance if m is odd
        "bt      $0, %[res];\n\t"
        "movq    (%[ap],%[m],8), %[res];\n\t"
        "sbbq    (%[bp],%[m],8), %[res];\n\t"
        "movq    %[res], (%[rp],%[m],8);\n\t"
        "leaq    1(%[m]), %[m];\n\t"
        "jrcxz   3f;\n\t"
        "jmp     2f;\n"
    "1:;\n\t"
        // Initialize carry flag
        "bt      $0, %[res];\n"
    "2:;\n\t"
        // Do two subtractions per loop iteration
        "movq    (%[ap],%[m],8), %[res];\n\t"
        "sbbq    (%[bp],%[m],8), %[res];\n\t"
        "movq    %[res], (%[rp],%[m],8);\n\t"
        "movq    8(%[ap],%[m],8), %[res];\n\t"
        "sbbq    8(%[bp],%[m],8), %[res];\n\t"
        "movq    %[res], 8(%[rp],%[m],8);\n\t"
        "leaq    2(%[m]), %[m];\n\t"
        "jrcxz   3f;\n\t"
        "jmp     2b;\n"
    "3:;\n\t"
        // Store carry flag as final borrow
        "movq    $0, %[res];\n\t"
        "setb    %b[res];\n"
    "4:;\n\t"
        : [rp] "+r" (rp), [ap] "+r" (ap), [bp] "+r" (bp),
          [m] "+c" (m), [res] "=r" (res)
        : [borrow] "rmi" (borrow)
        : "cc", "memory"
    );

    return res;
}
