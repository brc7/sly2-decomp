struct E { int n0; int n4; int n8; int nC; int n10; int n14; };
struct P { char pad[0x58]; int n58; struct E *ae; };
int func_001F0498(struct P *, int, int, int);
int func_001F0AD0(struct P *, int, struct E **);
/** @sly2-annot addr=0x1F0B80
 * Purpose: Collects transitions from cur (or reversible, flag 1) that cut the distance to the goal by one
 *   hop, then weighted pick
 * Info: proposed name PsmtChooseTowardGoal (T1); TU sm
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
int func_001F0B80(struct P *p, int mask, int a, int b)
{
    struct E *ap[64];
    int c;
    int i;
    int n;
    if (a == b) n = 1;
    else n = func_001F0498(p, mask, a, b);
    c = 0;
    for (i = 0; i < p->n58; i++) {
        struct E *pe = &p->ae[i];
        if (!(pe->nC & mask)) continue;
        if (pe->n4 == a && func_001F0498(p, mask, pe->n8, b) == n - 1) {
            ap[c++] = pe;
            continue;
        }
        if ((pe->nC & 1) && pe->n8 == a && func_001F0498(p, mask, pe->n4, b) == n - 1)
            ap[c++] = pe;
    }
    if (c == 0) return 0;
    return func_001F0AD0(p, c, ap);
}
