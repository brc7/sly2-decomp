struct E { char pad[0xB0]; };
struct P { char pad[0xE0]; int c; struct E *ae; };
/** @sly2-annot addr=0x1F7E08
 * Purpose: Collect parts passing optional predicate into array, return count
 * Info: proposed name CsopartCollectSo (T2); TU so
 * Notes: names_v2 v1 proposed SetSoAngularVelocityVec__FP2SOP6VECTOR (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F7E08(struct P *p, int (*pfn)(void *, struct E *), void *pv, struct E **ape)
{
    int n = 0;
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (!pfn || pfn(pv, pe)) {
            ape[n++] = pe;
        }
    }
    return n;
}
