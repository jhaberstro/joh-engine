#ifndef JOH_PORTABILITY_PLATFORMS_MACOSX_HPP
#define JOH_PORTABILITY_PLATFORMS_MACOSX_HPP

#include <TargetConditionals.h>

#ifndef JOH_PLATFORM
#   if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#       define JOH_PLATFORM JOH_PLATFORM_IPHONE
#   elif defined(TARGET_OS_MAC)
#       define JOH_PLATFORM JOH_PLATFORM_MACOSX
#   else
#       error "Cannot detect platform!"
#   endif
#endif

#ifndef JOH_ARCHITECTURE
#   if TARGET_CPU_PPC
#       define JOH_ARCHITECTURE JOH_ARCHITECTURE_PPC
#   elif TARGET_CPU_PPC64
#       define JOH_ARCHITECTURE JOH_ARCHITECTURE_PPC64
#   elif TARGET_CPU_X86
#       define JOH_ARCHITECTURE JOH_ARCHITECTURE_X86
#   elif TARGET_CPU_X86_64
#       define JOH_ARCHITECTURE JOH_ARCHITECTURE_X64
#   elif TARGET_CPU_ARM
#       define JOH_ARCHITECTURE JOH_ARCHITECTURE_ARM
#   else
#       error "Unsupported architecture!"
#   endif
#endif

#ifndef JOH_ENDIAN
#   if TARGET_RT_LITTLE_ENDIAN
#       define JOH_ENDIAN JOH_ENDIAN_LITTLE
#   elif TARGET_RT_BIG_ENDIAN
#       define JOH_ENDIAN JOH_ENDIAN_BIG
#   else
#       error "Unsupported runtime!"
#   endif
#endif

#if !defined(JOH_ARCHITECTURE_32BIT) && !defined(JOH_ARCHITECTURE_64BIT)
#   if TARGET_RT_64_BIT
#       define JOH_ARCHITECTURE_64BIT
#   else
#       define JOH_ARCHITECTURE_32BIT
#   endif
#endif

#ifndef JOH_REAL
#   define JOH_REAL JOH_REAL_SINGLE
#endif

#if !defined(JOH_SIMD) || JOH_SIMD == JOH_SIMD_NONE
#   if JOH_ARCHITECTURE == JOH_ARCHITECTURE_X86 || JOH_ARCHITECTURE_X86 == JOH_ARCHITECTURE_X64
#       define JOH_SIMD JOH_SIMD_SSE 
#   elif JOH_ARCHITECTURE == JOH_ARCHITECTURE_ARM
#       ifdef __ARM_NEON__
#           define JOH_SIMD JOH_SIMD_NEON
#       else
#           define JOH_SIMD JOH_SIMD_VFP
#       endif
#   else
#       define JOH_SIMD JOH_SIMD_NONE
#   endif
#endif

#endif // JOH_PORTABILITY_PLATFORMS_MACOSX_HPP
