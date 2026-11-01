#include "common.h"
struct T { char pad[0x70]; ulong fl; };
struct S { char pad[0xEC]; struct T *pt; };
struct DL;
extern struct DL D_003E11B8;
void func_0011A2E8(struct DL *, void *);
/** @sly2-annot addr=0x1F4BA8
 * Purpose: Clear tree flag bit0 and remove from D_003E11B8
 * Info: proposed name ClearSoTreeBoundsDirty (T2); TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F4BA8(struct S *p)
{
    struct T *pt = p->pt;
    if (!pt) return;
    if (!(pt->fl & 1)) return;
    pt->fl &= ~1;
    func_0011A2E8(&D_003E11B8, p->pt);
}
