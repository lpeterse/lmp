#define LMP_MUL_M1_ADD
inline lmp_limb_t lmp_mul_m1_add(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap, size_t m, lmp_limb_t b)
{
    lmp_limb_t rax;
    lmp_limb_t rdx;
    lmp_limb_t carry = 0;

    __asm__ (
        "leaq    (%[rp],%[m],8), %[rp];"
        "leaq    (%[ap],%[m],8), %[ap];"
        "negq    %[m];"
        "jz      4f;"
    "1:;"
        "bt      $0, %[m];"
        "jnc     2f;"
        "movq    %[b], %[rax];"
        "mulq    (%[ap],%[m],8);"
        "addq    %[carry], %[rax];"
        "adcq    $0, %[rdx];"
        "addq    %[rax], (%[rp],%[m],8);"
        "adcq    $0, %[rdx];"
        "movq    %[rdx], %[carry];"
        "add     $1, %[m];"
        "jz      4f;"
    "2:;"
        "bt      $1, %[m];"
        "jnc     3f;"
        "movq    %[b], %[rax];"
        "mulq    (%[ap],%[m],8);"
        "addq    %[carry], %[rax];"
        "adcq    $0, %[rdx];"
        "addq    %[rax], (%[rp],%[m],8);"
        "adcq    $0, %[rdx];"
        "movq    %[rdx], %[carry];"
        "movq    %[b], %[rax];"
        "mulq    8(%[ap],%[m],8);"
        "addq    %[carry], %[rax];"
        "adcq    $0, %[rdx];"
        "addq    %[rax], 8(%[rp],%[m],8);"
        "adcq    $0, %[rdx];"
        "movq    %[rdx], %[carry];"
        "add     $2, %[m];"
        "jz      4f;"
    "3:;"
        ".p2align 4, 0x90;"
        "movq    %[b], %[rax];"
        "mulq    (%[ap],%[m],8);"
        "addq    %[carry], %[rax];"
        "adcq    $0, %[rdx];"
        "addq    %[rax], (%[rp],%[m],8);"
        "adcq    $0, %[rdx];"
        "movq    %[rdx], %[carry];"
        "movq    %[b], %[rax];"
        "mulq    8(%[ap],%[m],8);"
        "addq    %[carry], %[rax];"
        "adcq    $0, %[rdx];"
        "addq    %[rax], 8(%[rp],%[m],8);"
        "adcq    $0, %[rdx];"
        "movq    %[rdx], %[carry];"
        "movq    %[b], %[rax];"
        "mulq    16(%[ap],%[m],8);"
        "addq    %[carry], %[rax];"
        "adcq    $0, %[rdx];"
        "addq    %[rax], 16(%[rp],%[m],8);"
        "adcq    $0, %[rdx];"
        "movq    %[rdx], %[carry];"
        "movq    %[b], %[rax];"
        "mulq    24(%[ap],%[m],8);"
        "addq    %[carry], %[rax];"
        "adcq    $0, %[rdx];"
        "addq    %[rax], 24(%[rp],%[m],8);"
        "adcq    $0, %[rdx];"
        "movq    %[rdx], %[carry];"
        "add     $4, %[m];"
        "jnz     3b;"
    "4:;"
        : [rax] "=&a" (rax), [rdx] "=&d" (rdx), [m] "+&r" (m), [carry] "+&r" (carry)
        , [rp] "+&r" (rp), [ap] "+&r" (ap)
        : [b] "r" (b)
        : "cc", "memory"
    );

    return carry;
}
