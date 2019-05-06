#ifndef LMP_SUB_MB
inline lmp_limb_t lmp_sub_mb(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap, size_t m, lmp_limb_t borrow)
{
    size_t i = 0;

    for (; i < m && borrow; i++) {
        lmp_limb_t a = ap[i];
        rp[i] = a - 1;
        if (a) break;
    }
    for (++i; i < m; i++) {
        rp[i] = ap[i];
        borrow = 0;
    }

    return borrow;
}
#endif
