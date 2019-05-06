#ifndef LMP_ADD_MMC
inline lmp_limb_t lmp_add_mmc(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap,
    const lmp_limb_t *restrict bp, size_t m, lmp_limb_t carry)
{
    for (size_t i = 0; i < m; i++) {
        lmp_dlimb_t x = carry;
        x += ap[i];
        x += bp[i];
        rp[i] = (lmp_limb_t) x;
        carry = (lmp_limb_t) (x >> LMP_LIMB_W);
    }

    return carry;
}
#endif
