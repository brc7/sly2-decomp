#include "m2c_prelude.h"
/** @sly2-annot addr=0x19DE10
 * Purpose: Init the STEPPLAYER+0xF50 pipe context (owner, state -1, tuning 45/40 ...)
 * Info: proposed name InitJtPipe (T1); TU pipe_jt
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/player.md#4-stepplayer-tu-0x13ff70-0x144638-file-name-unknown-lilly-stepplayer_rawc,
 *   docs/engine/player.md#10-source-files, docs/reference/functions/player.md#pipe_jt-0x19de10-0x19f0a8
 * @end-sly2-annot */
void InitJtPipe(void *arg0, s32 arg1) {
    void *temp_a1;
    void *temp_v0;

    temp_v0 = arg0 + 0x30;
    M2C_FIELD(arg0, s32 *, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 4) = -1;
    temp_a1 = arg0 + 0x40;
    M2C_FIELD(arg0, f32 *, 0x30) = 45.0f;
    M2C_FIELD(temp_v0, f32 *, 8) = 40.0f;
    M2C_FIELD(temp_v0, s32 *, 4) = 0;
    M2C_FIELD(arg0, f32 *, 0x40) = 45.0f;
    M2C_FIELD(temp_a1, f32 *, 8) = -20.0f;
    M2C_FIELD(temp_a1, s32 *, 4) = 0;
}
