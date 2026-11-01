struct E { int a, b, c, d, e, f; };
struct S { char pad[0x50]; int f50; char pad2[0x94 - 0x54]; int c; struct E *ae; };
/** @sly2-annot addr=0x162728
 * Purpose: Splice: appends a 24-byte {state, oidRoot, oidSm, oidGoal, 0, level} RSMG record to the job
 * Info: proposed name AddJobStateRsmg (T2); TU game
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00162728(struct S *p, int a, int b, int c, int d, int f)
{
    if (p->f50 == 0 && p->c < 32) {
        struct E *pe = &p->ae[p->c++];
        pe->f = f; pe->a = a; pe->b = b; pe->c = c; pe->d = d; pe->e = 0;
    }
}
