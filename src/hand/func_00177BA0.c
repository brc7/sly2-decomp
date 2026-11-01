typedef int qword __attribute__((mode(TI)));
struct G;
extern struct G D_003DD490;
extern float D_002A54D0;
struct VTE { short d; short pad; void *(*pfn)(void *); };
struct A { char pad[0xC]; struct VTE *vt; char pad2[0xFBC - 0x10]; int nFBC; char pad3[0x1010 - 0xFC0]; int n1010; };
struct S { char pad[0x10]; struct A *p10; char pad2[0x1C]; int n30; };
struct F { int a; int pad[3]; void *p10; float f14; int rest[0x16]; };
void *func_002333B4(void *p, int c, int n);
int func_00132B10(struct A *p, int n);
void *func_001C4840(struct G *g);
int func_00177D20(struct S *p);
int func_00177D70(struct S *p);
int func_00208B38(struct A *p, void *q);
float func_00208BB0(struct A *p);
qword func_001A62E8(struct A *p);
void *func_00156B98(struct G *g, int n, qword v, struct A *p);
void func_00176750(struct S *p, struct F *f);
/** @sly2-annot addr=0x177BA0
 * Purpose: Pick the mode to resume after an interrupting mode ends: flee (cid 0x3B), stalk hero (special
 *   0x15), target list, BHV174, combat/stalk on current target, investigate own position after being hit,
 *   else patrol
 * Info: proposed name ResumeBhvguard (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVGUARD role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00177BA0(struct S *p) {
    struct F f;
    func_002333B4(&f, 0, sizeof f);
    if (func_00132B10(p->p10, 0x3B) && p->p10->n1010 > 0) {
        f.a = 0xD;
        f.p10 = func_001C4840(&D_003DD490);
    } else if (p->p10->nFBC == 0x15) {
        f.a = 0x12;
        f.p10 = func_001C4840(&D_003DD490);
        f.f14 = D_002A54D0;
    } else if (func_00177D20(p)) {
        f.a = 0xC;
    } else if (func_00177D70(p)) {
        f.a = 0xF;
    } else {
        void *q;
        { struct A *a = p->p10; struct VTE *e = &a->vt[0x58]; q = e->pfn((char *)a + e->d); }
        if (q) {
            if (func_00208B38(p->p10, q)) {
                f.a = 0x12;
                f.p10 = q;
                f.f14 = func_00208BB0(p->p10);
            } else {
                f.a = 4;
                f.p10 = q;
            }
        } else {
            int n = p->n30;
            if (n < 11) {
                if (n >= 6) {
                    f.a = 2;
                    f.p10 = func_00156B98(&D_003DD490, 0, func_001A62E8(p->p10), p->p10);
                } else {
                    f.a = 1;
                }
            } else {
                f.a = 1;
            }
        }
    }
    func_00176750(p, &f);
}
