struct G;
extern struct G D_003DD490;
struct B { int id; void *p; };
struct S { char pad[0x110]; int c110; struct B a114[0x10]; int c194; struct B a198[1]; };
void func_002123C8(struct S *p);
void *PloFindSwObject__FP2SWi3OIDP2LO(struct G *g, int k, int id, int z);
/** @sly2-annot addr=0x211EC0
 * Purpose: CID 0x81 PostLoad: PostWidgetLoad then resolve both oid lists to LOs (PloFindSwObject grffso 5)
 * Info: proposed name PostWidget129Load (T2); class WIDGET129 slot15; TU hud
 * Notes: v2: demoted to T2 (placeholder token in name: 129)
 * Confidence: low (draft) [T2 0.65] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00211EC0(struct S *p)
{
    int i;
    func_002123C8(p);
    for (i = 0; i < p->c194; i++) {
        void *q = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 5, p->a198[i].id, 0);
        if (q)
            p->a198[i].p = q;
    }
    for (i = 0; i < p->c110; i++) {
        void *q = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 5, p->a114[i].id, 0);
        if (q)
            p->a114[i].p = q;
    }
}
