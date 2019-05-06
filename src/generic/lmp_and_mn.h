#ifndef LMP_AND_MN
inline void lmp_and_mn(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp)
{
    for(size_t ri = 0;  ri < rn; ri++) {
        rp[ri] = ap[ri] & bp[ri];
    }
}
#endif
