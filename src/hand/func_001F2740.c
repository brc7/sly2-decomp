#include "common.h"
struct S { char pad[0x148]; ulong a:20; int b:4; ulong c:40; };
/** @sly2-annot addr=0x1F2740
 * Purpose: Get 4-bit field +0x148 bits 20-23
 * Info: proposed name GetSoField148_20 (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F2740(struct S *p) { return p->b; }
