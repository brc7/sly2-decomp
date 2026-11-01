struct S { char pad[0x170]; unsigned int c; int a[7]; };
/** @sly2-annot addr=0x1DE3B0
 * Purpose: Append a cid to the no-trigger class list (+0x170, max 7)
 * Info: proposed name AddSensorNoTriggerClass (T1); TU sensor
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6b_rest+verdicts_B
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#sensor-0x1de2a0-0x1dfad8
 * @end-sly2-annot */
void AddSensorNoTriggerClass(struct S *p, int n) {
    if (p->c < 7) {
        p->a[p->c++] = n;
    }
}
