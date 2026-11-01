struct S { char pad[0x50]; int f50; char pad2[0xF0 - 0x54]; int n; };
extern char D_003DD490[];
int func_0020E040(void *p);
/** @sly2-annot addr=0x162670
 * Purpose: True if the job is enabled (+0x50==0), its mode (+0xF0) is not 2/3, and no world transition is in
 *   progress
 * Info: proposed name FJobLive (T1); TU game
 * Notes: names_v2 C proposed SetCmLookAt (0.438)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
int FJobLive(struct S *p)
{
    if (p->f50 == 0 && p->n != 2 && p->n != 3)
        return func_0020E040(D_003DD490) == 0;
    return 0;
}
