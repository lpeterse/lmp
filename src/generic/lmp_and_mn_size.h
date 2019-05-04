#ifndef LMP_AND_MN_SIZE
size_t lmp_and_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    size_t rn = MIN(an, bn);
    while (rn > 0 && !(ap[rn - 1] & bp[rn - 1])) rn--;
    return rn;
}
#endif
