#ifndef LMP_SETBIT
inline void lmp_setbit(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi)
{
    size_t ri = bi / LMP_LIMB_W;
    size_t wi = bi % LMP_LIMB_W;
    memcpy(rp, ap, an * LMP_LIMB_S);
    memset(rp + an, 0, (rn - an) * LMP_LIMB_S);
    rp[ri] = rp[ri] | (1 << wi);
}
#endif
