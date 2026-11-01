struct X;
struct Y { char pad[0x78]; int f78; };
struct X { char pad[0x54]; int n; char pad2[0x7C - 0x58]; struct Y *py; char pad3[0x94 - 0x80]; int c; struct X **apx; char pad4[0xF0 - 0x9C]; int fF0; };
/** @sly2-annot addr=0x164CE8
 * Purpose: Collects successors whose state must be raised to n (state 2 becomes 3 across jobs)
 * Info: proposed name CollectTaskCascade (T1); TU game
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void CollectTaskCascade(struct X *p, int n, int *pc, struct X **apx)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct X *px = p->apx[i];
        int nChild = n;
        if (n == 2) {
            struct Y *py = px->py;
            if (!py || (py != p->py && !py->f78))
                nChild = 3;
        }
        CollectTaskCascade(px, nChild, pc, apx);
    }
    if (p->fF0 == -1 && p->n < n) {
        apx[(*pc)++] = p;
        p->fF0 = n;
    }
}
