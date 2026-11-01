// (Claude agent) fresh14: b operands read through a plain qword pointer (scalar MEM, so the load waits for the VUF temp store)
typedef int qword __attribute__((mode(TI)));
struct AL { int cb; int cbAlign; int pad[2]; AL(int c, int a) { cb = c; cbAlign = a; } };
struct SO { char pad[0xF0]; qword xF0; qword x100; };
struct E40 { int x0, x4; SO *pso; int pad[13]; };
struct B { qword v[2][2]; char pad[0x10]; E40 *ae; char pad1[0x60 - 0x54]; };
struct A { int x0, x4, x8; float g; char pad[0x28 - 0x10]; };
struct S { int x0; int c; A *aa; char pad[0x58 - 0xC]; int cso; SO **apso; B *ab; };
extern "C" {
void func_001AE478(void *p);
void func_001AE4A0(void *p, int n);
void *func_001AE318(const AL &al);
qword LimitVectorLength(qword v, float g);
void ApplyDzg(S *p, float g1, float g2);
}
struct VUF { qword q; VUF(const float &g) { __asm__("qmtc2.ni %1, %0" : "=j"(q) : "r"(*(const int *)&g)); } };
static inline qword AddScale(qword a, qword b, const VUF &t)
{
    __asm__ __volatile__("vadda.xyzw $ACC, %0, $vf0\n\tvmaddx.xyzw %0, %1, %2x" : "+j"(a) : "j"(b), "j"(t.q), "j"(t.q), "j"(b));  /* last two operands: allocation only */
    return a;
}
class FRAME { public: FRAME() { func_001AE478(this); } ~FRAME() { func_001AE4A0(this, 2); } };
/** @sly2-annot addr=0x158E20
 * Purpose: Applies the solved row impulses to each SO's linear/angular velocity (+0xF0/+0x100) and clamps
 *   both to maxima g1/g2
 * Info: proposed name ApplyDzg (T1); TU dzg
 * Notes: names_v2 C proposed SolveInequalities__FiPfN21 (0.638)
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/core-library.md#dzg-0x1585d8-0x159118
 * @end-sly2-annot */
void ApplyDzg(S *p, float g1, float g2)
{
    FRAME frame;
    int i;
    SO *pso;
    { register int n asm("$3") = p->cso;  /* register pin: cso in v1 like the target */
      func_001AE318(AL(n * 32, 0x10)); }
    for (i = 0; i < p->c; i++) {
        A *pa = &p->aa[i];
        if (pa->g == 0.0f) continue;
        B *pb = &p->ab[i];
        for (int j = 0; j < 2; j++) {
            E40 *pe = &pb->ae[j];
            pso = pe->pso;
            if (!pso) continue;
            qword *pv = pb->v[j];
            qword r = AddScale(pso->xF0, pv[0], VUF(pa->g));
            qword a1 = pso->x100;
            __asm__ __volatile__("sqc2 %1, %0" : "=m"(pso->xF0) : "j"(r));
            pso->x100 = AddScale(a1, pv[1], VUF(pa->g));
        }
    }
    for (int k = 0; k < p->cso; k++) {
        pso = p->apso[k];
        pso->xF0 = LimitVectorLength(pso->xF0, g1);
        pso->x100 = LimitVectorLength(pso->x100, g2);
    }
}
