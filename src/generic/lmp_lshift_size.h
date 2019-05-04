#ifndef LMP_LSHIFT_SIZE
size_t lmp_lshift_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bits)
{
    size_t x = bits / LMP_LIMB_W;
    size_t y = bits % LMP_LIMB_W;
    return an
        ? an + x + (LMP_LIMB_W - CLZ(ap[an - 1]) + y > LMP_LIMB_W)
        : 0;
}
#endif
