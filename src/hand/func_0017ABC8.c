typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct VTE { short d; short pad; void *pfn; };
struct O { char pad[0xC]; struct VTE *vt; };
struct R { int n0; int a[11]; };
struct U { char pad[0x1C]; float g1C; };
struct W { char pad[0xA0]; int nA0; };
struct T { char pad[0x58]; struct W *p58; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x48]; struct O *p5C; int n60; struct U *p64; };
void func_0017AA78(struct S *p, struct R *r, qword *a, qword *b);
qword func_0022FC48(int a, int b, qword q);
qword func_0022FCC8(int a, int b, qword q);
void func_0012E0F0(struct U *u, qword a, qword b);
/** @sly2-annot addr=0x17ABC8
 * Purpose: Re-query the holder and update the attach ACT target
 * Info: proposed name UpdateBhvjuggledHoldAct (T2); TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded:
 *   UnadjustAloRotation (names_v2:B(unverified)) | BHV class word normalised: UpdateBhvheldHoldAct ->
 *   UpdateBhvjuggledHoldAct | runtime2/4 confirmed the BHVJUGGLED role on live guards (S02-S41)
 * Confidence: low (draft) [T2 0.45] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017ABC8(struct S *p) {
    struct R r;
    qword q30, q40;
    struct W *w;
    qword a;
    if (!p->n60) return;
    { struct O *o = p->p5C; struct VTE *e = &o->vt[0x60]; ((void (*)(void *, struct R *, float))e->pfn)((char *)o + e->d, &r, p->p64->g1C - D_002DDED0.t); }
    if (!r.n0) { p->n60 = 0; return; }
    w = p->pt->p58;
    func_0017AA78(p, &r, &q30, &q40);
    a = func_0022FC48(0, w->nA0, q30);
    func_0012E0F0(p->p64, a, func_0022FCC8(0, w->nA0, q40));
}
