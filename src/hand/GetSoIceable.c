#include "common.h"
struct S { char pad[0x148]; ulong fl; };
/** @sly2-annot addr=0x1F8500
 * Purpose: Return +0x148 bit33
 * Info: proposed name GetSoIceable (T1); TU so
 * Notes: names_v2 v1 proposed SetSoMtlk__FP2SO4MTLK (0.008)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int GetSoIceable(struct S *p) { return (int)(p->fl >> 33) & 1; }
