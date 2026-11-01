#include "m2c_prelude.h"
s32 func_001C4840(M2C_UNK *);                       /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1C40C0
 * Purpose: True if PO == g_sw current PO (+0x3EFC)
 * Info: proposed name FIsPoCur (T1); TU po
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
s32 func_001C40C0(s32 arg0) {
    return arg0 == func_001C4840(&D_003DD490);
}
