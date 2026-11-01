typedef int qword __attribute__((mode(TI)));
struct Q { char pad[0x1C]; float g; };
struct P { char pad[0x180]; float f180; int pad2[3]; qword q190; char pad3[0x1030 - 0x1A0]; struct Q *pq; char pad4[0x1840 - 0x1034]; int n; };
qword func_00204E38(struct P *p, struct Q *pq, float g);
/** @sly2-annot addr=0x1B90B8
 * Purpose: State 12 substates 1/3: velocity from animation root motion
 * Info: proposed name UpdateMurrayPowerVelocity (T2); TU murray
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B90B8(struct P *p)
{
    if (p->n == 1 || p->n == 3) {
        qword v = func_00204E38(p, p->pq, p->pq->g);
        qword r;
        union { qword q; float f; } s;
        __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(-1.0f));
        __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(r) : "j"(v), "j"(s.q));
        p->q190 = r;
        p->f180 = 1.0f;
    }
}
