#include "m2c_prelude.h"
/** @sly2-annot addr=0x210268
 * Purpose: Clear LO entry in update array, restore LO+0x40 flags, ++dead count (+0x3F18)
 * Info: proposed name RemoveSwUpdateLo (T2); TU sw
 * Notes: names_v2 v1 proposed LoadButtonFromBrx__FP6BUTTONP18CBinaryInputStream (0.001)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_00210268(void *arg0, void *arg1) {
    s32 temp_v1;

    temp_v1 = M2C_FIELD(arg1, s32 *, 0x40);
    if ((temp_v1 ^ 1) & 1) {
        M2C_FIELD(arg1, s32 *, 0x40) = (s32) M2C_FIELD(temp_v1, s32 *, 0);
        M2C_FIELD(temp_v1, s32 *, 0) = 0;
        M2C_FIELD(temp_v1, s32 *, 4) = 0;
        M2C_FIELD(arg0, s32 *, 0x3F18) = (s32) (M2C_FIELD(arg0, s32 *, 0x3F18) + 1);
    }
}
