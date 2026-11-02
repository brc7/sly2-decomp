struct S { char pad[0x54]; int c; int *a; };
/** @sly2-annot addr=0x162600
 * Purpose: Splice: appends a game-event id to the job list at +0x54/+0x58 (max 32); HandleJobGameEvent fires
 *   event 0x1D when it arrives
 * Info: proposed name AddJobCompleteEvent (EXISTING); TU game
 * Notes: agent/verifier proposes AddJobCompleteEvent for this existing name (see existing_name_changes.csv) |
 *   RENAME proposed -> AddJobCompleteEvent (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.55] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r3b_rest+arbitration
 * See: docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void FUN_001e53f8(struct S *p, int n)
{
    if (p->c < 32) {
        p->a[p->c++] = n;
    }
}
