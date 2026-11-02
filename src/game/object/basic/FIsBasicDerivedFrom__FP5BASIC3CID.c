struct VTE { short d; short pad; void *(*pfn)(); };
struct O { int pad[3]; struct VTE *vt; };
struct R { int f0; int f4; };
struct T { int f0; int f4; int f8; int fc; int f10; };
extern struct T D_002BAAE8[];
/** @sly2-annot addr=0x132B10
 * Purpose: cid range test: Pclassinfo()->cid in [cid, D_002BAAE8[cid].cidMax)
 * Info: proposed name FIsBasicDerivedFrom (T1); TU basic
 * Confidence: high [T1 0.92] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/object-model.md#1-class-descriptors-and-vtables,
 *   docs/engine/object-model.md#9-key-functions,
 *   docs/reference/functions/object-model.md#basic-0x132b00-0x1336f0
 * @end-sly2-annot */
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *po, int n) {
    struct R *pr = (struct R *)((void *(*)(void *))po->vt[6].pfn)((char *)po + po->vt[6].d);
    int x = pr->f4;
    int f = 0;
    if (x >= n) { struct T *pt = &D_002BAAE8[n]; f = x < pt->f8; }
    return f;
}
