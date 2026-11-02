#include "m2c_prelude.h"
extern s32 D_002F6384;
extern s32 D_002F6388;

/** @sly2-annot addr=0x1AE150
 * Purpose: Bytes free between world heap top (D_002F6384) and stack heap bottom (D_002F6388)
 * Info: proposed name CbFreeMem (T1); TU memory
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/core-library.md#1-memory, docs/engine/core-library.md#6-source-files,
 *   docs/engine/save-memcard.md#4-source-files
 * @end-sly2-annot */
s32 func_001AE150(void) {
    return D_002F6388 - D_002F6384;
}
