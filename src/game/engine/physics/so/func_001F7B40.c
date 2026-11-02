#include "common.h"
struct S { char pad[0x148]; ulong fl; };
/** @sly2-annot addr=0x1F7B40
 * Purpose: +0x148 bits 24-25
 * Info: proposed name GetSoZapMode (T2); TU so
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F7B40(struct S *p) { return (int)((long)(p->fl << 8) >> 32) & 3; }
