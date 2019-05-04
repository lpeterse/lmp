#define LMP_CPU_FEATURE_POPCNT (1UL << 23)

static inline unsigned long lmp_cpu_get_features()
{
    unsigned long r;

    __asm__ (
        // Save clobbered registers on stack in order to not having to
        // declare them which turned out to confuse the register allocator.
        "pushq   %%rbx;"
        "pushq   %%rcx;"
        "pushq   %%rdx;"
        "xor     %%rcx, %%rcx;"
        "xor     %%rdx, %%rdx;"
        // Call CPUID function 1.
        "mov     $1, %%eax;"
        "cpuid;"
        "mov     %%edx, %%eax;"
        "shl     $32, %%rax;"
        "xor     %%rcx, %%rax;"
        // Restore registers.
        "popq    %%rdx;"
        "popq    %%rcx;"
        "popq    %%rbx;"
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
