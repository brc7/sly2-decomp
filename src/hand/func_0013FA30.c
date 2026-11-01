struct VTE { short d; short pad; void (*pfn)(); };
struct O { int pad[3]; struct VTE *vt; };
struct S { int f0; int f4; int f8; };
void func_001A58D0(struct O *, int, int);
void func_0013F990(struct O *, int);
/** @sly2-annot addr=0x13FA30
 * Purpose: Collector used for k=-1/-2 objects: per object vt10 SetParent(0,0,0), SetLoNUnk48Hierarchy(x4),
 *   and if x8!=-1 vt25 SetIdProxy(x8,1)
 * Info: proposed name CLoadCollectorProxy_Add (T1); class CLoadCollectorProxy; TU brx
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/world-sw.md#brx-0x13e368-0x13ff70
 * @end-sly2-annot */
void func_0013FA30(struct S *p, int c, struct O **apo) {
    int i;
    for (i = 0; i < c; i++) {
        struct O *po = apo[i];
        func_001A58D0(po, 0, 0);
        func_0013F990(po, p->f4);
        if (p->f8 != -1) ((void (*)(void *, int, int))po->vt[25].pfn)((char *)po + po->vt[25].d, p->f8, 1);
    }
}
