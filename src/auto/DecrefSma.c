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

/** @sly2-annot addr=0x1F0538
 * Purpose: --cref; at 0: dtor, clear, unlink, free to pool D_003E1278
 * Info: proposed name DecrefSma (T1); class SMA slot2; TU sm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
void DecrefSma(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x10) - 1;
    M2C_FIELD(arg0, s32 *, 0x10) = temp_v0;
    if (temp_v0 <= 0) {
        func_00256EE0(arg0, 0);
        func_002580A8(arg0 + 4, 0);
        UnsubscribeLoAll(arg0);
        func_0011CD98((s32) &D_003E1278, arg0);
    }
}
