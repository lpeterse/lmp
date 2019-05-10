#define LMP_MUL_M1_ADD
static inline lmp_limb_t lmp_mul_m1_add_adx(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap, size_t m, lmp_limb_t b)
{
    lmp_limb_t lo, hi1=0, hi2=0;

    __asm__ volatile (
        "jrcxz   0f;"           // m == 0

        "leaq    (%[rp],%[m],8), %[rp];"
        "leaq    (%[ap],%[m],8), %[ap];"
        "movq    %[m], %[lo];"
        "negq    %[m];"

        "andq    $3, %[lo];"
        "cmpq    $3, %[lo];"
        "je      4f;"           // m % 4 == 3
        "cmpq    $2, %[lo];"
        "je      3f;"           // m % 4 == 2
        "cmpq    $1, %[lo];"
        "je      2f;"           // m % 4 == 1
        "xorq    %[lo], %[lo];" // clear flags
        "jmp     5f;"           // m % 4 == 0
    "0:;"
        "jmp     7f;"           // jump redirect to 7
    "1:;"
        "jmp     6f;"           // jump redirect to 6
    "2:;"
        "xorq    %[lo], %[lo];" // clear flags

        "mulx    (%[ap],%[m],8), %[lo], %[hi2];"
        "addq    (%[rp],%[m],8), %[lo];"
        "movq    %[lo], 0(%[rp],%[m],8);"

        "leaq    1(%[m]), %[m];"
        "jrcxz   1b;"           // indirect to 6
        "jmp     5f;"
    "3:;"
        "xorq    %[lo], %[lo];" // clear flags

        "mulx    (%[ap],%[m],8), %[lo], %[hi1];"
        "adcx    (%[rp],%[m],8), %[lo];"
        "adox     %[hi2], %[lo];"
        "movq     %[lo], (%[rp],%[m],8);"

        "mulx    8(%[ap],%[m],8), %[lo], %[hi2];"
        "adcx    8(%[rp],%[m],8), %[lo];"
        "adox    %[hi1], %[lo];"
        "movq    %[lo], 8(%[rp],%[m],8);"

        "leaq    2(%[m]), %[m];"
        "jrcxz   1b;"           // indirect to 6
        "jmp     5f;"
    "4:;"
        "xorq    %[lo], %[lo];" // clear flags

        "mulx    (%[ap],%[m],8), %[lo], %[hi2];"
        "adcx    (%[rp],%[m],8), %[lo];"
        "adox    %[hi1], %[lo];"
        "movq    %[lo], (%[rp],%[m],8);"

        "mulx    8(%[ap],%[m],8), %[lo], %[hi1];"
        "adcx    8(%[rp],%[m],8), %[lo];"
        "adox    %[hi2], %[lo];"
        "movq    %[lo], 8(%[rp],%[m],8);"

        "mulx    16(%[ap],%[m],8), %[lo], %[hi2];"
        "adcx    16(%[rp],%[m],8), %[lo];"
        "adox    %[hi1], %[lo];"
        "movq    %[lo], 16(%[rp],%[m],8);"

        "leaq    3(%[m]), %[m];"
        "jrcxz   6f;"
    "5:;"
        ".p2align 4, 0x90;"
        "mulx    (%[ap],%[m],8), %[lo], %[hi1];"
        "adcx    (%[rp],%[m],8), %[lo];"
        "adox     %[hi2], %[lo];"
        "movq     %[lo], (%[rp],%[m],8);"

        "mulx    8(%[ap],%[m],8), %[lo], %[hi2];"
        "adcx    8(%[rp],%[m],8), %[lo];"
        "adox    %[hi1], %[lo];"
        "movq    %[lo], 8(%[rp],%[m],8);"

        "mulx    16(%[ap],%[m],8), %[lo], %[hi1];"
        "adcx    16(%[rp],%[m],8), %[lo];"
        "adox    %[hi2], %[lo];"
        "movq    %[lo], 16(%[rp],%[m],8);"

        "mulx    24(%[ap],%[m],8), %[lo], %[hi2];"
        "adcx    24(%[rp],%[m],8), %[lo];"
        "adox    %[hi1], %[lo];"
        "movq    %[lo], 24(%[rp],%[m],8);"

        "leaq    4(%[m]), %[m];"
        "jrcxz   6f;"
        "jmp     5b;"
    "6:;"
        "mov    $0, %[hi1];"
        "adcx   %[hi1], %[hi2];"  // from ADDC carry chain
        "adox   %[hi1], %[hi2];"  // from ADOX carry chain
    "7:;"
        : [m] "+&c" (m), [rp] "+&r" (rp), [ap] "+&r" (ap),
          [lo] "=&r" (lo), [hi1] "+&r" (hi1),  [hi2] "+&r" (hi2)
        : [b] "d" (b)
        : "cc", "memory"
    );

    return hi2;
}

static inline lmp_limb_t lmp_mul_m1_add_noadx(
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

inline lmp_limb_t lmp_mul_m1_add(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap, size_t m, lmp_limb_t b)
{   
    if (lmp_cpu_has_feature(LMP_CPU_FEATURE_ADX)) {
        return lmp_mul_m1_add_adx(rp, ap, m, b);
    } else {
        return lmp_mul_m1_add_noadx(rp, ap, m, b);
    }
}
