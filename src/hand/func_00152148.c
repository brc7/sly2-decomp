typedef int qword __attribute__((mode(TI)));
struct A { char pad[0x1C]; float g1C; char pad2[0x50 - 0x20]; int k50; };
struct P { char pad[0x58]; void *p58; void *p5C; char pad2[0x190 - 0x60]; qword v190; char pad3[0x2C0 - 0x1A0]; int n2C0;
    char pad4[0x408 - 0x2C4]; struct A *pa408; int f40C; };
extern struct A *D_002DE570;
void func_00203FC0(struct P *p);
int func_00131968(struct A *pa);
void func_002303A0(struct P *p, void *pv, int n);
void func_001524A0(struct P *p, int n, int m);
void func_00152390(struct P *p, int a, int b, int c);
void func_001533B0(struct A *pa, struct P *p);
qword func_00204E38(struct P *p, struct A *pa, float g);
static inline qword VecMul(qword v, float g)
{
    qword r;
    union { qword q; float f[4]; } s;
    __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(g));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(s.q));
    return r;
}
/** @sly2-annot addr=0x152148
 * Purpose: cid 0x44 vt16 per-frame: base 0x203FC0; when its aseg finished, return to idle; react to the
 *   rhythm manager's state (D_002DE570->+0x50: 0 idle move, 2 JudgeRhythmPress); keep +0x190 velocity from
 *   aseg
 * Info: proposed name UpdateDancer (T2); class DANCER slot16; TU rhythm
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void func_00152148(struct P *p)
{
    int k;
    func_00203FC0(p);
    if (!p->pa408 || func_00131968(p->pa408)) {
        func_002303A0(p, p->p58, 1);
        func_002303A0(p, p->p5C, 1);
        p->f40C = 0;
        func_001524A0(p, p->n2C0, 8);
    }
    k = D_002DE570 ? D_002DE570->k50 : 0;
    switch (k) {
    case 0:
        if (!p->f40C)
            func_00152390(p, -1, -1, 0);
        break;
    case 2:
        func_001533B0(D_002DE570, p);
        break;
    }
    if (p->pa408)
        p->v190 = VecMul(func_00204E38(p, p->pa408, p->pa408->g1C), -1.0f);
}
