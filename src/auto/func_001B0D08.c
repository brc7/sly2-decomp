#include "m2c_prelude.h"
M2C_UNK func_001B0D90(void *);                      /* extern */
M2C_UNK func_001B1A38(void *, s32, void *);         /* extern */
M2C_UNK func_001B1BF0(void *);                      /* extern */
M2C_UNK PostSoLoad();                            /* extern */

/** @sly2-annot addr=0x1B0D08
 * Purpose: CID 96 vt15: PostSoLoad, resolve links, compute walk speeds from two ASEG clips (0x1B1A38), reset
 * Info: proposed name PostCid96Load (T2); class CID96 slot15; TU tu_1B0B78
 * Notes: names_v2 v1 proposed AdjustStepNewXp__FP4STEPP2XPi (0.438)
 * Confidence: low (draft) [T2 0.65] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
void func_001B0D08(void *arg0) {
    PostSoLoad();
    func_001B1BF0(arg0);
    func_001B1A38(arg0, M2C_FIELD(arg0, s32 *, 0x18C), arg0 + 0x1CC);
    func_001B1A38(arg0, M2C_FIELD(arg0, s32 *, 0x1B0), arg0 + 0x1D0);
    func_001B0D90(arg0);
}
