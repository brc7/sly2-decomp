#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, M2C_UNK *, M2C_UNK);   /* extern */
M2C_UNK __18CBinaryInputStream();                            /* extern */
M2C_UNK ReadVector__18CBinaryInputStreamP6VECTOR(s32, s32);                    /* extern */
s32 sceCdGetReadPos();                                /* extern */
s32 scePadRead(s32, s32, M2C_UNK *);             /* extern */
s32 scePadGetState_243388(s32, s32);                        /* extern */
s32 scePadGetState(s32, s32);                       /* extern */
extern s128 D_002A2DC0;
extern f32 D_002A5684;
extern f32 D_002A5688;
extern f32 D_002A568C;
extern f32 D_002A5690;
extern M2C_UNK D_002BBB60;
extern f32 D_002BE908;
extern f32 D_002BE90C;
extern f32 D_002BE910;
extern M2C_UNK D_002BE920;
extern M2C_UNK D_002BE930;
extern M2C_UNK D_002BE950;
extern M2C_UNK D_002BE990;
extern M2C_UNK D_002BF1B0;
extern f32 D_002BF2D0;
extern f32 D_002BF2D4;
extern f32 D_002BF2D8;
extern f32 D_002BF2DC;
extern M2C_UNK D_002BF310;
extern M2C_UNK D_002BF320;
extern M2C_UNK D_002BF330;
extern M2C_UNK D_002D2918;
extern M2C_UNK D_002DDEC0;
extern M2C_UNK D_002DDED0;
extern f32 D_002DDED4;
extern f32 D_002DDF0C;
extern M2C_UNK D_002DDF40;
extern f32 D_002DEB30;
extern s32 D_002DFB84;
extern s32 D_002DFBC0;
extern s32 D_002DFBC4;
extern s32 D_002E0CBC;
extern s32 D_002E1E40;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x189458
 * Purpose: JT vt92: if x18C0 (object he is attached to) call 0x2046F0(pjt,obj) and return 1
 * Info: proposed name JT_FFaceTarget (T1); class JT slot92; TU jt
 * Notes: verifier PLAUSIBLE: JT::FMatchAttached -> JT::FFaceTarget (vt92) | verifier: purpose partly wrong:
 *   JT slot 92 (MURRAY returns 0, BENTLEY no-op). If ploUnk18c0: 0x2046F0 = atan2 of (target pos - own pos)
 *   -> 0x204660 sets heading angle + Z rotation, i.e. turn t
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
s32 JT_FFaceTarget(s32 arg0) {
    s32 temp_a1;
    s32 var_v0;

    temp_a1 = M2C_FIELD(arg0, s32 *, 0x18C0);
    var_v0 = 0;
    if (temp_a1 != 0) {
        FaceStepToward(arg0, temp_a1);
        var_v0 = 1;
    }
    return var_v0;
}
