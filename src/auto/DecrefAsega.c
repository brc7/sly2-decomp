#include "m2c_prelude.h"
s32 *func_001AE318(s32 *, M2C_UNK);                 /* extern */
extern s128 D_002A2DA0;
extern s128 D_002A2DD0;
extern M2C_UNK D_002CAF08;
extern M2C_UNK D_002CAF48;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern f32 D_002DDF10;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DE578;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E1218;
extern M2C_UNK D_003E1224;

/** @sly2-annot addr=0x1308B8
 * Purpose: --cref; at 0 destroy and return to the ASEGA pool (FreeSlotheapPv)
 * Info: proposed name DecrefAsega (T1); class ASEGA slot2; TU asega
 * Notes: names_v2 v1 proposed SetAsegaHandsOff__FP5ASEGAi (0.000)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
void DecrefAsega(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x10) - 1;
    M2C_FIELD(arg0, s32 *, 0x10) = temp_v0;
    if (temp_v0 <= 0) {
        func_00256EE0(arg0, 0);
        func_002580A8(arg0 + 4, 0);
        UnsubscribeLoAll(arg0);
        func_0011CD98((s32) &D_003E1224, arg0);
    }
}
