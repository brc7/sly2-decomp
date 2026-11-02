struct S { char pad[0x5C]; int c; int *a; };
/** @sly2-annot addr=0x162630
 * Purpose: Splice: appends a game-event id to the job list at +0x5C/+0x60 (max 32); HandleJobGameEvent calls
 *   FailJob when it arrives
 * Info: proposed name AddJobFailEvent (T2); TU game
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00162630(struct S *p, int n)
{
    if (p->c < 32) {
        p->a[p->c++] = n;
    }
}
