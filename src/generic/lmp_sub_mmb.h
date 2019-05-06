#ifndef LMP_SUB_MMB
inline lmp_limb_t lmp_sub_mmb(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap,
    const lmp_limb_t *restrict bp, size_t m, lmp_limb_t borrow)
{
    size_t i = 0;

    lmp_dlimb_t x = -borrow;
    for (; i < m; i++) {
        x += ap[i];
        x &= LMP_LIMB_MAX; // ignore carry from previous addition
        x -= bp[i];
        rp[i] = (lmp_limb_t) x;
        x >>= LMP_LIMB_W;
    }

    return -((lmp_limb_t) x);
}
#endif
