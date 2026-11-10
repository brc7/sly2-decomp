/* include/sly2/hw.h -- the PS2 hardware addresses game code reads or writes directly, named once.
 *
 * Each address is an integer constant (use it through a cast: `(char *)SLY2_HW_SCRATCHPAD`, `*(volatile int *)
 * SLY2_HW_GIF_MODE`), so on the EE the expansion is the literal the code always had and codegen is unchanged.
 * Every macro is #ifndef-guarded: a port that emulates the memory map defines the ones it needs first, e.g. in a
 * header named by SLY2_HW_OVERRIDE (-DSLY2_HW_OVERRIDE='"port_hw.h"'), which this header includes before its
 * defaults:
 *
 *   extern "C" unsigned char g_ps2_spr[0x4000];
 *   #define SLY2_HW_SCRATCHPAD ((uptr)g_ps2_spr)
 *   #define SLY2_UNCACHED(a)   (a)
 *
 * Addresses inside inline-asm strings (gs.cpp's privileged-register writers) are not covered: those functions are
 * EE-only anyway. */
#ifndef SLY2_HW_H
#define SLY2_HW_H
#include "sly2/port.h"

#ifdef SLY2_HW_OVERRIDE
#include SLY2_HW_OVERRIDE
#endif

/* Scratchpad RAM (16 KB). The game splits it into two 8 KB halves for double-buffered DMA (sprbuf.cpp) and uses
   the first 16 KB as the LZSS window of decompressing streams (SetDecompress). */
#ifndef SLY2_HW_SCRATCHPAD
#define SLY2_HW_SCRATCHPAD 0x70000000
#endif

/* Uncached and uncached-accelerated mirrors of main RAM: an address with these bits set bypasses the data cache, so
   DMA sees CPU writes without a flush. A port with a flat address space defines both as (a). */
#ifndef SLY2_UNCACHED
#define SLY2_UNCACHED(a) ((a) | 0x20000000)
#endif
#ifndef SLY2_UNCACHED_ACCEL
#define SLY2_UNCACHED_ACCEL(a) ((a) | 0x30000000)
#endif

/* GS privileged register CSR (system status: FINISH, VSINT, FIELD...). */
#ifndef SLY2_HW_GS_CSR
#define SLY2_HW_GS_CSR 0x12001000
#endif

/* GIF_MODE (GIF control: path-3 masking / intermittent mode). */
#ifndef SLY2_HW_GIF_MODE
#define SLY2_HW_GIF_MODE 0x10003010
#endif

/* VIF1_FBRST (VIF1 reset / force break / stall cancel). */
#ifndef SLY2_HW_VIF1_FBRST
#define SLY2_HW_VIF1_FBRST 0x10003C10
#endif

/* IPU_TO DMA channel (channel 4): control, memory address and quadword count. */
#ifndef SLY2_HW_IPU_TO_CHCR
#define SLY2_HW_IPU_TO_CHCR 0x1000B400
#endif
#ifndef SLY2_HW_IPU_TO_MADR
#define SLY2_HW_IPU_TO_MADR 0x1000B410
#endif
#ifndef SLY2_HW_IPU_TO_QWC
#define SLY2_HW_IPU_TO_QWC 0x1000B420
#endif

#endif /* SLY2_HW_H */
