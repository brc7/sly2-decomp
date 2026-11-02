#include "m2c_prelude.h"
s32 FMemcardThreadValid__FP7MEMCARD();              /* extern */
M2C_UNK SignalSema(s32);                            /* extern */
M2C_UNK WaitSema(s32);                              /* extern */
M2C_UNK func_002348B0(s32, M2C_UNK);                /* extern */

/** @sly2-annot addr=0x1AB040
 * Purpose: If thread valid iReleaseAlarm(id,1); then wait/signal sema (block until job finishes)
 * Info: proposed name SetMemcardThreadHighPrio (T1); TU memcard
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources:
 *   r4_17F090+verdicts_A2+names_v2-signature(verified)
 * See: docs/engine/save-memcard.md#5-key-functions,
 *   docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
void SetMemcardThreadHighPrio__FP7MEMCARD(void *arg0) {
    if (FMemcardThreadValid__FP7MEMCARD() != 0) {
        func_002348B0(M2C_FIELD(arg0, s32 *, 4), 1);
    }
    WaitSema(M2C_FIELD(arg0, s32 *, 0));
    SignalSema(M2C_FIELD(arg0, s32 *, 0));
}
