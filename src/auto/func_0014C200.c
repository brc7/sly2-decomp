#include "m2c_prelude.h"
s32 func_001D82B8(M2C_UNK *, M2C_UNK *);            /* extern */
M2C_UNK func_001DAF38(M2C_UNK, M2C_UNK);            /* extern */
M2C_UNK func_001FB2F8(s32, M2C_UNK);                /* extern */
s32 func_001FBB90(M2C_UNK);                         /* extern */
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003E0EA8;

/** @sly2-annot addr=0x14C200
 * Purpose: If 0x1D82B8(&D_003E0EA8, ...) ? 100, 0x1DAF38(0x1F, 1); else play error sound 0x3E
 * Info: proposed name CheatBuyItem1F (T2); TU cheat
 * Confidence: low (draft) [T2 0.65] - evidence: semantic review; sources: semantic_tu_docs/cheat
 * See: docs/reference/functions/main-loop-startup.md#cheat-0x14bea8-0x14c548
 * @end-sly2-annot */
void func_0014C200(void) {
    if (func_001D82B8(&D_003E0EA8, &D_003D4A60) >= 0x64) {
        func_001DAF38(0x1F, 1);
        return;
    }
    func_001FB2F8(func_001FBB90(0x3E), 0);
}
