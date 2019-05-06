#ifndef LMP_MUL_M1
inline lmp_limb_t lmp_mul_m1(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap, size_t m, lmp_limb_t b)
{
    lmp_dlimb_t x = 0;
    for (size_t i = 0; i < m; i++)
    {
        x += (lmp_dlimb_t) ap[i] * b;
        rp[i] = (lmp_limb_t) x;
        x >>= LMP_LIMB_W;
    }
    return (lmp_limb_t) x;
}
#endif
