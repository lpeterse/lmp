#ifndef LMP_SUB_MN_SIZE
static inline size_t lmp_sub_xx_size(
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp, const size_t n)
{
    for (size_t i = n; i > 0; i--) {
        const lmp_limb_t a = ap[i - 1];
        const lmp_limb_t b = bp[i - 1];
        if (a > b) return n + 1;
        if (a < b) return i;
    }
    return n + 1;
}

static inline size_t lmp_sub_nn_size(
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp, const size_t n)
{
    ASSERT(n > 0);
    for (size_t i = n; i > 1; i--) {
        const lmp_limb_t a = ap[i - 1];
        const lmp_limb_t b = bp[i - 1];
        if (a == b + 1) return lmp_sub_xx_size(ap, bp, i - 1);
        if (b == a + 1) return lmp_sub_xx_size(bp, ap, i - 1);
        if (a != b)     return i;
    }
    return ap[0] != bp[0];
}

size_t lmp_sub_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn)
{
    ASSERT(an > 0);
    ASSERT(bn > 0);

    if (an > bn + 1) {
        return an;
    }
    if (bn > an + 1) {
        return bn;
    }
    if (an == bn + 1) {
        return ap[an - 1] > 1 ? an : lmp_sub_xx_size(ap, bp, bn);
    }
    if (bn == an + 1) {
        return bp[bn - 1] > 1 ? bn : lmp_sub_xx_size(bp, ap, an);
    }

    ASSERT(an == bn);

    return lmp_sub_nn_size(ap, bp, an);
}
#endif
