#include "common.h"
struct S { char pad[0xD0]; ulong fl; };
/** @sly2-annot addr=0x1F2758
 * Purpose: Return bit 34 of +0xD0 (fIgnoreLocked)
 * Info: proposed name GetSoIgnoreLocked (T1); TU so
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int GetSoIgnoreLocked(struct S *p) { return (int)(p->fl >> 34) & 1; }
