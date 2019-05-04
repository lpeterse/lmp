#ifndef LMP_RSHIFT
void lmp_rshift(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bits)
{
    const int is_shift_by_words = !(bits % LMP_LIMB_W);
    const int is_shift_by_bytes = !(bits % 8) && is_little_endian();
    if (!rn) {
        // Do nothing.
    } else if (is_shift_by_words) {
        memcpy(rp, ap + bits / LMP_LIMB_W, rn * LMP_LIMB_S);
    } else if (is_shift_by_bytes) {
        const size_t len = an * LMP_LIMB_S - bits / 8 - CLZ(ap[an - 1]) / 8;
        rp[rn - 1] = 0;
        memcpy(rp, ((char*) ap) + bits / 8, len);
    } else {
        const size_t sl = bits / LMP_LIMB_W;
        const size_t sb = bits % LMP_LIMB_W;
        lmp_limb_t x = ap[sl] >> sb;
        for (size_t ai = sl + 1; ai < an; ai++) {
            rp[ai - sl - 1] = x | (ap[ai] << (LMP_LIMB_W - sb));
            x = ap[ai] >> sb;
        }
        if (x) {
            rp[rn - 1] = x;
        }
    }
}
#endif
