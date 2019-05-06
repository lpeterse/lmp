#ifndef LMP_ADD_MNC
inline lmp_limb_t lmp_add_mnc(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap, size_t an,
    const lmp_limb_t *restrict bp, size_t bn, lmp_limb_t c)
{
    ASSERT(an > 0);
    ASSERT(bn > 0);
    ASSERT(an >= bn);
    ASSERT(c <= 1);

    lmp_limb_t co = lmp_add_mmc(rp, ap, bp, bn, c);
    return lmp_add_mc(&rp[bn], &ap[bn], an - bn, co);
}
#endif
