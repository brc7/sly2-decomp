#include "m2c_prelude.h"
M2C_UNK func_0012CFE0(s32);                         /* extern */
M2C_UNK func_0012D020(s32);                         /* extern */

/** @sly2-annot addr=0x18C120
 * Purpose: (pjt,f): enable/disable 5 actuators x19E4..x19F4 (0x12CFE0/0x12D020)
 * Info: proposed name EnableJtLeanActs (T2); TU jt
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018C120(void *arg0, s32 arg1) {
    s32 temp_a0;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0x19E4);
    if (temp_a0 != 0) {
        if (arg1 & 1) {
            func_0012CFE0(temp_a0);
        } else {
            func_0012D020(temp_a0);
        }
        if (arg1 & 2) {
            func_0012CFE0(M2C_FIELD(arg0, s32 *, 0x19E8));
        } else {
            func_0012D020(M2C_FIELD(arg0, s32 *, 0x19E8));
        }
        if (arg1 & 4) {
            func_0012CFE0(M2C_FIELD(arg0, s32 *, 0x19EC));
        } else {
            func_0012D020(M2C_FIELD(arg0, s32 *, 0x19EC));
        }
        if (arg1 & 8) {
            func_0012CFE0(M2C_FIELD(arg0, s32 *, 0x19F0));
        } else {
            func_0012D020(M2C_FIELD(arg0, s32 *, 0x19F0));
        }
        if (arg1 & 0x10) {
            func_0012CFE0(M2C_FIELD(arg0, s32 *, 0x19F4));
        } else {
            func_0012D020(M2C_FIELD(arg0, s32 *, 0x19F4));
        }
        if (arg1 & 0x20) {
            func_0012CFE0(M2C_FIELD(arg0, s32 *, 0x19FC));
        } else {
            func_0012D020(M2C_FIELD(arg0, s32 *, 0x19FC));
        }
        if (arg1 & 0x40) {
            func_0012CFE0(M2C_FIELD(arg0, s32 *, 0x19F8));
            return;
        }
        func_0012D020(M2C_FIELD(arg0, s32 *, 0x19F8));
    }
}
