typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct X { qword a; qword v; };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x28]; int n38; int n3C; char pad3[0xA8 - 0x40]; void *pA8; int nAC; char pad4[0x10]; int nC0; };
struct S { char pad[0x10]; void *p10; int pad2; char a18[0x18]; void *p30; char pad3[0xC]; qword v40; int n50; float f54; char pad4[8]; qword v60; };
void PopBhv(void *p);
struct O *func_00205438(void *p, int n);
qword func_001A62E8(void *p);
qword ConvertAloPos(void *p, int n, qword v);
void func_002562A8(struct X *x);
void SetBhvgotoGoal(void *p, struct X *x);
/** @sly2-annot addr=0x17C928
 * Purpose: BHVESCORT state: 0 push BHVGOTO to the offset point, 1 push BHVGOTO with run flag 0x1C to the
 *   object, 2 idle hands-off
 * Info: proposed name SetBhv183State (T2); TU guardbhv
 * Notes: BHV class word normalised: SetBhvescortState -> SetBhv183State
 * Confidence: low (draft) [T2 0.85] - evidence: static analysis; sources: r9_bhv_screen+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017C928(struct S *p, int n) {
    struct X x;
    if (n == p->n50) return;
    PopBhv(p->a18);
    p->n50 = n;
    p->f54 = D_002DDED0.t;
    switch (n) {
    case 1:
        {
            struct O *o = func_00205438(p->p10, 0xBA);
            qword v = func_001A62E8(p->p30);
            struct VTE *e;
            p->v60 = v;
            o->nC0 = 1; o->pA8 = p->p30; o->nAC = 1;
            func_002562A8(&x);
            x.v = v;
            SetBhvgotoGoal(o, &x);
            o->n38 = 0x1C;
            e = &o->vt[8]; e->pfn((char *)o + e->d);
        }
        break;
    case 2:
        {
            struct O *o = func_00205438(p->p10, 0x9E);
            struct VTE *e;
            o->n38 = (int)p->p30;
            o->n3C = 1;
            e = &o->vt[8]; e->pfn((char *)o + e->d);
        }
        break;
    case 0:
        {
            struct O *o = func_00205438(p->p10, 0xBA);
            qword v;
            struct VTE *e;
            p->v60 = func_001A62E8(p->p30);
            v = ConvertAloPos(p->p30, 0, p->v40);
            o->nC0 = 1;
            func_002562A8(&x);
            x.v = v;
            SetBhvgotoGoal(o, &x);
            o->pA8 = p->p30;
            e = &o->vt[8]; e->pfn((char *)o + e->d);
        }
        break;
    }
}
