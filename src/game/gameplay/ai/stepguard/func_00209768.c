#include "m2c_prelude.h"
/** @sly2-annot addr=0x209768
 * Purpose: mover(+0xFD0)->owner(+0xC) = guard
 * Info: proposed name InitStepguardMover (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00209768(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, s32 *, 0xC) = arg1;
}
