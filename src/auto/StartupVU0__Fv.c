#include "m2c_prelude.h"
M2C_UNK FlushCache(M2C_UNK);                        /* extern */
M2C_UNK func_00241728(s32 *, M2C_UNK *);            /* extern */
M2C_UNK func_002417F0(s32 *, M2C_UNK, M2C_UNK);     /* extern */
extern M2C_UNK D_00259DF0;
extern s32 *D_002DE590;

/** @sly2-annot addr=0x1A7890
 * Purpose: DMA VU0 microcode (D_00259DF0) via sceDmaSend/sceDmaSync
 * Info: proposed name StartupVU0__Fv (EXISTING); TU main
 * Confidence: high [EXISTING 0.95] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/main-loop-startup.md#1-maincpp-0x1a7518-0x1a7c18,
 *   docs/engine/main-loop-startup.md#8-key-functions, docs/engine/overview.md#1-boot
 * @end-sly2-annot */
void StartupVU0__Fv(void) {
    *D_002DE590 |= 0x40;
    FlushCache(0);
    func_00241728(D_002DE590, &D_00259DF0);
    func_002417F0(D_002DE590, 0, 0);
    *D_002DE590 &= ~0x40;
}
