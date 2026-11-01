#include "m2c_prelude.h"
s32 func_001C4A48(M2C_UNK *);                       /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1C40F8
 * Purpose: True if PO == effective current PO (override +0x3F00 else +0x3EFC)
 * Info: proposed name FIsPoActive (T1); TU po
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
s32 func_001C40F8(s32 arg0) {
    return arg0 == func_001C4A48(&D_003DD490);
}
