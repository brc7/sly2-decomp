#include "common.h"
struct T { char pad[0x10]; ulong fl; };
struct S { char pad[0x28]; ulong fl; };
void RemoveSoPartFromTree(struct S *, struct T *);
/** @sly2-annot addr=0x1F2AD0
 * Purpose: Set part hidden bit50 and, when in world, RemoveSoPartFromTree
 * Info: proposed name HideSoPart (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void HideSoPart(struct S *p, struct T *pt)
{
    if (!(pt->fl & (1UL << 50))) {
        pt->fl |= 1UL << 50;
        if ((int)(p->fl >> 60) & 1) RemoveSoPartFromTree(p, pt);
    }
}
