#include "m2c_prelude.h"
extern s128 D_002A2DD0;
extern M2C_UNK D_002BBB60;
extern M2C_UNK D_002D9BE0;
extern M2C_UNK D_002D9C70;
extern M2C_UNK D_002D9D00;
extern s32 D_002DD5BC;
extern M2C_UNK D_002DDFA0;
extern s32 D_002E1E40;
extern s32 D_002F7900;
extern M2C_UNK D_003DD040;
extern M2C_UNK D_003DD08C;
extern M2C_UNK D_003DD490;

/** @sly2-annot addr=0x1FB6A8
 * Purpose: If not in SNDMGR pending DL, append and Incref
 * Info: proposed name AddSndToDirtyList (T1); TU sound
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void AddSndToDirtyList(s32 arg0) {
    void *temp_v0;

    if (FFindDlEntry__FP2DLPv((s32) &D_003DD08C, arg0) == 0) {
        temp_v0 = M2C_FIELD(arg0, void **, 0xC);
        M2C_FIELD(temp_v0, void (**)(s32), 0xC)(arg0 + M2C_FIELD(temp_v0, s16 *, 8));
        AppendDlEntry__FP2DLPv((s32) &D_003DD08C, arg0);
    }
}
