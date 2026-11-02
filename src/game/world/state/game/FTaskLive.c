struct S { char pad[0x50]; int f50; char pad2[0xF4 - 0x54]; int n; };
extern char D_003DD490[];
int func_0020E040(void *p);
/** @sly2-annot addr=0x1637E8
 * Purpose: Task counterpart of FJobLive (mode at +0xF4)
 * Info: proposed name FTaskLive (T1); TU game
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
int FTaskLive(struct S *p)
{
    if (p->f50 == 0 && p->n != 2 && p->n != 3)
        return func_0020E040(D_003DD490) == 0;
    return 0;
}
