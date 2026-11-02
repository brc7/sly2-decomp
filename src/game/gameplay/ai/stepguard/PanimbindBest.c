struct E { char pad[0x10]; long id; char pad2[8]; };
struct S { char pad[0x4C0]; int c; char pad2[0xC]; struct E a[1]; };
long GrfFromSgf(int n);
int func_00209A28(long id, long mask);
/** @sly2-annot addr=0x209AD0
 * Purpose: Binding whose flag set is the largest subset of mask and contains its lowest flag
 * Info: proposed name PanimbindBest (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/engine/guards-ai.md#4-combat-ko-and-takedowns,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
struct E *PanimbindBest(struct S *p, long mask)
{
    int i;
    int k = 0;
    long m = 0;
    int best = 0;
    struct E *pbest = 0;
    struct E *pe;
    for (; k < 0x30; k++) {
        m = GrfFromSgf(k);
        if (mask & m)
            break;
    }
    for (i = 0; i < p->c; i++) {
        int r;
        pe = &p->a[i];
        r = func_00209A28(pe->id, mask);
        if (r && (pe->id & m) && best < r) {
            best = r;
            pbest = pe;
        }
    }
    return pbest;
}
