struct VTE { short d; short p; void (*pfn)(void *, int); };
struct VT { struct VTE a[48]; };
struct L { char pad[0xC]; struct VT *pvt; char pad1[0x24 - 0x10]; struct L *next; char pad1b[0x40 - 0x28]; int v40; char pad2[0x64 - 0x44]; struct L *first; char pad3[0x70 - 0x68]; struct L *end; };
/** @sly2-annot addr=0x1F2FE0
 * Purpose: SO vt47: grf(+0x40 tagged word) |= f, then vt47 on each child
 * Info: proposed name SetSoGrf (T1); class SO slot47; TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void SetSoGrf(struct L *p, int f)
{
    struct L *q;
    int unused[4];
    int v = p->v40;
    if ((v ^ 1) & 1) *(int *)v |= f;
    else p->v40 = v | f;
    for (q = p->first; q != p->end; q = q->next)
        q->pvt->a[47].pfn((char *)q + q->pvt->a[47].d, f);
}
