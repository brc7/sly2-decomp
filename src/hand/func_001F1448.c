struct VTE { short d; short p; void (*pfn)(void *); };
struct VT { struct VTE a[3]; };
struct X { char pad[0xC]; struct VT *pvt; };
void func_001F0598(struct X *);
struct X *func_001F0380(void *, int, int, int);
/** @sly2-annot addr=0x1F1448
 * Purpose: Retract and Decref *ppsma; if a new SM is given, PsmaApplySm, Incref, store
 * Info: proposed name ReplaceSma (T1); TU sm
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768,
 *   docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void func_001F1448(void *pv, int a, int b, int c, struct X **ppx)
{
    if (*ppx) {
        func_001F0598(*ppx);
        (*ppx)->pvt->a[2].pfn((char *)*ppx + (*ppx)->pvt->a[2].d);
        *ppx = 0;
    }
    if (pv) {
        struct X *px = func_001F0380(pv, a, b, c);
        *ppx = px;
        px->pvt->a[1].pfn((char *)px + px->pvt->a[1].d);
    }
}
