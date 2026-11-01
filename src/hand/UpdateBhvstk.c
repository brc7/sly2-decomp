struct VTE { short d; short pad; void (*pf)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { int n0; int c; struct O *ap[1]; };
/** @sly2-annot addr=0x1EED10
 * Purpose: Call BHV vt10 (Update) on every stacked behaviour, top (last pushed) first
 * Info: proposed name UpdateBhvstk (T1); TU bhvstk
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void UpdateBhvstk(struct S *p)
{
    int i;
    for (i = p->c; --i >= 0;) {
        struct O *po = p->ap[i];
        po->vt[10].pf((char *)po + po->vt[10].d);
    }
}
