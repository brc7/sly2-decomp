struct VTE { short d; short pad; void *(*pfn)(); };
struct O { int pad[3]; struct VTE *vt; int pad2[2]; int f18; };
struct R { int f0; int f4; };
struct O *PloNew__F3CIDP2SO3OIDi(int, int, int, int);
/** @sly2-annot addr=0x13E510
 * Purpose: Clone object: PloNew(Pclassinfo(po)->cid, psoParent, po->oid, -1) then vt40 (clone-from, which for
 *   SO also clones children via PloCloneLo)
 * Info: proposed name PloCloneLo (T1); TU brx
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: high [T1 0.93] - evidence: semantic review; sources: r2_131F70+parse-verified
 * See: docs/engine/object-model.md#3-creation-hierarchy-and-removal,
 *   docs/engine/world-sw.md#loadswfrombrx-0x13da18-matched-hand-file,
 *   docs/engine/world-sw.md#10-key-functions
 * @end-sly2-annot */
struct O *PloCloneLo__FP2LOP2SO(struct O *po, int a) {
    struct R *pr;
    struct O *pn;
    if (!po) return 0;
    pr = (struct R *)((void *(*)(void *))po->vt[6].pfn)((char *)po + po->vt[6].d);
    pn = PloNew__F3CIDP2SO3OIDi(pr->f4, a, po->f18, -1);
    ((void (*)(void *, struct O *))pn->vt[40].pfn)((char *)pn + pn->vt[40].d, po);
    return pn;
}
