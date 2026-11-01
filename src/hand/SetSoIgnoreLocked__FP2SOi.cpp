#include "sly2/so.h"
// SO+0xD0 bit 34 = GLOBSET+0x50 bit 34 (Lilly's port: globset.fUnk34).
/** @sly2-annot addr=0x1F2768
 * Purpose: Set +0xD0 bit34 (fIgnoreLocked)
 * Info: proposed name SetSoIgnoreLocked__FP2SOi (EXISTING); TU so
 * Confidence: high [EXISTING 0.85] - evidence: pre-existing symbol in symbol_addrs.txt; sources:
 *   r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions, docs/engine/physics-collision.md#open-questions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void SetSoIgnoreLocked(SO *pso, int f) { pso->globset.fUnk34 = f; }
