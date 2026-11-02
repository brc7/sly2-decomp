
struct VTE { short d; short pad; void (*pf)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x58]; struct O *po; int n5C; };
void UnsubscribeLoObject(struct O *po, struct S *p);
/** @sly2-annot addr=0x1E5410
 * Purpose: Unregister from and release psmaCur (+0x58), fSmaReached=0
 * Info: proposed name ClearJsgSma (T1); TU relay
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void ClearJsgSma(struct S *p)
{
    if (p->po) {
        UnsubscribeLoObject(p->po, p);
        { struct O *po = p->po; po->vt[2].pf((char *)po + po->vt[2].d); }
        p->po = 0;
        p->n5C = 0;
    }
}
