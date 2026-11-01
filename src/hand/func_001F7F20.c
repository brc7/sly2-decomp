#include "common.h"
struct S { char pad[0x148]; ulong a:29; int b:1; ulong c:34; };
/** @sly2-annot addr=0x1F7F20
 * Purpose: Set +0x148 bit29
 * Info: proposed name SetSoFlag29 (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7F20(struct S *p, int n) { p->b = n; }
