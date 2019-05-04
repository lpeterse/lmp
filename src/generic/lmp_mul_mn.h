#ifndef LMP_MUL_MN
void lmp_mul_mn(
          lmp_limb_t *const restrict rp,
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    ASSERT(an > 1);
    ASSERT(bn > 1);

    lmp_dlimb_t ab;
    lmp_limb_t addc, mulc = 0;
    for (size_t bi = 0; bi < bn; bi++)
    {
        ab = (lmp_dlimb_t) ap[0] * bp[bi] + mulc;
        mulc = (lmp_limb_t) (ab >> LMP_LIMB_W);
        rp[bi] = (lmp_limb_t) ab;
    }
    rp[bn] = mulc;
    for (size_t ai = 1; ai < an; ai++)
    {
        addc = mulc = 0;
        for (size_t bi = 0; bi < bn; bi++)
        {
            ab = (lmp_dlimb_t) ap[ai] * bp[bi] + mulc;
            mulc = (lmp_limb_t) (ab >> LMP_LIMB_W);
            rp[ai + bi] = lmp_add_1c(rp[ai+bi], (lmp_limb_t) ab, &addc);
        }
        if (ai + 1 < an || mulc || addc)
        {
            rp[ai + bn] = addc + mulc;
        }
    }
}
#endif
