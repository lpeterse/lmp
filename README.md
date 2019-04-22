# lmp

`lmp` is a BSD licensed multiprecision integer library.

## Purpose

This library is an experiment towards offering an alternative large integer implementation for
the [Glasgow Haskell Compiler](https://gitlab.haskell.org/ghc/ghc).

See [replacing-gmp-notes](https://gitlab.haskell.org/ghc/ghc/wikis/replacing-gmp-notes)
for more motivation behind replacing GMP.

## Design

### Completeness over premature optimization

Getting a working prototype which implements all functions required by GHC is the primary goal.

### No assembly (if possible)

The code shall be compliant C99. Compiler intrinsics may be used if supported by both clang and
gcc and their use yields a significant benefit. Compiler intrinsics shall be used in favour of
switching to assembly language as they maintain architecture independence even if not compiling
to efficient primitives on all architectures. Having a single code path executed and tested on
all architectures also reduces maintenance burden and the likelyhood of architecture specific bugs.

### Performance

Performance is nice, but not a primary goal for now. It would be hybris to try to compete with GMP.
It is considered good enough to stay in the same algorithmic complexity classes and stay within the
same order of magnitude wrt to performance.

C compilers are incredibly smart these days. It shall be assumed that this library is used with
recent compiler versions and optimisations turned on.

### Architecture specific extendsions (AVX, SSE etc)

Some extensions have the potential for massive perfomance gains, but they are not available on
all CPUs.

This topic may be explored in a second step unless it's a low hanging fruit (auto-vectorization).

### Memory allocation and interfacing with GHC

One notable point is that ....

## License

BSD3
