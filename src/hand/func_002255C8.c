typedef int qword __attribute__((mode(TI)));
struct T { char pad[0x80]; qword v; };
struct O { char pad[0xE8]; struct T *t; };
struct Q { int a; struct O *o; };
struct S { char pad[0x1B0]; void *x; };
extern qword D_002A2DD0;
extern char D_002C43D0[], D_002C43E0[];
void func_001FAC30(struct O *o, qword v);
qword func_001A62E8(void *p);
void func_001F5E10(struct O *o, qword v, void *a, qword b, void *c);
/** @sly2-annot addr=0x2255C8
 * Purpose: negated vector -> 0x1FAC30
 * Info: class CID102 slot71; TU tu_223F70
 * Notes: descriptive only: Cid102 impulse helper | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r8_21BB88
 * @end-sly2-annot */
void func_002255C8(struct S *p, struct Q *q)
{
    struct O *o = q->o;
    union { qword q; float f[4]; } u;
    qword r, v;

    float m = -1.0f;
    qword s;
    __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(m));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(o->t->v), "j"(s));
    u.q = s;
    func_001FAC30(o, r);
    if (p->x) v = func_001A62E8(p->x); else v = func_001A62E8(p);
    func_001F5E10(o, v, D_002C43D0, D_002A2DD0, D_002C43E0);
}
