typedef int qword __attribute__((mode(TI)));
struct T { char pad[0x70]; qword pos; };
struct O { char pad[0x19A4]; void *p19A4; char pad2[0x19C0 - 0x19A8]; struct T *p19C0; };
extern qword func_001A62E8(void *p);
extern qword ConvertAloPos(int n, struct O *p, qword v);
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0, %1" : "=r"(y) : "r"(q)); return y; }
/** @sly2-annot addr=0x18ED98
 * Purpose: Midpoint of (hook child x19A4 pos, right-hand XF x19C0 pos) in Sly's local frame has y>0 (target
 *   behind) -> 1
 * Info: proposed name FJtSweepBack (T1); TU cane
 * Notes: names_v2 C proposed BindAlo__FP3ALO (0.638)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#cane-0x18e540-0x18f908
 * @end-sly2-annot */
int FJtSweepBack(struct O *p)
{
    union { qword q; float f[4]; } u;
    qword a, b, s, d;
    float t;
    int f;
    a = ConvertAloPos(0, p, func_001A62E8(p->p19A4));
    b = ConvertAloPos(0, p, p->p19C0->pos);
    t = 0.5f;
    __asm__("qmtc2.ni %1, %0" : "=j"(s) : "r"(t));
    u.q = s;
    __asm__("vmulax.xyzw ACC, %1, %2x\n\tvmaddx.xyzw %0, %3, %2x" : "=j"(d) : "j"(a), "j"(s), "j"(b));
    f = 1;
    if (GetY(d) > 0.0f) {} else f = 0;
    return f;
}
