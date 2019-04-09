#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

// It is considered necessary to check the value of ULLONG_MAX
// here as a precondition for all overflow related operations.
#if __WORDSIZE == 64 && ULLONG_MAX == 18446744073709551615ULL
typedef unsigned long long lmp_limb_t;
typedef __uint128_t lmp_dlimb_t;
#define LMP_LIMB_S 8
#define LMP_LIMB_W __WORDSIZE
#define LMP_LIMB_C(c) c ## ULL
#define LMP_MSB(x) (LMP_LIMB_W - 1 - __builtin_clzll(x))

#elif __WORDSIZE == 32 && ULONG_MAX == 4294967295UL
typedef unsigned long lmp_limb_t;
typedef unsigned int lmp_dlimb_t;
#define LMP_LIMB_S 4
#define LMP_LIMB_W __WORDSIZE
#define LMP_LIMB_C(c) c ## UL
#define LMP_MSB(x) (LMP_LIMB_W - 1 - __builtin_clzl(x))

#else
#error "unknown machine word size"
#endif

#define ASSERT(x) // assert(x)

/* Multiplication */
size_t lmp_mul_size(lmp_limb_t *ap, size_t an, lmp_limb_t *bp, size_t bn);
void lmp_mul_n1(lmp_limb_t *rp, lmp_limb_t *ap, size_t an, lmp_limb_t b);
void lmp_mul_nm(lmp_limb_t *rp, lmp_limb_t *ap, size_t an, lmp_limb_t *bp, size_t bn);

