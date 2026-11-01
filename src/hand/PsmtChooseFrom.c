struct E { int n0; int k4; int k8; int fl; int pad[2]; };
struct P { char pad[0x58]; int c; struct E *ae; };
int PsmtChooseWeighted(struct P *, int, struct E **);
/** @sly2-annot addr=0x1F0CF0
 * Purpose: Collects all transitions out of a state under the mask, then weighted pick
 * Info: proposed name PsmtChooseFrom (T1); TU sm
 * Notes: names_v2 v1 proposed SendSmaMessage__FP3SMA5MSGIDPv (0.001)
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; static naming pass; sources:
 *   semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
int PsmtChooseFrom(struct P *p, int mask, int k)
{
    struct E *ape[64];
    int n = 0;
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if ((pe->fl & mask) && (pe->k4 == k || ((pe->fl & 1) && pe->k8 == k)))
            ape[n++] = pe;
    }
    if (n == 0) return 0;
    return PsmtChooseWeighted(p, n, ape);
}
