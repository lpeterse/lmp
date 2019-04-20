#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define LIMB_MIN         LMP_LIMB_C(0x0000000000000000)
#define LIMB_MIN_PP      LMP_LIMB_C(0x0000000000000001)
#define LIMB_MAX         LMP_LIMB_C(0xFFFFFFFFFFFFFFFF)
#define LIMB_MAX_MM      LMP_LIMB_C(0xFFFFFFFFFFFFFFFE)
#define LIMB_HALF_FULL   LMP_LIMB_C(0x00000000FFFFFFFF)
#define LIMB_HALF_FULL_1 LMP_LIMB_C(0x0000000100000000)
#define LIMB_INI         LMP_LIMB_C(0x2323232323232323)

// It is considered necessary to check the value of ULLONG_MAX
// here as a precondition for all overflow related operations.
#if __WORDSIZE == 64 && ULONG_MAX == 18446744073709551615ULL
typedef unsigned long lmp_limb_t;
typedef __uint128_t lmp_dlimb_t;
#define LMP_LIMB_S 8
#define LMP_LIMB_W __WORDSIZE
#define LMP_LIMB_C(c) c ## UL
#define LMP_LIMB_MAX ULONG_MAX
#define LMP_MSB(x) (LMP_LIMB_W - 1 - __builtin_clzl(x))
#define LMP_ADDC(x,y,ci,co) __builtin_addcl(x,y,ci,co)

#elif __WORDSIZE == 32 && ULONG_MAX == 4294967295UL
typedef unsigned long lmp_limb_t;
typedef unsigned int lmp_dlimb_t;
#define LMP_LIMB_S 4
#define LMP_LIMB_W __WORDSIZE
#define LMP_LIMB_C(c) c ## UL
#define LMP_LIMB_MAX ULONG_MAX
#define LMP_MSB(x) (LMP_LIMB_W - 1 - __builtin_clzl(x))

#else
#error "unknown machine word size"
#endif

/*****************************************************************************
 * Multiplication
 *****************************************************************************/

void lmp_mul_m1(
          lmp_limb_t *const restrict rp,
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t b);

size_t lmp_mul_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn);

void lmp_mul_mn(
          lmp_limb_t *const restrict rp,
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn);

/*****************************************************************************
 * Shift operations
 *****************************************************************************/

 size_t lmp_lshift_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bits);

 void lmp_lshift(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bits);

/*****************************************************************************
 * Bitwise operations 
 *****************************************************************************/

size_t lmp_ior_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn);

void lmp_ior_mn(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp);

size_t lmp_xor_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn);

void lmp_xor_mn(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp);

size_t lmp_and_mn_size(
    const lmp_limb_t *const restrict ap, const size_t an,
    const lmp_limb_t *const restrict bp, const size_t bn);

void lmp_and_mn(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap,
    const lmp_limb_t *const restrict bp);

size_t lmp_testbit(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi);

size_t lmp_setbit_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi);

void lmp_setbit(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi);

size_t lmp_clearbit_size(
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi);

void lmp_clearbit(
          lmp_limb_t *const restrict rp, const size_t rn,
    const lmp_limb_t *const restrict ap, const size_t an,
                                         const size_t bi);

size_t lmp_popcount(
    const lmp_limb_t *const restrict ap, const size_t an);

