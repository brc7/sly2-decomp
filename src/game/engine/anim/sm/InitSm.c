#include "m2c_prelude.h"
extern M2C_UNK D_002C3738;
extern M2C_UNK D_002C3748;
extern M2C_UNK D_002C3968;
extern M2C_UNK D_002D9768;
extern f32 D_002DDED0;
extern M2C_UNK D_002DDFA0;
extern M2C_UNK D_002DE578;
extern M2C_UNK D_002DEB30;
extern s32 D_002E1E40;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003E126C;
extern M2C_UNK D_003E1278;

/** @sly2-annot addr=0x1EFA90
 * Purpose: InitLo (0x1A5270) then flags(+0x40)
 * Info: proposed name InitSm (T1); class SM slot7; TU sm
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/engine/animation.md#6-source-files, docs/engine/rendering.md#12-source-files,
 *   docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
s32 InitSm(s32 arg0) {
    s32 temp_v0;

    InitLo(arg0);
    temp_v0 = M2C_FIELD(arg0, s32 *, 0x40) | 0x40;
    M2C_FIELD(arg0, s32 *, 0x40) = temp_v0;
    return temp_v0;
}
