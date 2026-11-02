#include "m2c_prelude.h"
/** @sly2-annot addr=0x1B2820
 * Purpose: Returns 0; result stored in CMpeg+0x104 per queued movie by ExecuteOids
 * Info: proposed name MpegStubReturn0 (T2); TU mpeg
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/cutscenes.md#4-source-files, docs/engine/cutscenes.md#open-questions,
 *   docs/engine/gameplay-objects.md#source-files
 * @end-sly2-annot */
s32 func_001B2820(void) {
    return 0;
}
