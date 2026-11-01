#include "m2c_prelude.h"
M2C_UNK func_001F08D0();                            /* extern */
M2C_UNK func_001F0D88(void *, M2C_UNK);             /* extern */

/** @sly2-annot addr=0x1F1350
 * Purpose: Stores the transition mask (+0x48); if the running transition is now disallowed, abort and
 *   re-choose
 * Info: proposed name SetSmaMask (T1); TU sm
 * Confidence: mid (draft) [T1 0.65] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
void func_001F1350(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0x48) = arg1;
    if ((M2C_FIELD(arg0, s32 *, 0x30) != 0) && (M2C_FIELD(M2C_FIELD(arg0, void **, 0x40), s32 *, 0x10) & arg1)) {
        func_001F08D0();
        func_001F0D88(arg0, 0);
    }
}
