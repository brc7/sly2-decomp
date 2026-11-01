struct S { char pad[0x1CC]; int x; };
void func_001F36A8(struct S *p);
void func_00226370(struct S *p, int a);
/** @sly2-annot addr=0x225AA8
 * Purpose: PostSoLoad then pending state -> 0x226370(p,1)
 * Info: proposed name PostTurretLoad (T1); class TURRET slot15; TU turret
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550
 * @end-sly2-annot */
void func_00225AA8(struct S *p)
{
    func_001F36A8(p);
    if (p->x) { p->x = 0; func_00226370(p, 1); }
}
