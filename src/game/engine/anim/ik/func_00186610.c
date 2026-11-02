#include "m2c_prelude.h"
M2C_UNK CopyAb__FPvT0Ui(s32, M2C_UNK *, M2C_UNK);   /* extern */
M2C_UNK func_001177A8();                            /* extern */
M2C_UNK func_00117EB0(s32, s32);                    /* extern */
s32 func_00242810();                                /* extern */
s32 func_002432D0(s32, s32, M2C_UNK *);             /* extern */
s32 func_00243388(s32, s32);                        /* extern */
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

/** @sly2-annot addr=0x186610
 * Purpose: ACT subclass override: ACT::Clone-slot base then save owner xA8 into x1C
 * Info: proposed name ApplyActik (T2); class ACTIK; TU ik
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/animation.md#6-source-files, docs/engine/player.md#10-source-files,
 *   docs/reference/functions/animation.md#ik-0x186610-0x187030
 * @end-sly2-annot */
s32 func_00186610(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *temp_v0;

    CloneAct(arg0, arg1, arg2, arg3);
    temp_v0 = M2C_FIELD(arg0, void **, 8);
    M2C_FIELD(arg0, s32 *, 0x1C) = (s32) M2C_FIELD(temp_v0, s32 *, 0xA8);
    return (s32) temp_v0;
}
