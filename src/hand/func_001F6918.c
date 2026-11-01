#include "common.h"
struct S { char pad[0x148]; ulong a:26; int b:1; ulong c:37; };
struct G;
extern struct G D_003DD490;
void RecalcSwOxfFilterForObject(struct G *, struct S *);
/** @sly2-annot addr=0x1F6918
 * Purpose: Set +0x148 bit26 and refresh contacts (0x134320)
 * Info: proposed name SetSoFlag26 (T2); TU so
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F6918(struct S *p, int f) { p->b = f; RecalcSwOxfFilterForObject(&D_003DD490, p); }
