typedef int qword __attribute__((mode(TI)));
struct MAT4 { qword a[4]; };
struct Q { char pad[0xA0]; void *xA0; };
class LOB {
public:
    int a[3];
    virtual void V1();
    virtual void V2();
    virtual void V3();
    virtual void V4();
    virtual void V5();
    virtual void V6();
    virtual void V7();
    virtual void V8();
    virtual void V9();
    virtual void V10(void *a, void *m, int b);
    virtual void V11();
    virtual void V12();
    virtual void V13();
    virtual void V14();
    virtual void V15();
    virtual void V16();
    virtual void V17();
    virtual void V18();
    virtual void V19();
    virtual void V20();
    virtual void V21();
    virtual void V22();
    virtual void V23();
    virtual void V24();
    virtual void V25();
    virtual void V26();
    virtual void V27();
    virtual void V28();
    virtual void V29();
    virtual void V30();
    virtual MAT4 *V31();
    virtual void *V32(MAT4 *pm);
};
class LO : public LOB {
public:
    char p0[0x58 - 0x10];
    Q *x58;
    Q *x5C;
    char p1[0xE8 - 0x60];
    int xE8;
};
struct S { void *x0; char p0[0x34 - 4]; LO *x34; char p1[0x94 - 0x38]; int x94; int x98; int x9C; LO *xA0; };
extern "C" {
void func_0012E460(int *p);
LO *func_001CCA40(void *pw, LO *po);
void func_0011C168(MAT4 *pd, MAT4 *pa, MAT4 *pb);
void func_001A6BE8(LO *po, MAT4 *pm);
void func_001F5720(LO *po, int n);
void func_001F57C0(LO *po, int n);
qword func_0022CA18(void *a, void *b);
int func_0012DC18(LO *p, Q *q, int a, int b, int c, qword d);
void func_001F7C68(LO *po, float g);
void func_001CD098(S *p, LO *po);
}
MAT4 func_001A71B0(int n, LO *po, MAT4 *pm);
MAT4 func_0011C318(MAT4 m);
/** @sly2-annot addr=0x1CD098
 * Purpose: Attach held object to the hand: constraints (SetSoCnstrForce/Torque), XF from matrix, acts,
 *   FadeSoIn
 * Info: proposed name AttachJtHandsObject (T2); TU jthands
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#jthands-0x1cc158-0x1cd5f0
 * @end-sly2-annot */
void func_001CD098(S *p, LO *po)
{
    MAT4 buf;
    MAT4 m;
    if (po == p->xA0) return;
    func_0012E460(&p->x94);
    func_0012E460(&p->x98);
    func_0012E460(&p->x9C);
    void *v = p->x34->V32(&buf);
    po->V10(p->x0, v, 0);
    m = *p->x34->V31();
    LO *r = func_001CCA40(p->x0, po);
    if (r) {
        MAT4 u;
        MAT4 t = func_001A71B0(0, po, r->V31());
        u = t;
        MAT4 w = func_0011C318(u);
        func_0011C168(&t, &m, &w);
        m = t;
    }
    func_001A6BE8(po, &m);
    if (po->xE8) {
        func_001F5720(po, 1);
        func_001F57C0(po, 1);
    }
    Q *q = po->x58;
    p->x98 = func_0012DC18(po, q, 5, 0, 2, func_0022CA18(q, q));
    q = po->x5C;
    p->x9C = func_0012DC18(po, q, 5, 0, 2, func_0022CA18(q, q));
    func_001F7C68(po, 0.25f);
    p->xA0 = po;
}
