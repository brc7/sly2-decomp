#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s128 **, s32, s32);    /* extern */
M2C_UNK func_00117EB0(s32, void *);                 /* extern */
M2C_UNK func_00117F70(s32, s32);                    /* extern */
s128 **func_001AE318(s32 *, M2C_UNK);               /* extern */
extern f32 D_002A76DC;
extern s128 *D_002A76E0;
extern f32 D_002A76E4;
extern f32 D_002A76E8;
extern f32 D_002C0C00;
extern f32 D_002C0C04;
extern M2C_UNK D_002C0C10;
extern M2C_UNK D_002C0C90;
extern M2C_UNK D_002C0CB0;
extern M2C_UNK D_002C0CE0;
extern f32 D_002C0CE8;
extern f32 D_002C0CEC;
extern M2C_UNK D_002C0CF0;
extern M2C_UNK D_002C0CF8;
extern M2C_UNK D_002DEA50;
extern M2C_UNK D_002F7980;
extern M2C_UNK D_00303980;
extern M2C_UNK D_003CF980;
extern s32 D_003D3980;
extern M2C_UNK D_003DD020;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1C0378
 * Purpose: CID 17 vt7: InitXfm, ids -1, init DL
 * Info: proposed name InitPathzone (T1); class PATHZONE slot7; TU path
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void func_001C0378(s32 arg0) {
    func_00231100(arg0);
    M2C_FIELD(arg0, s32 *, 0x110) = -1;
    func_0011A1F8(arg0 + 0x118, 0x60);
    M2C_FIELD(arg0, s32 *, 0x124) = -1;
}
