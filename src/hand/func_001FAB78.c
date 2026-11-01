typedef int qword __attribute__((mode(TI)));
struct O { char pad[0xA0]; int nA0; char pad3[0xF0 - 0xA4]; qword pos; };
struct P { char pad[0x58]; struct O *po58; struct O *po5C; };
qword func_001FAA00(struct P *, qword);
qword func_001FAA48(struct P *, qword, qword);
qword func_0022FCC8(int, int, qword);
static inline void Add(struct O *po, qword d)
{
    qword a = po->pos;
    qword r = func_0022FCC8(0, po->nA0, d);
    qword s;
    __asm__("vadd.xyzw %0, %1, %2" : "=j"(s) : "j"(a), "j"(r));
    po->pos = s;
}
/** @sly2-annot addr=0x1FAB78
 * Purpose: Apply impulse at a point: add DvFromSoImpulse to linear and DwFromSoImpulse to angular velocity
 * Info: proposed name ApplySoImpulse (T2); TU solve
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
void func_001FAB78(struct P *p, qword a, qword b)
{
    qword x = func_001FAA00(p, b);
    qword y = func_001FAA48(p, a, b);
    Add(p->po58, x);
    Add(p->po5C, y);
}
