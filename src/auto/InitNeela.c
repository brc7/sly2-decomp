#include "m2c_prelude.h"
M2C_UNK InitStepguard();                            /* extern */

/** @sly2-annot addr=0x1B9F80
 * Purpose: CID 60 vt7: InitStepguard + tuning (1.35, -0.0003, 0.9, 1.2, 7.0), flags
 * Info: proposed name InitNeela (T1); class NEELA slot7; TU tu_1B9F80
 * Notes: class identity from whole_game WE names/object strings: InitCid60Guard -> InitNeela
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r5b_rest+whole_game(class identity)
 * See: docs/engine/guards-ai.md#8-source-files, docs/engine/player.md#10-source-files,
 *   docs/reference/functions/guards-ai.md#tu_1b9f80-0x1b9f80-0x1bab50
 * @end-sly2-annot */
void InitNeela(void *arg0) {
    void *temp_v0;

    InitStepguard();
    temp_v0 = arg0 + 0x1070;
    M2C_FIELD(arg0, f32 *, 0x1070) = 1.35f;
    M2C_FIELD(temp_v0, f32 *, 4) = -0.0003f;
    M2C_FIELD(temp_v0, s32 *, 8) = 0;
    M2C_FIELD(arg0, f32 *, 0x1080) = 0.9f;
    M2C_FIELD(arg0, f32 *, 0x1084) = 1.2f;
    M2C_FIELD(arg0, f32 *, 0x1088) = 7.0f;
    M2C_FIELD(arg0, s32 *, 0x364) = 0;
    M2C_FIELD(arg0, s32 *, 0xE64) = 0;
    M2C_FIELD(M2C_FIELD(arg0, void **, 0xF64), s32 *, 0xF8) = 3;
    M2C_FIELD(arg0, s32 *, 0x4A4) = 1;
}
