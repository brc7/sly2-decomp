typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern float D_002BE760, D_002BE764, D_002BE768;
struct VTE { short d; short pad; void *pfn; };
struct X { qword a; qword v; };
struct T { char pad[0x40]; struct X x40; };
struct A { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x10]; struct A *p10; int pad2; struct T *p18; char pad3[0x14]; int n30; int pad4; void *p38; float f3C; float f40; };
void func_001EEF98(struct S *p);
int func_001EF0A8(struct S *p);
int func_00174FB8(void *p);
int func_00209098(void *p);
void func_0017E278(struct S *p, int n);
int func_00208B38(struct A *a, void *o);
float func_00208BB0(struct A *a);
qword func_001A62E8(void *p);
qword func_001C1EC0(struct X *s, qword v);
void func_0017E0E0(struct S *p);
static __inline__ float Dist(qword a, qword b) {
    qword d;
    float f;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    __asm__("vmul.xy %0, %1, %1\n\tvaddy.x %0, %0, %0y" : "=j"(d) : "j"(d));
    __asm__("vsqrt $Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, $Q" : "=j"(d) : "j"(d));
    __asm__("qmfc2.ni %0, %1" : "=r"(f) : "j"(d));
    return f;
}
/** @sly2-annot addr=0x17DE50
 * Purpose: BHVSTALK Update: after the alert anim run near the forced-class target and keep re-positioning;
 *   leave when the delay expires and the target is no longer forced
 * Info: proposed name UpdateBhv189 (T2); class BHV189 slot10; TU guardbhv
 * Notes: BHV class word normalised: UpdateBhvstalk -> UpdateBhv189
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017DE50(struct S *p) {
    int s;
    func_001EEF98(p);
    while (s = p->n30, !func_001EF0A8(p)) {
        switch (p->n30) {
        case 0:
            if (func_00174FB8(p->p18)) func_0017E278(p, 1);
            break;
        case 1:
            if (func_00209098(p->p10)) {
                func_0017E278(p, 2);
            } else {
                { float dt = D_002DDED0.dt; p->f40 -= dt; }
                if (p->f40 <= 0.0f) {
                    void *o;
                    { struct A *a = p->p10; struct VTE *e = &a->vt[0x58]; o = ((void *(*)(void *))e->pfn)((char *)a + e->d); }
                    if (o == p->p38 && func_00208B38(p->p10, o)) {
                        p->f40 = func_00208BB0(p->p10);
                    } else {
                        func_0017E278(p, -1);
                    }
                }
            }
            break;
        case 2:
            if (!func_00209098(p->p10)) func_0017E278(p, 1);
            break;
        }
        if (p->n30 != s) continue;
        break;
    }
    if (p->n30 == 1) {
        struct T *t = p->p18;
        if (D_002DDED0.t - p->f3C > D_002BE760) {
        struct X *x = &t->x40;
        qword pos = func_001A62E8(p->p10);
        int f = 0;
        qword q = func_001C1EC0(x, pos);
        if (Dist(func_001A62E8(p->p10), q) < D_002BE764) {
            f = 1;
        } else if (Dist(func_001A62E8(p->p10), func_001A62E8(p->p38)) < D_002BE768) {
            f = 1;
        }
        if (f) func_0017E0E0(p);
        }
    }
}
