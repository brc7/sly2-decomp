typedef int qword __attribute__((mode(TI)));
union SCALAR { qword q; float f[4]; };
union FLOATWORD { float value; int bits; };
struct PROJECTION { qword axis; int mode; };
struct LO;
struct B { char pad[0xB0]; struct PROJECTION projection; union FLOATWORD mass; };
struct A { char pad[0x58]; struct LO *space; char pad2[0xE8-0x5C]; struct B *state; };
qword ApplyConstr(struct PROJECTION *,struct LO *,qword);
static inline qword Get(qword r){qword a; __asm__("qmfc2.ni %0,%1" : "=r"(a) : "j"(r)); return a;}
/** @sly2-annot addr=0x1FAA00
 * Purpose: Linear velocity change = impulse / mass, constrained by force constraint
 * Info: proposed name DvFromSoImpulse (T2); TU solve
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
qword func_001FAA00(struct A *input,qword v) {
    register struct A *p __asm__("$2")=input;
    register struct PROJECTION *projection __asm__("$4");
    union SCALAR scalar;
    register qword vv __asm__("vf3")=v; register qword t __asm__("vf2"); register qword r __asm__("vf1");
    register int massWord __asm__("$3")=p->state->mass.bits;
    __asm__ __volatile__("qmtc2.ni %1,%0" : "=j"(t) : "r"(massWord));
    projection=&p->state->projection;
    __asm__ __volatile__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q" : "=j"(r) : "j"(t),"j"(vv));
    scalar.q=t;
    return ApplyConstr(projection,p->space,Get(r));
}
