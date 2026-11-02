struct O;
struct S { char pad[0xA4]; int c; struct O *a[4]; int b[4]; int d[4]; };
void func_001E5280(struct S *p, struct O *object, int b, int d);
/** @sly2-annot addr=0x1E54E8
 * Purpose: RemoveJsgStepHold for every held step
 * Info: proposed name ClearJsgStepHolds (T1); TU relay
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001E54E8(struct S *p)
{
    while (p->c) {
        func_001E5280(p, p->a[p->c - 1], p->b[p->c - 1], p->d[p->c - 1]);
    }
}
