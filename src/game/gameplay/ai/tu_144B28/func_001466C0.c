void InitBhv(void *p);
/** @sly2-annot addr=0x1466C0
 * Purpose: BHV197 slot 7
 * Info: proposed name InitBhv197 (T2); class BHV197 slot7; TU tu_144B28
 * Notes: superseded: OnBhv197Push (r2b_rest) | BHV vtable slot 7 role per r9 (CONFLICTS #15); was
 *   OnBhv197Push
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest+bhv-slot-canon(r9)
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void func_001466C0(char *p)
{
    InitBhv(p);
    *(int *)(p + 0x30) = -1;
}
