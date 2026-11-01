typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002A5408;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct X { qword a; qword v; };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x20]; int n30; int pad3; int n38; int n3C; char pad4[0xA8 - 0x40]; int nA8; int nAC; char pad5[0x10]; int nC0; };
struct A { char pad[0x3CC]; void *p3CC; char pad2[0xF04 - 0x3D0]; float fF04; char pad3[0xF64 - 0xF08]; void *pF64; };
struct P { char pad[0x60]; float f60; float f64; };
struct S { char pad[0x10]; struct A *p10; int pad2; char a18[0x18]; int n30; int n34; float f38; struct P *p3C; int pad4; int n44; char pad5[8]; qword v50; };
void PopBhv(void *p);
float GRandInRange__Fff(float a, float b);
struct O *func_00205438(void *p, int n);
void func_0020A9A8(void *p, int n);
qword func_001A62E8(int p);
void func_002562A8(struct X *x);
void SetBhvgotoGoal(void *p, struct X *x);
int PpathzoneFromPos(qword v, int n);
void SetDetectorTargetStateById(void *p, int a, int b);
void SetStepguardTarget(void *p, int n);
/** @sly2-annot addr=0x176458
 * Purpose: BHVCOMBAT state switch: 0 push BHVCIRCLE, 1 push BHVATTACK with entry, 2 push BHVSHOOT, 3 push
 *   BHVGOTO toward target with run flag 0x1C, 4 push BHVANIM flag 0xD (lost him) and drop target, 5 push
 *   BHVIDLE flag 1; leaving 1 sets cooldown
 * Info: proposed name SetBhvcombatState (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVCOMBAT role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void SetBhvcombatState(struct S *p, int n) {
    if (n == p->n34) return;
    PopBhv(p->a18);
    if (p->n34 == 1) {
        float t = D_002DDED0.t;
        t += GRandInRange__Fff(p->p3C->f60, p->p3C->f64);
        p->p10->fF04 = t;
        p->p3C = 0;
    }
    p->n34 = n;
    p->f38 = D_002DDED0.t;
    switch (n) {
    case 0:
        { struct O *o = func_00205438(p->p10, 0xB3); struct VTE *e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    case 1:
        {
            struct O *o = func_00205438(p->p10, 0xA4);
            struct VTE *e;
            o->n30 = p->n30;
            o->n3C = (int)p->p3C;
            p->p10->fF04 = D_002A5408;
            e = &o->vt[8]; e->pfn((char *)o + e->d);
        }
        func_0020A9A8(p->p10->p3CC, 3);
        break;
    case 2:
        { struct O *o = func_00205438(p->p10, 0xAD); struct VTE *e; o->n38 = p->n30; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        func_0020A9A8(p->p10->p3CC, 4);
        break;
    case 3:
        {
            struct O *o = func_00205438(p->p10, 0xBA);
            qword v1, v2;
            struct X x;
            struct VTE *e;
            o->nC0 = 1;
            o->n38 = 0x1C;
            v1 = func_001A62E8(p->n30);
            v2 = func_001A62E8(p->n30);
            func_002562A8(&x);
            x.v = v2;
            SetBhvgotoGoal(o, &x);
            o->nA8 = p->n30;
            o->nAC = 1;
            e = &o->vt[8]; e->pfn((char *)o + e->d);
            p->v50 = v1;
            p->n44 = PpathzoneFromPos(v1, 1);
        }
        break;
    case 4:
        { struct O *o = func_00205438(p->p10, 0x9D); struct VTE *e; o->n3C = 0xD; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        SetDetectorTargetStateById(p->p10->pF64, p->n30, 0);
        SetStepguardTarget(p->p10, 0);
        break;
    case 5:
        { struct O *o = func_00205438(p->p10, 0x9E); struct VTE *e; o->n3C = 1; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    }
}
