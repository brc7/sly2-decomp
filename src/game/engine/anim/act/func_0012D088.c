#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK func_00117EB0(s32, void *);                 /* extern */
M2C_UNK func_001291A8(s32, void *);                 /* extern */
M2C_UNK func_00129EC8(s32, s32, s32);               /* static */
M2C_UNK SetTag__4CRef4TAGK(s32, M2C_UNK);                /* static */
extern s128 D_002A2DD0;
extern f32 D_002A45A8;
extern M2C_UNK D_002CA8A0;
extern M2C_UNK D_002CA8C0;
extern M2C_UNK D_002CB588;
extern M2C_UNK D_003DD490;
extern s32 D_003E123C;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_00452148;
extern M2C_UNK D_004521E8;

/** @sly2-annot addr=0x12D088
 * Purpose: vtable 0x2CA830 slot 4
 * Info: proposed name ActUnk_2CA830_4 (T2); TU act
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012D088(s32 arg0) {
    M2C_FIELD(M2C_FIELD(arg0, void **, 8), s128 *, 0xC0) = (s128) M2C_FIELD(arg0, s128 *, 0x20);
    M2C_FIELD(M2C_FIELD(arg0, void **, 8), s128 *, 0xD0) = (s128) D_002A2DD0;
    M2C_FIELD(M2C_FIELD(arg0, void **, 8), u16 *, 0x9A) = (u16) M2C_FIELD(arg0, u16 *, 0x1C);
    func_0022C818(M2C_FIELD(arg0, s32 *, 4), (s32) M2C_FIELD(arg0, void **, 8));
}
