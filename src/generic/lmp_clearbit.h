#ifndef LMP_CLEARBIT
inline void lmp_clearbit(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi)
{
    size_t ai = bi / LMP_LIMB_W;
    size_t wi = bi % LMP_LIMB_W;
    memcpy(rp, ap, rn * LMP_LIMB_S);
    if (ai < rn) rp[ai] = rp[ai] & ~(1 << wi);
}
#endif
