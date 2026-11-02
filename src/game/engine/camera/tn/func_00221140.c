#include "m2c_prelude.h"
s32 PloCloneLo__FP2LOP2SO(s32, s32);                        /* extern */
M2C_UNK CloneXfm();                            /* extern */

/** @sly2-annot addr=0x221140
 * Purpose: CloneXfm then re-resolves ref field 0x10C via 0x13E510
 * Info: proposed name CloneTn (T1); class TN slot41; TU tn
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void func_00221140(void *arg0) {
    CloneXfm();
    M2C_FIELD(arg0, s32 *, 0x10C) = PloCloneLo__FP2LOP2SO(M2C_FIELD(arg0, s32 *, 0x10C), M2C_FIELD(arg0, s32 *, 0x10));
}
