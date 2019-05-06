static inline size_t lmp_popcount_1_native(lmp_limb_t a)
{
    size_t r;

    __asm__ (
        "popcnt %[r], %[a];"
        : [r] "=r" (r)
        : [a] "r" (a)
        : "cc"
    );

    return r;
}

#define LMP_POPCOUNT_M
inline size_t lmp_popcount_m(
    const lmp_limb_t *restrict ap, size_t an)
{
    size_t r = 0;

    if (lmp_cpu_has_feature(LMP_CPU_FEATURE_POPCNT)) {
        for (; an > 0; an--) {
            r += lmp_popcount_1_native(ap[an - 1]);
        }
    } else {
        for (; an > 0; an--) {
            r += __builtin_popcountl(ap[an - 1]);
        }
    }

    return r;
}
