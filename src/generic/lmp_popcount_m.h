#ifndef LMP_POPCOUNT_M
inline size_t lmp_popcount_m(
    const lmp_limb_t *restrict ap, size_t an)
{
    size_t r = 0;

    for (; an > 0; an--) {
        r += lmp_popcount_1(ap[an - 1]);
    }

    return r;
}
#endif
