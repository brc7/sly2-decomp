#include "m2c_prelude.h"
M2C_UNK func_0011CCA8(M2C_UNK *, M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
extern s32 D_002DD2E0;
extern s32 D_002DD2E4;
extern s32 D_002DD2E8;
extern s32 D_002DD2EC;
extern s32 D_002DD2F0;
extern s32 D_002DD2F4;
extern M2C_UNK D_004520A8;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_004520F8;
extern M2C_UNK D_00452120;
extern M2C_UNK D_00452148;
extern M2C_UNK D_00452170;

/** @sly2-annot addr=0x220BE8
 * Purpose: compiler-generated __static_initialization_and_destruction_0 of TU throw
 * Info: proposed name StaticInit_throw (T1); TU throw
 * Notes: real local symbol name is __static_initialization_and_destruction_0 (not unique; renamed per TU)
 * Confidence: high [T1 0.95] - evidence: static analysis; sources: tu_map(structural)+r8_21BB88
 * @end-sly2-annot */
void StaticInit_throw(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;

    if ((arg1 == 0xFFFF) && (arg0 != 0)) {
        temp_v0 = D_002DD2E0 + 1;
        D_002DD2E0 = temp_v0;
        if (temp_v0 == 1) {
            func_0011CCA8(&D_004520A8, 0x14, 0x2000, 0x10);
        }
        temp_v0_2 = D_002DD2E4 + 1;
        D_002DD2E4 = temp_v0_2;
        if (temp_v0_2 == 1) {
            func_0011CCA8(&D_004520D0, 0x18, 0x800, 0x10);
        }
        temp_v0_3 = D_002DD2E8 + 1;
        D_002DD2E8 = temp_v0_3;
        if (temp_v0_3 == 1) {
            func_0011CCA8(&D_004520F8, 0x28, 0x200, 0x10);
        }
        temp_v0_4 = D_002DD2EC + 1;
        D_002DD2EC = temp_v0_4;
        if (temp_v0_4 == 1) {
            func_0011CCA8(&D_00452120, 0x1C, 0x800, 0x10);
        }
        temp_v0_5 = D_002DD2F0 + 1;
        D_002DD2F0 = temp_v0_5;
        if (temp_v0_5 == 1) {
            func_0011CCA8(&D_00452148, 0xC, 0xC8, 0x10);
        }
        temp_v0_6 = D_002DD2F4 + 1;
        D_002DD2F4 = temp_v0_6;
        if (temp_v0_6 == 1) {
            func_0011CCA8(&D_00452170, 0x14, 0x100, 0x10);
        }
    }
}
