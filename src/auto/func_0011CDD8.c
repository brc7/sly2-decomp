#include "m2c_prelude.h"
s32 PvAllocSlotheapClear();                                /* extern */
M2C_UNK func_0011CD98(s32, s32);                    /* extern */

/** @sly2-annot addr=0x11CDD8
 * Purpose: Allocates and immediately frees one slot (forces the block allocation now)
 * Info: proposed name PrimeSlotheap (T2); TU slotheap
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#slotheap-0x11cca8-0x11cf60
 * @end-sly2-annot */
void func_0011CDD8(s32 arg0) {
    func_0011CD98(arg0, PvAllocSlotheapClear());
}
