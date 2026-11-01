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

/** @sly2-annot addr=0x12E010
 * Purpose: vtable 0x2CA6F0 slot 4
 * Info: proposed name ActUnk_2CA6F0_4 (T2); TU act
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012E010(s32 arg0) {
    M2C_FIELD(M2C_FIELD(arg0, void **, 8), f32 *, 0xA8) = (f32) M2C_FIELD(arg0, f32 *, 0x1C);
    M2C_FIELD(M2C_FIELD(arg0, void **, 8), s128 *, 0xC0) = (s128) M2C_FIELD(arg0, s128 *, 0x20);
    M2C_FIELD(M2C_FIELD(arg0, void **, 8), s128 *, 0xD0) = (s128) M2C_FIELD(arg0, s128 *, 0x30);
    M2C_FIELD(M2C_FIELD(arg0, void **, 8), s16 *, 0x9A) = 2;
    func_0022C818(M2C_FIELD(arg0, s32 *, 4), (s32) M2C_FIELD(arg0, void **, 8));
}
