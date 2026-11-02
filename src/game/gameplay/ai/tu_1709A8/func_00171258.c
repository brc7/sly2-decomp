struct S;
struct VTE { short d; short pad; void (*pfn)(); };
struct T { int n0; char pad[0x4C]; int n50; };
struct U { int n0; int n4; };
struct S { int pad0; char a4[8]; struct VTE *vt; char pad[8]; char a18[0x18]; struct T *pt; int n34; int n38; int pad3; int n40; struct U *pu; int pad4; int n4C; };
void PopBhv(void *p);
void FramePtr_Assign(struct S *p, int n);
void func_002580A8(void *p, int n);
/** @sly2-annot addr=0x171258
 * Purpose: Switches the script (cid 0x71) a BHV169 runs: stops the old one (message 3), starts the new
 *   (message 2) at its first command or a saved resume point
 * Info: proposed name SetBhv169Script (T2); TU tu_1709A8
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00171258(struct S *p, struct T *t, int a, struct U *u) {
    if (t == p->pt) return;
    if (p->pt) {
        PopBhv(p->a18);
        p->vt[3].pfn((char *)p + p->vt[3].d, 3, 0, 0, 0, 0);
        FramePtr_Assign(p, 0);
        func_002580A8(p->a4, 0);
        p->n34 = 0;
        p->n38 = 0;
        p->n40 = 0;
        p->n4C = 0;
    }
    p->pt = t;
    if (!t) return;
    FramePtr_Assign(p, t->n0);
    p->vt[3].pfn((char *)p + p->vt[3].d, 2, 0, 0, 0, 0);
    p->n4C = a;
    if (!u || !u->n0) {
        struct T *x = p->pt;
        int n = x->n50;
        p->n34 = (int)x;
        p->n38 = n;
    } else {
        p->n38 = u->n0;
        p->pu = u;
        p->n34 = u->n4;
    }
    if (p->n38) p->vt[15].pfn((char *)p + p->vt[15].d, p->n38);
    p->pu = 0;
}
