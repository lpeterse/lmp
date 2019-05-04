#ifndef LMP_TESTBIT
size_t lmp_testbit(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi)
{
    size_t ai = bi / LMP_LIMB_W;
    size_t wi = bi % LMP_LIMB_W;
    return ai < an && (ap[ai] & (1 << wi));
}
#endif
