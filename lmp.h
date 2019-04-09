#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#if __WORDSIZE == 64
typedef unsigned long long lmp_limb_t;
#define LMP_LIMB_WIDTH __LONG_LONG_WIDTH__

#elif __WORDSIZE == 32
typedef unsigned long lmp_limb_t;
#define LMP_LIMB_WIDTH __LONG_WIDTH__
#endif

#define LMP_LIMB_SIZE (LMP_LIMB_WIDTH / 8)

size_t lmp_mul_size(lmp_limb_t *ap, size_t an, lmp_limb_t *bp, size_t bn);
