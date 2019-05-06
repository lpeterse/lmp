#ifndef LMP_MUL_M1_ADD
inline lmp_limb_t lmp_mul_m1_add(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap, size_t m, lmp_limb_t b)
{
    lmp_dlimb_t ab;
    lmp_limb_t addc = 0, mulc = 0;
    for (size_t i = 0; i < m; i++)
    {
        ab = (lmp_dlimb_t) ap[i] * b + mulc;
        mulc = (lmp_limb_t) (ab >> LMP_LIMB_W);
        rp[i] = lmp_add_1c(rp[i], (lmp_limb_t) ab, &addc);
    }
    return addc + mulc;
}
#endif
