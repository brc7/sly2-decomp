#include "common.h"
struct S { char pad[0x148]; ulong a:16; int b:4; ulong c:44; };
/** @sly2-annot addr=0x1F7EC8
 * Purpose: Set 4-bit field +0x148 bits 16-19
 * Info: proposed name SetSoField148_16 (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F7EC8(struct S *p, int n) { p->b = n; }
