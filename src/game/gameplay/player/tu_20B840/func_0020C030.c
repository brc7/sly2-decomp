#include "m2c_prelude.h"
M2C_UNK func_0011CD98(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_003E1AD8;

/** @sly2-annot addr=0x20C030
 * Purpose: free to slotheap D_003E1AD8
 * Info: proposed name FreeLeap (T2); TU tu_20B840
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020C030(s32 arg0) {
    func_0011CD98(&D_003E1AD8, arg0);
}
