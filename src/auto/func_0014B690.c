#include "m2c_prelude.h"
extern M2C_UNK D_002A4EF0;

/** @sly2-annot addr=0x14B690
 * Purpose: returns the constant string 'fid:#unknown' (retail stub of a file-id name lookup)
 * Info: proposed name PchzFromFid (T2); TU tu_14B660
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/core-library.md#4-binary-input-streams-and-cd,
 *   docs/formats/disc-records.md#2-the-read-path, docs/formats/world-file.md#31-world-entry-table
 * @end-sly2-annot */
M2C_UNK *func_0014B690(void) {
    return &D_002A4EF0;
}
