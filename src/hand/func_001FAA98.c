typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
struct LO;
struct S { qword v; int k; };
struct E { char pad[0x30]; M3 m; char pad2[0x90 - 0x60]; struct S vector; };
struct B { char pad[0x40]; M3 m; char pad2[0xA0 - 0x70]; struct LO *plo; };
struct X { char pad[0x5C]; struct B *pb; char pad2[0xE8 - 0x60]; struct E *pe; };
M3 *InvertMatrix3Cofactor(M3 *pd, const M3 *ps);
qword ApplyConstr(const struct S *pv, const struct B *pb, qword v);
static inline qword MulMV(M3 *pm, qword v, qword r)
{
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z" : "+j"(r) : "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]), "j"(v));
    return r;
}
static inline void MulMVIn(M3 *pm,qword *pv)
{
 register qword row0 __asm__("vf5")=pm->r[0];
 register qword row1 __asm__("vf2")=pm->r[1];
 register qword row2 __asm__("vf1")=pm->r[2];
 __asm__("vmulax.xyz $ACC,%1,%0x\n\tvmadday.xyz $ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z"
 : "+j"(*pv):"j"(row0),"j"(row1),"j"(row2));
}
static inline qword MulMVRow(M3 *pm, qword *pv)
{
    register qword row __asm__("vf3")=pm->r[2];
    register qword row0 __asm__("vf2")=pm->r[0];
    register qword row1 __asm__("vf1")=pm->r[1];
    __asm__("vmulax.xyz $ACC, %1, %0x\n\tvmadday.xyz $ACC, %2, %0y\n\tvmaddz.xyz %0, %3, %0z" : "+j"(*pv) : "j"(row0), "j"(row1), "j"(row));
    return row;
}
/** @sly2-annot addr=0x1FAA98
 * Purpose: Transform torque into body frame, apply inertia tensor, back to world, constrain by torque
 *   constraint
 * Info: proposed name ApplySoInverseInertia (T2); TU solve
 * Notes: names_v2 v1 proposed FUN_001bc670 (0.000)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
qword func_001FAA98(struct X *p, qword v)
{
    struct B *pb = p->pb;
    M3 m;
    qword row;
    InvertMatrix3Cofactor(&m, &pb->m);
    MulMVIn(&m, &v);
    row=MulMVRow(&p->pe->m,&v);
    return ApplyConstr(&p->pe->vector, p->pb, MulMV(&pb->m, v, row));
}
