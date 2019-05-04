#ifndef LMP_CMP_MM
int lmp_cmp_mm(
    const lmp_limb_t *restrict ap,
    const lmp_limb_t *restrict bp, size_t m)
{
    while (m--) {
        if (ap[m] != bp[m]) {
            if (ap[m] > bp[m]) {
                return 1;
            } else {
                return -1;
            };
        }
    }

    return 0;
}
#endif
