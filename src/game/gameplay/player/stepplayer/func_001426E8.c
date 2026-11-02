typedef int qword __attribute__((mode(TI)));
typedef struct { qword r[3]; } M3;
struct SO { char pad[0x40]; M3 mat; };
struct X60 { char pad[0x40]; M3 mat; };
struct P { char pad[0x60]; struct X60 *p60; char pad2[0x10F4 - 0x64]; void *p10F4; char pad3[0x1150 - 0x10F8]; M3 mat; };
struct SO *PxfFindSo(struct P *p, int x, int n);
M3 InvertMatrix3Cofactor(M3 *pm);
qword func_0011BFF0(M3 *pm);
void *func_0012DC18(struct P *p, struct SO *pso, int a, int b, int c, qword q);
void func_0012D020(void *pv);
static __inline__ qword MulV(M3 *pm, qword v)
{
    qword o;
    __asm__("vmulax.xyz $ACC, %1, %4x\n\tvmadday.xyz $ACC, %2, %4y\n\tvmaddz.xyz %0, %3, %4z"
            : "=j"(o) : "j"(pm->r[0]), "j"(pm->r[1]), "j"(pm->r[2]), "j"(v));
    return o;
}
static __inline__ M3 MulM3(M3 *pa, M3 *pb)
{
    M3 m;
    m.r[0] = MulV(pa, pb->r[0]);
    m.r[1] = MulV(pa, pb->r[1]);
    m.r[2] = MulV(pa, pb->r[2]);
    return m;
}
/** @sly2-annot addr=0x1426E8
 * Purpose: Find child SO x (0x1F4C58 kind 6), compute relative matrix, create an actor/ACT (0x12DC18) at
 *   +0x10F4 and start it
 * Info: proposed name AttachStepplayerToStartMarker (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_001426E8(struct P *p, int x)
{
    struct SO *pso = PxfFindSo(p, x, 6);
    M3 m, a;
    if (pso) {
        a = InvertMatrix3Cofactor(&p->p60->mat);
        {
            M3 t;
            M3 *pb = &pso->mat;
            t.r[0] = m.r[0] = MulV(&a, pb->r[0]);
            t.r[1] = m.r[1] = MulV(&a, pb->r[1]);
            m.r[2] = t.r[2] = MulV(&a, pb->r[2]);
            p->mat = t;
        }
        p->p10F4 = func_0012DC18(p, pso, 3, 7, 2, func_0011BFF0(&p->mat));
        func_0012D020(p->p10F4);
    }
}
