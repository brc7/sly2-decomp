#include "m2c_prelude.h"
/* Warning: missing "jr $ra" in last block of func_001A50E8 (initial). */

M2C_UNK CopyAb__FPvT0Ui(s32, s32, s32);             /* extern */
M2C_UNK Decref__4CRef(void *);                      /* extern */
extern f32 D_002A6A40;
extern M2C_UNK D_002C44F0;
extern f32 D_002DDED0;
extern M2C_UNK D_002DEB30;
extern M2C_UNK D_003DD490;
extern M2C_UNK D_003DD4A0;
extern M2C_UNK D_003DD4BC;
extern s32 D_003E139C;
extern M2C_UNK D_004520D0;
extern M2C_UNK D_00452148;

/** @sly2-annot addr=0x1A5D58
 * Purpose: Alloc a 12-byte {pfn,arg,next} node from g_sw array (+0x39D0/+0x39D4 counter) and push on plo->x34
 *   list
 * Info: proposed name PdcbAddLo (T1); TU lo
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r4_17F090+verdicts_tu_r4r6
 * See: docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
s32 PdcbAddLo(s32 arg0) {
    void *temp_v0;

    temp_v0 = func_00210100((s32) &D_003DD490);
    M2C_FIELD(temp_v0, void **, 8) = (void *) M2C_FIELD(arg0, void **, 0x34);
    M2C_FIELD(arg0, void **, 0x34) = temp_v0;
    return (s32) temp_v0;
}
