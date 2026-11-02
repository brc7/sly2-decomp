struct VTE { short d; short pad; void (*pfn)(); };
struct O { int pad[3]; struct VTE *vt; char pad2[0x1020]; int f1030; };
struct S { struct O *po; };
int FAsegaRetracted(int);
/** @sly2-annot addr=0x1371C8
 * Purpose: If action +0x1030 done, vt88(0)
 * Info: proposed name JtHelperStopScript (T2); TU bentley
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_001371C8(struct S *p) {
    if (FAsegaRetracted(p->po->f1030)) {
        struct O *po = p->po; struct VTE *vt = po->vt;
        ((void (*)(void *, int))vt[88].pfn)((char *)po + vt[88].d, 0);
    }
}
