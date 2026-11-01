struct VTE { short d; short pad; void (*pf)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x4C]; struct O *po; int n50; void *pv; };
void func_00130B18(struct O *po);
void func_00143FF8(void *pv);
/** @sly2-annot addr=0x1E5370
 * Purpose: Release the running ASEGA (+0x4C: 0x130B18 + Release) and the aseg step hold (+0x54 -> 0x143FF8)
 * Info: proposed name ClearJsgAseg (T1); TU relay
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E5370(struct S *p)
{
    if (p->po) {
        func_00130B18(p->po);
        { struct O *po = p->po; po->vt[2].pf((char *)po + po->vt[2].d); }
        p->po = 0;
    }
    if (p->pv) {
        func_00143FF8(p->pv);
        p->pv = 0;
    }
}
