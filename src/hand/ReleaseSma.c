struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { int a[3]; struct VTE *vt; };
struct S { struct O *po; };
void func_001F0598(struct O *);
/** @sly2-annot addr=0x1F15B8
 * Purpose: Retract, Decref, null
 * Info: proposed name ReleaseSma (T1); TU sm
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768,
 *   docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
void ReleaseSma(struct S *p)
{
    if (p->po) {
        func_001F0598(p->po);
        { struct O *po = p->po; po->vt[2].pfn((char *)po + po->vt[2].d); }
        p->po = 0;
    }
}
