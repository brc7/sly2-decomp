struct E { int id; int n; float t; int pad; };
struct S { char pad[0xFC]; int c; struct E a[1]; };
int KCalculateDetectorTarget(struct S *p, int id, int n);
void SetDetectorTargetState(struct S *p, struct E *pe, int n);
/** @sly2-annot addr=0x153DE0
 * Purpose: Re-evaluates every tracked target of a detector: new state = 0x1541D8(target, old state), applied
 *   via 0x154080
 * Info: proposed name UpdateDetectorTargets (T1); TU detector
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/guards-ai.md#detector-0x153d58-0x154a60
 * @end-sly2-annot */
void UpdateDetectorTargets(struct S *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->a[i];
        SetDetectorTargetState(p, pe, KCalculateDetectorTarget(p, pe->id, pe->n));
    }
}
