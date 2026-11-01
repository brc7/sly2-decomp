struct E { int a, b, c, d; };
struct S { char pad[0x50]; int f50; char pad2[0x8C - 0x54]; int c; struct E *ae; };
int func_00161FC8(int i);
/** @sly2-annot addr=0x1626C0
 * Purpose: Splice: appends a {oidLo, oidContext, GrfFromState(state), level} visibility record to the job
 * Info: proposed name AddJobStateLo (T2); TU game
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_001626C0(struct S *p, int a, int i, int b, int d)
{
    if (p->f50 == 0 && p->c < 32) {
        struct E *pe = &p->ae[p->c++];
        pe->a = a;
        pe->b = b;
        pe->c = func_00161FC8(i);
        pe->d = d;
    }
}
