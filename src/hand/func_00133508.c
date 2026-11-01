struct VTE { short d; short pad; void (*pfn)(); };
struct O { int pad[3]; struct VTE *vt; };
struct N { struct O *po; int f4; struct N *pnext; };
struct S { int f0; int f4; struct N *pfirst; struct VTE *vt; };
/** @sly2-annot addr=0x133508
 * Purpose: BASIC vt4: own vt5 HandleMessage, then vt5 of every subscriber
 * Info: proposed name SendLoMessage (T1); class SW slot4; TU basic
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/object-model.md#2-the-lo-virtual-interface, docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
void func_00133508(struct S *p, int a, int b) {
    struct N *pn;
    ((void (*)(void *, int, int))p->vt[5].pfn)((char *)p + p->vt[5].d, a, b);
    for (pn = p->pfirst; pn; ) {
        struct O *po = pn->po;
        pn = pn->pnext;
        ((void (*)(void *, int, int))po->vt[5].pfn)((char *)po + po->vt[5].d, a, b);
    }
}
