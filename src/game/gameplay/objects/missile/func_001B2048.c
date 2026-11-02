struct S { char pad[0x24C]; unsigned int c; int a[4]; };
/** @sly2-annot addr=0x1B2048
 * Purpose: Append CID to MISSILE list +0x250 (max 4, count +0x24C)
 * Info: proposed name AddMissileSpecialClass (T2); TU missile
 * Notes: names_v2 C proposed AddSensorNoTriggerClass__FP6SENSOR3CID (0.638)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/gameplay-objects.md#missile-0x1b1db8-0x1b2820
 * @end-sly2-annot */
void func_001B2048(struct S *p, int x)
{
    if (p->c < 4) p->a[p->c++] = x;
}
