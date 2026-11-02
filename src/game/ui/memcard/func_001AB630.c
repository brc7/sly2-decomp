#include "m2c_prelude.h"
M2C_UNK SetMemcardThreadHighPrio__FP7MEMCARD(void *);                      /* extern */
M2C_UNK SetSaveState(void *, M2C_UNK);             /* extern */
M2C_UNK func_001AC570(void *, void *);              /* extern */
extern s32 D_003E0EA8;

/** @sly2-annot addr=0x1AB630
 * Purpose: If state 5 and D_003E0EA8: wait for thread, re-init all 3 slots (stride 0x5464, 0x1AC570), go to
 *   state 6
 * Info: proposed name ReloadSaveSlots (T2); TU memcard
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void func_001AB630(void *arg0) {
    s32 var_s2;
    void *var_s0;

    if ((M2C_FIELD(arg0, s32 *, 8) == 5) && (D_003E0EA8 != 0)) {
        var_s0 = arg0 + 0x24;
        var_s2 = 2;
        SetMemcardThreadHighPrio__FP7MEMCARD(arg0 + 0xFD5C);
        do {
            func_001AC570(arg0, var_s0);
            var_s2 -= 1;
            var_s0 += 0x5464;
        } while (var_s2 >= 0);
        M2C_FIELD(arg0, s32 *, 0xC) = 6;
        SetSaveState(arg0, 6);
    }
}
