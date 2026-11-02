typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
struct S { char pad[0x10C]; void *p10C; char pad1[0x12C - 0x110]; int x12C; char pad2[0x140 - 0x130]; float g140; char pad3[0x150 - 0x144]; qword v150; float g160; };
struct P { char pad[0x10]; struct S *ps; int x14; };
void func_00220DD8(int a, int b, qword *pq, int c);
void FindShapeClosestPointFromS(void *p, qword q, float g, int a, int b, int c, int d, float *pg);
qword ConvertAloPos(int a, struct S *ps, qword q);
float atan2f(float y, float x);
/** @sly2-annot addr=0x221CE8
 * Purpose: positions tn target via FillCrvcCache or atan2 heading
 * Info: proposed name UpdateCptnTarget (T2); TU tn
 * Notes: agent qualifier: tn follow target heading
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void func_00221CE8(struct P *p, int b)
{
    struct S *ps = p->ps;
    qword q;
    if (!ps) return;
    if (!ps->p10C && !ps->x12C) return;
    func_00220DD8(p->x14, b, &q, 0);
    if (ps->p10C) {
        FindShapeClosestPointFromS(ps->p10C, q, ps->g140, 0, 0, 0, 0, &ps->g140);
    } else {
        qword v = ConvertAloPos(0, ps, q), d;
        float g;
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(v), "j"(ps->v150));
        g = atan2f(GetY(d), GetX(d));
        if (g < 0.0f) g += 6.2831855f;
        ps->g140 = g * ps->g160;
    }
}
