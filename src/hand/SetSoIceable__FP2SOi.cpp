#include "common.h"
struct SO { char pad[0x148]; ulong a:33; int fIceable:1; ulong c:30; };
/** @sly2-annot addr=0x1F84D0
 * Purpose: Set +0x148 bit33 (fIceable)
 * Info: proposed name SetSoIceable__FP2SOi (EXISTING); TU so
 * Confidence: high [EXISTING 0.85] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void SetSoIceable(SO *pso, int f) { pso->fIceable = f; }
