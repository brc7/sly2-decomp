#include "common.h"
struct S { char pad[0x148]; ulong fl; };
/** @sly2-annot addr=0x1F6960
 * Purpose: Get +0x148 bit26
 * Info: proposed name GetSoFlag26 (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F6960(struct S *p) { return (int)((long)(p->fl << 6) >> 32) & 1; }
