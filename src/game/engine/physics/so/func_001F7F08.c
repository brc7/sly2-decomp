#include "common.h"
struct S { char pad[0x148]; ulong fl; };
/** @sly2-annot addr=0x1F7F08
 * Purpose: Get +0x148 bit29
 * Info: proposed name GetSoFlag29 (T2); TU so
 * Notes: names_v2 v1 proposed AddSoAngularAcceleration__FP2SOP6VECTOR (0.054)
 * Confidence: low (draft) [T2 0.20] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F7F08(struct S *p) { return (int)((long)(p->fl << 3) >> 32) & 1; }
