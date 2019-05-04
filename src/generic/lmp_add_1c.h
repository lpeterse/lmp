#ifndef LMP_ADD_1C
lmp_limb_t lmp_add_1c(
    const lmp_limb_t a, const lmp_limb_t b, lmp_limb_t *carry)
{
#if __has_feature(__builtin_addcl)
    return __builtin_addcl(a, b, carry, &carry);
#else
    lmp_limb_t x = a + b;
    if (x < a) {
        x += *carry;
        *carry = 1;
    } else {
        x += *carry;
        *carry = !x && *carry;
    }
    return x;
#endif
}
#endif