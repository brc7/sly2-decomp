#include "m2c_prelude.h"
/** @sly2-annot addr=0x13D8B8
 * Purpose: Stub returning 0 (file input unused in retail)
 * Info: proposed name CQueueInputFile_CbRead (T1); class CQueueInputFile; TU bq
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest+verdicts_A1
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#bq-0x13d558-0x13da18
 * @end-sly2-annot */
s32 CbRead__15CQueueInputFileiPv(void) {
    return 0;
}
