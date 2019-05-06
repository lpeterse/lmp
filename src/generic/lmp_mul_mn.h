#ifndef LMP_MUL_MN
inline void lmp_mul_mn(
          lmp_limb_t *const restrict rp,
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    ASSERT(an > 1);
    ASSERT(bn > 1);

    rp[bn] = lmp_mul_m1(rp, bp, bn, ap[0]);
    for (size_t ai = 1; ai < an; ai++)
    {
        lmp_limb_t mulc = lmp_mul_m1_add(rp + ai, bp, bn, ap[ai]);
        if (ai + 1 < an || mulc)
        {
            rp[ai + bn] = mulc;
        }
    }
}
#endif
