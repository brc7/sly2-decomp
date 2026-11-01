struct X { char pad[0x54]; int n; };
struct S { char pad[0x64]; struct X *a; struct X *b; };
static inline int Max(int a, int b) { return a > b ? a : b; }
/** @sly2-annot addr=0x163430
 * Purpose: Job state from its tasks: start-task state if <2, else max(end-task state,1)
 * Info: proposed name NJobState (T2); TU game
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
int func_00163430(struct S *p)
{
    int n = p->a->n;
    return n < 2 ? n : Max(p->b->n, 1);
}
