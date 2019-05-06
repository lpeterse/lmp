#ifndef LMP_POPCOUNT_1
inline size_t lmp_popcount_1(lmp_limb_t a)
{
    return __builtin_popcountl(a);
}
#endif