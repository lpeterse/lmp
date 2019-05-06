#define LMP_CMP_MM
inline int lmp_cmp_mm(
    const lmp_limb_t *restrict ap,
    const lmp_limb_t *restrict bp, size_t m)
{
    long res;
    long tmp;

    __asm__ (
        "xorq    %[res], %[res];"
        "test    %[m], %[m];"
        "je      3f;"
    ".p2align 4, 0x90;"
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
        : [m] "+&r" (m), [res] "=&r" (res), [tmp] "=&r" (tmp)
        : [ap] "r" (ap), [bp] "r" (bp)
        : "cc"
    );

    return (int) res;
}
