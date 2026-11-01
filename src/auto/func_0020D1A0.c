#include "m2c_prelude.h"
/** @sly2-annot addr=0x20D1A0
 * Purpose: enabled = 1
 * Info: proposed name EnableStuckWatch (T1); TU tu_20CE28
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#tu_20ce28-0x20ce28-0x20d5d0
 * @end-sly2-annot */
void func_0020D1A0(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0xC) = 1;
}
