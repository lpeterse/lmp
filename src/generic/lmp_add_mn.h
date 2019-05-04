#ifndef LMP_ADD_MN
void lmp_add_mn(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap, size_t an,
    const lmp_limb_t *restrict bp, size_t bn)
{
    ASSERT(an > 0);
    ASSERT(bn > 0);
    ASSERT(an >= bn);

    lmp_limb_t c = lmp_add_mnc(rp, ap, an, bp, bn, 0);
    if (c) {
        rp[MAX(an, bn) - 1] = 1;
    }
}
#endif
