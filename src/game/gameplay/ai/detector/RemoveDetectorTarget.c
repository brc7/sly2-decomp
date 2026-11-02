struct E { int id; int n; float t; int pad; };
struct S { char pad[0xFC]; int c; struct E a[1]; };
struct E *PdteFindDetectorTarget(struct S *p, int id);
void SetDetectorTargetState(struct S *p, struct E *pe, int n);
void memmove(void *dst, void *src, int cb);
/** @sly2-annot addr=0x153F80
 * Purpose: Removes a target from the detector list (state forced to 0 first, array compacted)
 * Info: proposed name RemoveDetectorTarget (T1); TU detector
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/guards-ai.md#detector-0x153d58-0x154a60
 * @end-sly2-annot */
void RemoveDetectorTarget(struct S *p, int id)
{
    struct E *pe = PdteFindDetectorTarget(p, id);
    if (pe) {
        SetDetectorTargetState(p, pe, 0);
        p->c--;
        memmove(pe, pe + 1, (&p->a[p->c] - pe) * sizeof(struct E));
    }
}
