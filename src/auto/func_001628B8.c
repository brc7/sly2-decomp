#include "m2c_prelude.h"
M2C_UNK func_00162908(s32);                         /* extern */
M2C_UNK func_001629A0(s32);                         /* extern */
s32 func_001633E8();                                /* extern */
extern s32 D_003E0F9C;

/** @sly2-annot addr=0x1628B8
 * Purpose: If p is not the current job: end the current one (EndCurrentJob) and begin p (BeginJob)
 * Info: proposed name SetCurrentJob (T1); TU game
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_001628B8(s32 arg0) {
    if (func_001633E8() == 0) {
        if (D_003E0F9C != 0) {
            func_00162908(D_003E0F9C);
        }
        func_001629A0(arg0);
    }
}
