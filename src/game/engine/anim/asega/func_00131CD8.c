struct VTE { short d; short pad; void (*pfn)(); };
struct X { char pad[0x88]; int f88; int f8c; char pad2[0x9a - 0x90]; short f9a; };
struct C { struct VTE *vt; };
struct S { int f0; int f4; struct X *px; int pad[4]; struct C *pc; int f20; int f24; };
void func_002303A0(int, struct X *, int);
void func_0022C818(int, struct X *);
/** @sly2-annot addr=0x131CD8
 * Purpose: Applies channel to target (+0x88) and kicks state
 * Info: proposed name AsegaChnApply (T2); TU asega
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#asega-0x1308a8-0x131f70
 * @end-sly2-annot */
void func_00131CD8(struct S *p) {
    if (p->pc) {
        struct C *pc;
        p->px->f88 = p->f24 + 0x1c;
        p->px->f8c = p->f24 + 0x40;
        pc = p->pc;
        ((void (*)(void *, struct X *))pc->vt[2].pfn)((char *)pc + pc->vt[2].d, p->px);
    } else {
        p->px->f9a = 0;
        func_002303A0(p->f4, p->px, 0);
        func_0022C818(p->f4, p->px);
    }
}
