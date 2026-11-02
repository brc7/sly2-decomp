#include "m2c_prelude.h"
M2C_UNK InitSo();                            /* extern */

/** @sly2-annot addr=0x21BB88
 * Purpose: Init: InitSo then fields 0x170=1.0, 0x174/0x178=0, 0x184=100.0
 * Info: proposed name InitTrail (T1); class CID105 slot7; TU trail
 * Notes: agent qualifier: cid105 Init
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r8_21BB88
 * See: docs/engine/hud-ui.md#4-source-files, docs/engine/particles-effects.md#5-source-files,
 *   docs/reference/functions/particles-effects.md#trail-0x21bb88-0x21c820
 * @end-sly2-annot */
void func_0021BB88(void *arg0) {
    void *temp_v0;

    InitSo();
    temp_v0 = arg0 + 0x170;
    M2C_FIELD(arg0, f32 *, 0x170) = 1.0f;
    M2C_FIELD(temp_v0, s32 *, 4) = 0;
    M2C_FIELD(temp_v0, s32 *, 8) = 0;
    M2C_FIELD(arg0, f32 *, 0x184) = 100.0f;
}
