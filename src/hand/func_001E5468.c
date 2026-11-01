struct VTE { short d; short pad; void (*pf)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x64]; struct O *po; };
/** @sly2-annot addr=0x1E5468
 * Purpose: Call vt50 on ploWidget (+0x64) and clear it
 * Info: proposed name ClearJsgWidget (T2); TU relay
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E5468(struct S *p)
{
    struct O *po = p->po;
    if (po) {
        po->vt[50].pf((char *)po + po->vt[50].d);
        p->po = 0;
    }
}
