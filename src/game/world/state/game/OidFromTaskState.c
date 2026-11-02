struct S { char pad[0x54]; int n; };
/** @sly2-annot addr=0x164098
 * Purpose: Maps task state 0..3 to oid 0x612..0x615 (the default SM goal per state), else -1
 * Info: proposed name OidFromTaskState (T1); TU game
 * Notes: superseded: OidTaskStateGoal (verdicts_followups) | verifier better_name (PLAUSIBLE row): state 0..3
 *   -> 0x612..0x615 else -1; only a data ref (0x2B8EFC). Mapping correct; 'Goal'/'default SM goal' is
 *   unverified.
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3b_rest+verdicts_followups
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
int OidFromTaskState(struct S *p)
{
    switch (p->n) {
    case 0: return 0x612;
    case 1: return 0x613;
    case 2: return 0x614;
    case 3: return 0x615;
    }
    return -1;
}
