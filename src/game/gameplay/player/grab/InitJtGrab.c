#include "m2c_prelude.h"
extern M2C_UNK D_002A2D00;
extern s128 D_002A2DA0;
extern s128 D_002A2DC0;
extern s128 D_002A2DD0;
extern M2C_UNK D_002BBB60;
extern f32 D_002BF3A0;
extern M2C_UNK D_002BF3A8;
extern M2C_UNK D_002BF3B0;
extern f32 D_002BF3B8;
extern f32 D_002BF3BC;
extern M2C_UNK D_002BF3F8;
extern M2C_UNK D_002BF408;
extern f32 D_002BF414;
extern M2C_UNK D_002BF418;
extern f32 D_002BF4B8;
extern M2C_UNK D_002BF4C0;
extern M2C_UNK D_002BF4D0;
extern M2C_UNK D_002BF4E0;
extern M2C_UNK D_002BF4F0;
extern M2C_UNK D_002BF500;
extern f32 D_002BF510;
extern M2C_UNK D_002BF520;
extern M2C_UNK D_002BF530;
extern f32 D_002BF544;
extern M2C_UNK D_002BF548;
extern f32 D_002BF554;
extern f32 D_002BF558;
extern f32 D_002BF55C;
extern f32 D_002BF560;
extern f32 D_002BF564;
extern f32 D_002BF568;
extern f32 D_002BF56C;
extern M2C_UNK D_002BF570;
extern M2C_UNK D_002BF580;
extern f32 D_002BF58C;
extern f32 D_002BF590;
extern f32 D_002BF594;
extern M2C_UNK D_002BF598;
extern f32 D_002DDED0;
extern f32 D_002DDED4;
extern M2C_UNK D_002DE2E0;
extern M2C_UNK D_003DD490;
extern void *D_003E13B4;
extern s32 D_003E1BEC;

/** @sly2-annot addr=0x191090
 * Purpose: Init the STEPPLAYER+0x4C0 grab/action context: owner, nState=-1, nKind=-1, x4FC=-1, init two
 *   blend/actuator blocks (+0x180,+0x240, 0x20B840 id 0x6F) and the reach tracker at +0x440
 * Info: proposed name InitJtGrab (T1); TU grab
 * Notes: names_v2 B proposed GMeasureJumpHshape__FP3MJHf (0.978)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r4b_rest
 * See: docs/engine/player.md#4-stepplayer-tu-0x13ff70-0x144638-file-name-unknown-lilly-stepplayer_rawc,
 *   docs/engine/player.md#10-source-files, docs/reference/functions/player.md#grab-0x191090-0x195950
 * @end-sly2-annot */
void InitJtGrab(s32 arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 8) = -1;
    M2C_FIELD(arg0, s32 *, 0xC) = -1;
    M2C_FIELD(arg0, s32 *, 0x4FC) = -1;
    InitGait(arg0 + 0x180, arg1, 0x6F);
    InitGait(arg0 + 0x240, arg1, 0x6F);
    func_001CC158(arg0 + 0x440);
}
