#include "m2c_prelude.h"
/** @sly2-annot addr=0x12E740
 * Purpose: slot 44: 16-byte accessor
 * Info: proposed name CID44_Slot44 (T2); class CID44 slot44; TU tu_12E600
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48
 * @end-sly2-annot */
void func_0012E740(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x12C) = (s32) (M2C_FIELD(arg0, s32 *, 0x12C) + 1);
}
