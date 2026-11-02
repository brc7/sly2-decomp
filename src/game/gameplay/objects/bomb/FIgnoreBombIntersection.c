#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK Read__18CBinaryInputStreamiPv(s32, s32, M2C_UNK *);         /* extern */
M2C_UNK Skip__18CBinaryInputStreami(s32, s32);                    /* extern */
M2C_UNK ReadVector4__18CBinaryInputStreamP7VECTOR4(s32, void *);                 /* extern */
M2C_UNK func_001291A8(s32 *, s32 *);                /* extern */
M2C_UNK Decref__4CRef(s32 *);                       /* extern */
s32 func_001AE318(void *, s32, s32, M2C_UNK, s32, s32); /* extern */
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern s128 D_002A2DE0;
extern f32 D_002A4B18;
extern f32 D_002A4B1C;
extern f32 D_002A4B20;
extern M2C_UNK D_002A4BB8;
extern M2C_UNK D_002A4BC0;
extern M2C_UNK D_002A4BE0;
extern M2C_UNK D_002AC880;
extern M2C_UNK D_002BAAE8;
extern s32 D_002BC9D0;
extern M2C_UNK D_002BC9F0;
extern M2C_UNK D_002CBE20;
extern M2C_UNK D_002CBE38;
extern M2C_UNK D_002DD590;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDF40;
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_002DEDC0;
extern s32 D_002E1E54;
extern M2C_UNK D_003D4A60;
extern M2C_UNK D_003DA0C0;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E0EA8;
extern s32 D_003E0F9C;
extern M2C_UNK D_003E1C20;
extern s32 D_003E1C34;

/** @sly2-annot addr=0x13BBF8
 * Purpose: BOMB vt61: SO base; ignore (max with 1) objects sharing the owner's root
 * Info: proposed name FIgnoreBombIntersection (T1); class BOMB slot61; TU bomb
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#bomb-0x13a9e8-0x13bdf0
 * @end-sly2-annot */
s32 FIgnoreBombIntersection(s32 arg0, s32 arg1) {
    s32 var_a3;

    var_a3 = FIgnoreSoIntersection(arg0, arg1);
    if (*M2C_FIELD(arg1, s32 **, 0x14) == *M2C_FIELD(M2C_FIELD(arg0, void **, 0x200), s32 **, 0x14)) {
        var_a3 = func_00134310(var_a3, 1);
    }
    return var_a3;
}
