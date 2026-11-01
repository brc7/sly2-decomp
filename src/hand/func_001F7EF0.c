#include "common.h"
struct S { char pad[0x148]; ulong a:16; int b:4; ulong c:44; };
/** @sly2-annot addr=0x1F7EF0
 * Purpose: Get 4-bit field +0x148 bits 16-19
 * Info: proposed name GetSoField148_16 (T2); TU so
 * Notes: names_v2 v1 proposed AddSoAcceleration__FP2SOP6VECTOR (0.054)
 * Confidence: low (draft) [T2 0.20] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F7EF0(struct S *p) { return p->b; }
