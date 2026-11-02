typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
struct P { char pad[0xC]; int nC; float g10; int g14; };
struct Q { char pad[0x20]; qword v20; char pad2[4]; int n34; };
void EvaluateShapeS(int n, float g, int m, U *pv);
/** @sly2-annot addr=0x19EA00
 * Purpose: Curve tangent at s scaled by climb speed (+0x14) into out
 * Info: proposed name GetJtPipeVelocity (T2); TU pipe_jt
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8,
 *   docs/reference/functions/player.md#pipe_jt-0x19de10-0x19f0a8
 * @end-sly2-annot */
void func_0019EA00(struct P *p, struct Q *q)
{
    U v;
    U s;
    qword r;
    EvaluateShapeS(p->nC, p->g10, 0, &v);
    p = (struct P *)p->g14;
    __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(p));
    __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v.q), "j"(s.q));
    q->v20 = r;
    q->n34 = 0;
}
