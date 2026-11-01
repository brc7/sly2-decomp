struct VE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VE *vt; };
struct L { char pad[0x20]; int x20; };
struct S { char pad[0xC]; struct VE *vt; char pad1[0xE0 - 0x10]; int c; struct O *ao[4]; int af[4]; char pad2[0x128 - 0x104]; struct L *x128; };
#define VCALL(o, n, args...) ((void (*)())(o)->vt[n].pfn)((char *)(o) + (o)->vt[n].d, args)
#define VCALL0(o, n) ((void (*)())(o)->vt[n].pfn)((char *)(o) + (o)->vt[n].d)
/** @sly2-annot addr=0x1BED18
 * Purpose: Release slot, restore objects (vt9 / vt21(1)), self vt21(0), vt3 msg 3
 * Info: proposed name ReleaseCid45RenderSlot (T2); class CID45; TU tu_1BE398
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/rendering.md#tu_1be398-0x1be398-0x1bf110
 * @end-sly2-annot */
void func_001BED18(struct S *p)
{
    int i;
    if (!p->x128) return;
    p->x128->x20 = 0;
    p->x128 = 0;
    for (i = 0; i < p->c; i++) {
        struct O *o = p->ao[i];
        if (p->af[i] & 1) VCALL0(o, 9);
        if (p->af[i] & 2) VCALL(o, 21, 1);
    }
    VCALL(p, 21, 0);
    VCALL(p, 3, 3, 0, 0, 0, 0);
}
