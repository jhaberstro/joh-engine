#ifndef JOH_PORTABILITY_COMPILEJOH_GCC_HPP
#define JOH_PORTABILITY_COMPILEJOH_GCC_HPP

#include <climits>

#define JOH_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if JOH_GCC_VERSION < 40000
#   error "Unsupported GCC version!"
#endif
#undef JOH_GCC_VERSION

#ifndef JOH_COMPILER
#   define JOH_COMPILER JOH_COMPILER_GCC
#endif

#ifndef JOH_EXIT
#   define JOH_EXIT __builtin_trap
#endif

//#ifndef JOH_PUSH_PACK
//#   define JOH_PUSH_PACK(x) #pragma pack(push, x)
//#endif

//#ifndef JOH_POP_PACK
//#   define JOH_POP_PACK #pragma pack(pop)
//#endif

#ifndef JOH_ALIGN
#   define JOH_ALIGN(declaration, alignment) declaration __attribute__((aligned(alignment)))
#endif

#ifndef JOH_FORCE_INLINE
#   define JOH_FORCE_INLINE __inline __attribute__((always_inline))
#endif

#ifndef JOH_RESTRICT
#   define JOH_RESTRICT __restrict
#endif

#ifndef JOH_UNUSED
#   define JOH_UNUSED(expression) ((void)sizeof((expression)))
#endif

#endif // JOH_PORTABILITY_COMPILEJOH_GCC_HPP
