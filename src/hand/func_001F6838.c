#include "common.h"
struct T { char pad[0x70]; ulong fl; };
struct S { char pad[0xEC]; struct T *pt; };
/** @sly2-annot addr=0x1F6838
 * Purpose: Bit 2 of tree flags (+0x70), 1 if no tree
 * Info: proposed name FSoTreeFlag2 (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F6838(struct S *p) { if (!p->pt) return 1; return (int)((long)(p->pt->fl << 30) >> 32) & 1; }
