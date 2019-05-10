#define LMP_CPU_FEATURE_POPCNT (1UL << 23)
#define LMP_CPU_FEATURE_ADX    (1UL << 19)

static unsigned long lmp_cpu_get_features()
{
    unsigned long r;

    __asm__ volatile (
        // Save clobbered registers on stack in order to not having to
        // declare them which turned out to confuse the register allocator.
        "pushq   %%r8;"
        "pushq   %%rbx;"
        "pushq   %%rcx;"
        "pushq   %%rdx;"
        // Set the lowest bit in order to mark cpuid as executed.
        "movq    $1, %%r8;"
        // Call CPUID function 1.
        "mov     $1, %%eax;"
        "xor     %%rcx, %%rcx;"
        "xor     %%rdx, %%rdx;"
        "cpuid;"
        "and     $8388608, %%rcx;" // Filter for POPCNT extension flag
        "or      %%rcx, %%r8;"
        // Call CPUID function 7.
        "mov     $7, %%eax;"
        "xor     %%rcx, %%rcx;"
        "xor     %%rdx, %%rdx;"
        "cpuid;"
        "and     $524288, %%rbx;" // Filter for ADX extension flag
        "or      %%rbx, %%r8;"
        // Restore registers.
        "movq    %%r8, %%rax;"
        "popq    %%rdx;"
        "popq    %%rcx;"
        "popq    %%rbx;"
        "popq    %%r8;"
        : "=a" (r)
        :: "cc"
    );

    return r;
}

static inline unsigned long lmp_cpu_has_feature(unsigned long feature)
{
    static unsigned long lmp_cpu_features;

    if (!lmp_cpu_features) {
        lmp_cpu_features = lmp_cpu_get_features();
    }

    return lmp_cpu_features & feature;
}
