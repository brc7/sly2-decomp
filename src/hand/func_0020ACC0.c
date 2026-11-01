typedef int qword __attribute__((mode(TI)));
typedef struct MATRIX3 { qword a[3]; } MATRIX3;
struct A { char pad[0x30]; qword pos; char pad2[0x60]; int nA0; };
struct B { char pad[0x90]; qword v; struct A *pa; };
struct D { char pad[0x20]; qword v; };
struct S { char pad[0xF08]; struct B *pb; struct D *pd; int nF10; char pad2[0xC]; qword vF20; };
qword func_0022FC48(int a, int b, qword v);
void func_0011BA08(MATRIX3 *pm, qword a, qword b);
qword func_0011BFF0(MATRIX3 *pm);
/** @sly2-annot addr=0x20ACC0
 * Purpose: Aim rotation toward predicted point (0x22FC48) as Euler
 * Info: proposed name UpdateStepguardAim (T2); TU stepguard
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020ACC0(struct S *p)
{
    MATRIX3 m;
    qword v, d;
    v = func_0022FC48(p->nF10, p->pb->pa->nA0, p->vF20);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(v), "j"(p->pb->pa->pos));
    func_0011BA08(&m, p->pb->v, d);
    { struct D *pd = p->pd; pd->v = func_0011BFF0(&m); }
}
