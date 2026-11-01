#include "m2c_prelude.h"
M2C_UNK func_001EEFC0(void *);                      /* extern */
M2C_UNK func_001EF038();                            /* extern */
extern f32 D_002DDED0;

/** @sly2-annot addr=0x146650
 * Purpose: state setter
 * Info: proposed name SetBhv196State (T2); TU tu_144B28
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_00146650(void *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x30);
    if (arg1 != temp_v0) {
        if (temp_v0 == 2) {
            func_001EF038();
        }
        M2C_FIELD(arg0, s32 *, 0x30) = arg1;
        M2C_FIELD(arg0, f32 *, 0x34) = (f32) D_002DDED0;
        if (arg1 == 2) {
            func_001EEFC0(arg0);
        }
    }
}
