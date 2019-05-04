#ifndef LMP_RSHIFT_SIZE
size_t lmp_rshift_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bits)
{
    const size_t sl = bits / LMP_LIMB_W;
    const size_t sb = bits % LMP_LIMB_W;
    return an - MIN(an, sl + !(ap[an - 1] >> sb));
}
#endif
