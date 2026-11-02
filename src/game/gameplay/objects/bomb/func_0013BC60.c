#include "m2c_prelude.h"
M2C_UNK RecalcSwOxfFilterForObject(M2C_UNK *, void *);           /* extern */
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x13BC60
 * Purpose: Set owner (+0x200) and RecalcSwOxfFilterForObject if changed
 * Info: proposed name SetBombOwner (T2); TU bomb
 * Notes: runtime5a #13: owner +0x200 = launcher CID112 for all darts | T2 forced: v2 tier kept (v2 held this
 *   name at T2)
 * Confidence: low (draft) [T2 0.60] - evidence: runtime (save-state RAM); sources: r2b_rest+runtime
 * See: docs/reference/functions/gameplay-objects.md#bomb-0x13a9e8-0x13bdf0
 * @end-sly2-annot */
void func_0013BC60(void *arg0, s32 arg1) {
    if (arg1 != M2C_FIELD(arg0, s32 *, 0x200)) {
        M2C_FIELD(arg0, s32 *, 0x200) = arg1;
        RecalcSwOxfFilterForObject(&D_003DD490, arg0);
    }
}
