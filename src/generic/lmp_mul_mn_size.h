#ifndef LMP_MUL_MN_SIZE
size_t lmp_mul_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    lmp_limb_t a = ap[an - 1];
    lmp_limb_t b = bp[bn - 1];

    return an + bn - ((LMP_LIMB_W - 1 - CLZ(a)) + (LMP_LIMB_W - 1 - CLZ(b)) + 1 < LMP_LIMB_W);
}
#endif
