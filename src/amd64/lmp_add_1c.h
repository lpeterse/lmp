#define LMP_ADD_1C
inline lmp_limb_t lmp_add_1c(
    const lmp_limb_t a, const lmp_limb_t b, lmp_limb_t *carry)
{
    lmp_limb_t r;
    lmp_limb_t c = *carry;

    __asm__ (
        "bt      $0, %[c];"
        "mov     %[a], %[r];"
        "adc     %[b], %[r];"
        "setb    %b[c];"
        : [r] "=&r" (r), [c] "+&r" (c)
        : [a] "r" (a), [b] "r" (b)
        : "cc"
    );

    *carry = c;
    return r;
}
