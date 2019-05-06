#define LMP_SUB_MMB
inline lmp_limb_t lmp_sub_mmb(
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
        "bt      $0, %[borrow];"
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
        : [rp] "+&r" (rp), [ap] "+&r" (ap), [bp] "+&r" (bp),
          [m] "+&c" (m), [borrow] "+&r" (borrow), [res] "=&a" (res)
        :
        : "cc", "memory"
    );

    return res;
}
