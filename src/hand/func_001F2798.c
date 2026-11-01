typedef unsigned long ulong;
struct Q { char pad[0xA0]; int a0; char pad2[0x1C]; int c0; };
struct A { char pad[0x10]; struct A *next; char pad2[0xD4]; struct Q *pq; char pad3[0x5C]; ulong fl; };
/** @sly2-annot addr=0x1F2798
 * Purpose: Walk up the parent chain to the first SO that has non-fixed physics (SOPHYS modes != 3) or flag
 *   +0x148 bit34; returns last visited
 * Info: proposed name PsoFindPhysRoot (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct A *func_001F2798(struct A *p)
{
    struct A *r = 0;
    while (p) {
        r = p;
        if (p->pq && (p->pq->c0 != 3 || p->pq->a0 != 3))
            break;
        if (p->fl & (1L << 34))
            break;
        p = p->next;
    }
    return r;
}
