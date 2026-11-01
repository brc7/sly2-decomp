#include "m2c_prelude.h"
s32 func_00134310(s32, M2C_UNK);                    /* extern */
extern s32 D_002DD5BC;

/** @sly2-annot addr=0x220498
 * Purpose: if q is the carried object and state>=3 (or player in jts 0x12) return 0x134310(flags,3) else
 *   flags
 * Info: proposed name AdjustJtthrowGrfsp (T2); TU throw
 * Notes: agent qualifier: filter flags for held object
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#throw-0x21f2e0-0x220d40
 * @end-sly2-annot */
s32 func_00220498(void *arg0, void *arg1, s32 arg2) {
    s32 temp_a0;

    if (arg1 == M2C_FIELD(arg0, s32 *, 0x10)) {
        temp_a0 = M2C_FIELD(arg0, s32 *, 4);
        if (temp_a0 != -1) {
            if ((temp_a0 >= 3) || ((arg1 == D_002DD5BC) && (M2C_FIELD(arg1, s32 *, 0x2DC) == 0x12))) {
                return func_00134310(arg2, 3);
            }
            /* Duplicate return node #7. Try simplifying control flow for better match */
            return arg2;
        }
        /* Duplicate return node #7. Try simplifying control flow for better match */
        return arg2;
    }
    return arg2;
}
