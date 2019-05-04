#ifndef LMP_IOR_MN_SIZE
size_t lmp_ior_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    return MIN(an, bn);
}
#endif
