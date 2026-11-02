typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x20]; union { int n30; qword v30; struct { int a, b, n38; } s; } u30; union { int n40; qword v40; } u40; char pad5[0xC0 - 0x50]; int nC0; int pad6; int nC8; };
struct A { char pad[0x294]; int n294; char pad2[0xFE8 - 0x298]; int nFE8; };
struct S {
    char pad[0x10]; struct A *p10; int pad2; struct O *p18; char pad3[0x14]; int n30; float f34;
    char pad4[0x80 - 0x38]; qword v80; qword v90; char pad5[0xA8 - 0xA0]; int nA8; int nAC; char pad6[8]; int nB8; int nBC;
};
void PopBhv(void *p);
struct O *func_00205438(void *p, int n);
void SetBhvpathShape(struct O *o, int a, int b);
/** @sly2-annot addr=0x17D450
 * Purpose: BHVGOTO state: 4 stand (BHVIDLE), 1/2 walk path (BHVWALKPATH, run anim 8 when allowed), 3 traverse
 *   pathlink (BHVPATHLINK), 5 relocate (BHVRELOCATE)
 * Info: proposed name SetBhvgotoState (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVGOTO role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void SetBhvgotoState(struct S *p, int n) {
    if (n == p->n30) return;
    if (!((unsigned)n < 3 && p->n30 >= 0 && p->n30 < 3)) PopBhv((char *)p + 0x18);
    p->n30 = n;
    p->f34 = D_002DDED0.t;
    switch (n) {
    case 4:
        { struct O *o = func_00205438(p->p10, 0x9E); struct VTE *e; o->u30.s.n38 = p->nA8; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    case 3:
        { struct O *o = func_00205438(p->p10, 0xBE); struct VTE *e; o->u30.n30 = p->nB8; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    case 1:
    case 2:
        {
            struct O *o = p->p18;
            if (o == 0) {
                struct VTE *e;
                o = func_00205438(p->p10, 0xA7);
                o->nC8 = p->nA8;
                o->u40.n40 = p->nBC;
                if (!p->nA8 || p->nAC || !(p->p10->nFE8 & 4)) o->nC0 = 9; else o->nC0 = 8;
                SetBhvpathShape(o, p->p10->n294, 0);
                e = &o->vt[8]; e->pfn((char *)o + e->d);
            } else {
                SetBhvpathShape(o, p->p10->n294, 0);
            }
        }
        break;
    case -1:
    case 0:
        break;
    case 5:
        { struct O *o = func_00205438(p->p10, 0xBF); struct VTE *e; o->u30.v30 = p->v80; o->u40.v40 = p->v90; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    }
}
