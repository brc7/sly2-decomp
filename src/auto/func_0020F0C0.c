#include "m2c_prelude.h"
M2C_UNK AddGrfusr__Fi(M2C_UNK);                     /* extern */

/** @sly2-annot addr=0x20F0C0
 * Purpose: ++SW+0x3D7C no-pause counter (first -> AddGrfusr(4)); used by SetJsgHandsOff and SetJtModeState
 *   kind 1 (recon)
 * Info: proposed name IncrementSwNoPause (T1); TU sw
 * Notes: v3: superseded IncrementSwUsrLock | runtime6 N1: Lilly jtmode.c calls it IncrementSwNoPause; S57
 *   0x3D7C=1 in recon mode without hands-off; v2 IncrementSwUsrLock
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); Sly 1 / Lilly reference; sources:
 *   r7_1F8DD8+verdicts_B+runtime6
 * See: docs/engine/cutscenes.md#1-relay-and-jsg-the-sequence-engine-tu-0x1dfad8-0x1e5d80-file-name-unknown-lilly-jsgc,
 *   docs/engine/world-sw.md#5-hands-off-and-locks, docs/engine/world-sw.md#10-key-functions
 * @end-sly2-annot */
void func_0020F0C0(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x3D7C) + 1;
    M2C_FIELD(arg0, s32 *, 0x3D7C) = temp_v0;
    if (temp_v0 == 1) {
        AddGrfusr__Fi(4);
    }
}
