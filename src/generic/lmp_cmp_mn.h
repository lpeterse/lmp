#ifndef LMP_CMP_MN
inline int lmp_cmp_mn(
    const lmp_limb_t *restrict ap, size_t an,
    const lmp_limb_t *restrict bp, size_t bn)
{
    if (an > bn) {
        return 1;
    }
    if (an < bn) {
        return -1;
    }

    return lmp_cmp_mm(ap, bp, an);
}
#endif
