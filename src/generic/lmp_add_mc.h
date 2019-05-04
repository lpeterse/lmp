#ifndef LMP_ADD_MC
lmp_limb_t lmp_add_mc(
          lmp_limb_t *restrict rp,
    const lmp_limb_t *restrict ap, size_t m, lmp_limb_t c)
{
    size_t i = 0;

    for (; i < m && c; i++) {
        lmp_limb_t a = rp[i] = ap[i] + 1;
        if (a) break;
    }
    for (++i; i < m; i++) {
        rp[i] = ap[i];
        c = 0;
    }

    return c;
}
#endif
