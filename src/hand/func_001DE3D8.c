struct S { char pad[0x150]; unsigned int c; int a[7]; };
/** @sly2-annot addr=0x1DE3D8
 * Purpose: Append an oid to the no-trigger object list (+0x150, max 7)
 * Info: proposed name AddSensorNoTriggerObject (T1); TU sensor
 * Notes: names_v2 B proposed AddSensorTriggerClass__FP6SENSOR3CID (0.872)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/gameplay-objects.md#sensor-0x1de2a0-0x1dfad8
 * @end-sly2-annot */
void func_001DE3D8(struct S *p, int n) {
    if (p->c < 7) {
        p->a[p->c++] = n;
    }
}
