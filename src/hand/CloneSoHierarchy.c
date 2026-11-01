typedef unsigned long ulong;
struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[42]; };
struct A { char pad[0xC]; struct VT *pvt; };
struct L { char pad[0x24]; struct L *next; ulong fl; char pad2[0x64 - 0x30]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; };
void PloCloneLo__FP2LOP2SO(struct L *, struct A *);
/** @sly2-annot addr=0x1F66F0
 * Purpose: SO vt40: vt41 Clone then PloCloneLo each non-removed child
 * Info: proposed name CloneSoHierarchy (T1); class SO slot40; TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void CloneSoHierarchy(struct A *pa, struct L *p)
{
    struct L *q;
    pa->pvt->a[41].pfn((char *)pa + pa->pvt->a[41].d);
    for (q = p->first; q != p->end; q = q->next)
        if (!(q->fl & (1L << 57))) PloCloneLo__FP2LOP2SO(q, pa);
}
