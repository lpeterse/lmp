#ifndef LMP_SUB_MN
inline void lmp_sub_mn(
          lmp_limb_t *restrict rp, size_t rn,
    const lmp_limb_t *restrict ap, size_t an,
    const lmp_limb_t *restrict bp, size_t bn)
{
    ASSERT(an > 0);
    ASSERT(bn > 0);
    ASSERT(an >= bn);

    if (!rn) {
        // Do nothing.
    } else if (an == bn) {
        // Borrow out does not occur due to invariant.
        lmp_sub_mmb(rp, ap, bp, rn, 0);
    } else {
        lmp_limb_t borrow = lmp_sub_mmb(rp, ap, bp, bn, 0);
        lmp_sub_mb(rp + bn, ap + bn, an - bn, borrow);
    }
}
#endif
