#ifndef _PSQLXX_OVERLOAD_HXX
#define _PSQLXX_OVERLOAD_HXX

#ifdef __MSVC_COMPILER__
#define PSQLXX_INLINE __forceinline

#define PSQLXX_NOINLINE __declspec(noinline)
#elif __CLANG_COMPILER__ || __GCC_COMPILER__ || __INTEL_COMPILER__
#define PSQLXX_INLINE __attribute__((always_inline)) inline

#define PSQLXX_NOINLINE __attribute__((noinline))
#else
#define PSQLXX_INLINE inline

#define PSQLXX_NOINLINE
#endif

namespace psqlxx {
    // ...
}

#endif // _PSQLXX_OVERLOAD_HXX