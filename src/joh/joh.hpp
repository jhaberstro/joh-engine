#ifndef JOH_HPP
#define JOH_HPP

#define JOH_PLATFORM_MACOSX      1
#define JOH_PLATFORM_IPHONE      2
#define JOH_PLATFORM_WINDOWS     3
#define JOH_PLATFORM_LINUX       4

#define JOH_COMPILER_GCC         1
#define JOH_COMPILER_CLANG       2

#define JOH_ARCHITECTURE_ARM     1
#define JOH_ARCHITECTURE_X86     2
#define JOH_ARCHITECTURE_X64     3
#define JOH_ARCHITECTURE_PPC     4
#define JOH_ARCHITECTURE_PPC64   5

#define JOH_LITTLE_ENDIAN        1
#define JOH_BIG_ENDIAN           2

#define JOH_REAL_SINGLE          1
#define JOH_REAL_DOUBLE          2

#define JOH_SIMD_NONE            1
#define JOH_SIMD_SSE             2
#define JOH_SIMD_NEON            3
#define JOH_SIMD_VFP             4

#if defined(__GNUC__)
    #define alignof __alignof__
#else
    #error "Unsupported compiler for alignof."
#endif

#if defined(__APPLE__) || defined(__APPLE_CC__)
#   include "joh/portability/platforms/MacOSX.hpp"
#else
#   error "Platform not supported!"
#endif

#if defined(__GNUC__)
#   include "joh/portability/compilers/GCC.hpp"
#else
#   error "Compiler not supported!"
#endif

// I'm lazy, let's just hope the platform has this for us... for now :)
#include <stdint.h>
#include <cstddef>

#endif // JOH_HPP
