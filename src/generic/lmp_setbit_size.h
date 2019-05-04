#ifndef LMP_SETBIT_SIZE
size_t lmp_setbit_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi)
{
    size_t ai = bi / LMP_LIMB_W;
    return MAX(an, ai + 1);
}
#endif
