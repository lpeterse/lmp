#include <limits.h>
#include "lmp.h"

#if __WORDSIZE == 64
#define msb(x) (LMP_LIMB_WIDTH - 1 - __builtin_clzll(x))
#else
#define msb(x) (LMP_LIMB_WIDTH - 1 - __builtin_clzl(x))
#endif

size_t lmp_mul_size(
    lmp_limb_t *ap, size_t an,
    lmp_limb_t *bp, size_t bn
) {
    lmp_limb_t a = ap[an - 1];
    lmp_limb_t b = bp[bn - 1];
    return an + bn - (msb(a) + msb(b) + 1 < 64);
}

