#ifndef LMP_CLEARBIT_SIZE
inline size_t lmp_clearbit_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi)
{
    size_t ai = bi / LMP_LIMB_W;
    size_t wi = bi % LMP_LIMB_W;
    if (ai + 1 == an && !(ap[ai] & ~(1 << wi)))
    {
        size_t rn = an - 1;
        while (rn > 0 && !ap[rn - 1]) rn--;
        return rn;
    }
    return an;
}
#endif
