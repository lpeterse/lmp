#ifndef LMP_ADD_MN_SIZE
size_t lmp_add_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    ASSERT(an > 0);
    ASSERT(bn > 0);

    if (bn > an) {
        return lmp_add_mn_size(bp, bn, ap, an);
    }

    size_t i = an - 1;
    for (; i >= bn; i--) {
        if (ap[i] != LMP_LIMB_MAX) {
            // Overflow impossible independant of other limbs.
            return an;
        }
        // Overflow depends on carry from less signifcant limbs. Continue...
    }
    for (; i > 0; i--) {
        lmp_limb_t a = ap[i];
        lmp_limb_t b = bp[i];
        lmp_limb_t c = a + b;
        if (c < a) {
            // Overflow inevitable independant of less significant limbs.
            return an + 1;
        }
        if (c != LMP_LIMB_MAX) {
            // Overflow impossible independant of other limbs.
            return an;
        }
        // Overflow depends on carry from less signifcant limbs. Continue...
    }
    return an + (ap[0] + bp[0] < ap[0]);
}
#endif
