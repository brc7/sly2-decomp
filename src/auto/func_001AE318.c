#include "m2c_prelude.h"
M2C_UNK CheckForOutOfMemory__Fv(s32);               /* extern */
extern s32 D_002F6388;

/** @sly2-annot addr=0x1AE318
 * Purpose: Stack (scratch) heap allocator: D_002F6388 = (D_002F6388 - align16(cb)) & -cbAlign; grows down
 * Info: proposed name PvAllocStackImpl (EXISTING); TU memory
 * Notes: names_v2 v1 proposed PvAllocSwCopyImpl__FiPv (0.002) | agent/verifier proposes PvAllocStackImpl for
 *   this existing name (see existing_name_changes.csv) | RENAME proposed -> PvAllocStackImpl
 *   (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.90] - evidence: arbitration; static naming pass; pre-existing symbol in
 *   symbol_addrs.txt; sources: r5_1AE130+verdicts_tu_r3r5+arbitration
 * See: docs/engine/core-library.md#1-memory, docs/engine/core-library.md#7-key-functions,
 *   docs/engine/world-sw.md#10-key-functions
 * @end-sly2-annot */
s32 func_001AE318(void *arg0) {
    s32 temp_a0;
    s32 var_v0;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0);
    var_v0 = 0;
    if (temp_a0 != 0) {
        CheckForOutOfMemory__Fv(temp_a0);
        var_v0 = (D_002F6388 - ((M2C_FIELD(arg0, s32 *, 0) + 0xF) & ~0xF)) & -M2C_FIELD(arg0, s32 *, 4);
        D_002F6388 = var_v0;
    }
    return var_v0;
}
