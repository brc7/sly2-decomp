void func_001EEF68(void *p);
/** @sly2-annot addr=0x1461C8
 * Purpose: BHV196 slot 7 (same shape as BHV195)
 * Info: proposed name InitBhv196 (T2); class BHV196 slot7; TU tu_144B28
 * Notes: superseded: OnBhv196Push (r2b_rest) | BHV vtable slot 7 role per r9 (CONFLICTS #15); was
 *   OnBhv196Push
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_001461C8(char *p)
{
    func_001EEF68(p);
    *(int *)(p + 0x30) = -1;
}
