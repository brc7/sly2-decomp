typedef int qword __attribute__((mode(TI)));
qword func_001A62E8(void *p);
void SetStepRadTargetVec(void *p, qword v);
/** @sly2-annot addr=0x2046F0
 * Purpose: Turn toward another LO: SetStepRadTargetVec(PosLoWorld(lo) - PosLoWorld(this))
 * Info: proposed name FaceStepToward (T1); TU step
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/player.md#bentley-0x134648-0x1378d8,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void FaceStepToward(void *p, void *q)
{
    qword a, b, d;
    a = func_001A62E8(q);
    b = func_001A62E8(p);
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
    SetStepRadTargetVec(p, d);
}
