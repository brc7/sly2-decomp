typedef int qword __attribute__((mode(TI)));
struct P { char pad[0x1420]; qword q1420; };
void SetMurrayState(struct P *p, int n);
qword func_001A62E8(void *p);
void SetStepRadTargetVec(struct P *p, qword q);
/** @sly2-annot addr=0x1B7A78
 * Purpose: Splice-callable: SetMurrayState(7) and override the throw goal with the given object's position,
 *   facing it
 * Info: proposed name ThrowMurrayAt (T2); TU murray
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void func_001B7A78(struct P *p, void *pv)
{
    qword a, b, d;
    SetMurrayState(p, 7);
    if (pv) {
        a = func_001A62E8(pv);
        p->q1420 = a;
        b = func_001A62E8(p);
        __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b));
        SetStepRadTargetVec(p, d);
    }
}
