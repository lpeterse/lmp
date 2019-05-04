#ifndef LMP_POPCOUNT
size_t lmp_popcount(
    const lmp_limb_t *restrict ap, size_t an)
{
    size_t r = 0;

    for (; an > 0; an--) {
        r += POPCOUNT(ap[an - 1]);
    }

    return r;
}
#endif
