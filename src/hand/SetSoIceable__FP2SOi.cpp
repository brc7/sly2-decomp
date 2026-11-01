#include "sly2/so.h"
// SO+0x148 bit 33 (64-bit unit at 0x148). Lilly's port calls this bit fHurtRespawns; the function name says "iceable".
/** @sly2-annot addr=0x1F84D0
 * Purpose: Set +0x148 bit33 (fIceable)
 * Info: proposed name SetSoIceable__FP2SOi (EXISTING); TU so
 * Confidence: high [EXISTING 0.85] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void SetSoIceable(SO *pso, int f) { pso->fHurtRespawns = f; }
