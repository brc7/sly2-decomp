#include "m2c_prelude.h"
M2C_UNK func_00182C70(s32);                         /* extern */
M2C_UNK func_001E5D80();                            /* extern */

/** @sly2-annot addr=0x1CB908
 * Purpose: RAIL::Init (vt7): InitShape + init GRABSPOT base at +0x190; +0x1C4=1, +0x1C0=1000
 * Info: proposed name InitRail (T1); class RAIL slot7; TU rail
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/object-model.md#8-source-files, docs/engine/player.md#10-source-files,
 *   docs/reference/functions/player.md#rail-0x1cb908-0x1cbeb0
 * @end-sly2-annot */
void func_001CB908(void *arg0) {
    func_001E5D80();
    func_00182C70(arg0 + 0x190);
    M2C_FIELD(arg0, f32 *, 0x1C4) = 1.0f;
    M2C_FIELD(arg0, f32 *, 0x1C0) = 1000.0f;
}
