struct VTE { short d; short p; void *pfn; };
struct VT { struct VTE a[5]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0x24 - 0x10]; void *p24; };
struct L { struct P *p; int a; int b; };
int func_001F04F8(void *, int);
/** @sly2-annot addr=0x1F13A8
 * Purpose: Builds {psma, oidFrom, oidTo} and sends message 10 (notify) and 8 (Splice event with 2 args)
 *   through vtable slots 4/3
 * Info: proposed name NotifySmaSpliceOnEnterState (T1); TU sm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
void func_001F13A8(struct P *p, int a, int b)
{
    struct L l;
    int *ap[2];
    l.p = p;
    l.a = func_001F04F8(p->p24, a);
    l.b = func_001F04F8(p->p24, b);
    ((void (*)(void *, int, struct L *))p->pvt->a[4].pfn)((char *)p + p->pvt->a[4].d, 10, &l);
    ap[0] = &l.a;
    ap[1] = &l.b;
    ((void (*)(void *, int, int, int, int **, int))p->pvt->a[3].pfn)((char *)p + p->pvt->a[3].d, 8, 0, 2, ap, 0);
}
