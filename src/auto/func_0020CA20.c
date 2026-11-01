#include "m2c_prelude.h"
M2C_UNK func_0011CD98(M2C_UNK *, s32);              /* extern */
extern M2C_UNK D_003E1AF0;

/** @sly2-annot addr=0x20CA20
 * Purpose: free to slotheap D_003E1AF0
 * Info: proposed name FreeDirAnim (T2); TU tu_20B840
 * Notes: T2 forced: verifier: coined class word without descriptor
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020CA20(s32 arg0) {
    func_0011CD98(&D_003E1AF0, arg0);
}
