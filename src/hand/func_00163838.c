struct E { int a, b, c, d; };
struct S { char pad[0x50]; int f50; char pad2[0xA8 - 0x54]; int c; struct E *ae; };
int func_00162050(void *p);
/** @sly2-annot addr=0x163838
 * Purpose: Splice: appends a visibility record (state mask from a Splice list) to the task
 * Info: proposed name AddTaskStateLo (T2); TU game
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00163838(struct S *p, int a, void *pv, int b, int d)
{
    if (p->f50 == 0 && p->c < 32) {
        struct E *pe = &p->ae[p->c++];
        pe->a = a;
        pe->b = b;
        pe->c = func_00162050(pv);
        pe->d = d;
    }
}
