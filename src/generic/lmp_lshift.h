#ifndef LMP_LSHIFT
void lmp_lshift(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bits)
{
    ASSERT(rn > 0);
    ASSERT(an > 0);
    ASSERT(rn >= an);

    const int is_shift_by_words = !(bits % LMP_LIMB_W);
    const int is_shift_by_bytes = !(bits % 8) && is_little_endian();

    // This optimisation does a wordsize aligned memcpy and
    // is eventually vectorized.
    if (is_shift_by_words) {
        memset(rp, 0, bits / 8);
        memcpy((char*)rp + bits / 8, ap, an * LMP_LIMB_S);
        return;
    }

    // Doing a memcpy to implement the shift is only allowed if
    // the machine is little endian (due to memory representation).
    if (is_shift_by_bytes) {
        const lmp_limb_t z = ap[an - 1];
        const lmp_limb_t x = z << (bits % LMP_LIMB_W);
        const lmp_limb_t y = z >> (LMP_LIMB_W - bits % LMP_LIMB_W);
        if (y) {
            rp[rn - 2] = x;
            rp[rn - 1] = y;
        } else {
            rp[rn - 1] = x;
        }
        memset(rp, 0, bits / 8);
        memcpy((char*)rp + bits / 8, ap, (an - 1) * LMP_LIMB_S);
        return;
    }

    // Generic case:
    //   - initialise the lower limbs with 0
    //   - word-wise copy and shift from ap
    //   - the highest limb is only assigned when it's non-zero
    const size_t sl = bits / LMP_LIMB_W; // 1
    const size_t sb = bits % LMP_LIMB_W; // 1
    for (size_t ri = 0; ri < sl; ri++) {
        rp[ri] = 0; // one iteration
    }
    lmp_limb_t carry = 0;
    for (size_t ai = 0; ai < an; ai++) { // one iteration
        rp[ai + sl] = (ap[ai] << sb) | carry;
        carry = ap[ai] >> (LMP_LIMB_W - sb);
    }
    if (carry) {
        rp[rn - 1] = carry;
    }
    return;
}
#endif
