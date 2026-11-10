/* include/sly2/port.h -- portability hooks for compiling the decomp somewhere other than the EE.
 *
 * The matching build is the reference: on the EE every hook below expands to exactly what the code used before
 * (so codegen is unchanged); a native port gets a working default or overrides the hook before this header.
 *
 *   uptr, sptr      unsigned and signed integers as wide as a pointer, for address arithmetic (alignment, masks,
 *                   distances). The EE has 32-bit pointers but 64-bit long, so they are unsigned int and int there; a
 *                   host compiler that predefines __UINTPTR_TYPE__ / __INTPTR_TYPE__ (gcc >= 4.5, clang) gets
 *                   uintptr_t's and intptr_t's types. Override: SLY2_UPTR_TYPE, SLY2_SPTR_TYPE. Replace a cast to int
 *                   with sptr and a cast to unsigned with uptr: the signedness picks the EE instructions (an
 *                   unsigned `& ~0x3F` needs a 64-bit mask constant), so swapping one for the other changes codegen.
 *   SLY2_EE_ASM     defined when inline EE assembly may be used. Functions whose asm only pins a register or an
 *                   instruction for the match keep it under #ifdef SLY2_EE_ASM, with a plain C equivalent in #else.
 *                   Defined on the R5900 compiler unless SLY2_NO_EE_ASM is.
 *
 * Hardware addresses are in sly2/hw.h. Static layout asserts throughout the headers still describe the EE. */
#ifndef SLY2_PORT_H
#define SLY2_PORT_H

#ifndef SLY2_UPTR_TYPE
#ifdef __UINTPTR_TYPE__
#define SLY2_UPTR_TYPE __UINTPTR_TYPE__
#else
#define SLY2_UPTR_TYPE unsigned int
#endif
#endif
#ifndef SLY2_SPTR_TYPE
#ifdef __INTPTR_TYPE__
#define SLY2_SPTR_TYPE __INTPTR_TYPE__
#else
#define SLY2_SPTR_TYPE int
#endif
#endif
typedef SLY2_UPTR_TYPE uptr;
typedef SLY2_SPTR_TYPE sptr;
typedef char assert_uptr_size[sizeof(uptr) == sizeof(void *) && sizeof(sptr) == sizeof(void *) ? 1 : -1];   /* every target */

#if defined(__R5900__) && !defined(SLY2_NO_EE_ASM)
#define SLY2_EE_ASM 1
#endif

#endif /* SLY2_PORT_H */
