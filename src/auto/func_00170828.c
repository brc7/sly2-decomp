#include "m2c_prelude.h"
M2C_UNK func_001ECC28(M2C_UNK, s32, M2C_UNK);       /* extern */

/** @sly2-annot addr=0x170828
 * Purpose: Assigns GS addresses to the class-1 bitmaps/CLUTs without uploading (0x1ECC28 with no GIFS)
 * Info: proposed name AllocBitmapsGsb (T2); TU gs
 * Notes: mangled withheld (not T1): AllocBitmapsGsb__FP3GSB
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_00170828(s32 arg0) {
    func_001ECC28(1, arg0, 0);
}
