int func_001F78D8(void *p, void *q);
struct N { char pad[0x10]; struct N *next; };
/** @sly2-annot addr=0x1F7530
 * Purpose: First LO in chain whose zap response for grfzpk is nonzero
 * Info: proposed name PloFindZapResponder (T2); TU so
 * Notes: names_v2 v1 proposed UpdateSoXfWorldHierarchy__FP2SO (0.026)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct N *func_001F7530(struct N *p, void *q)
{
    for (; p; p = p->next) {
        if (func_001F78D8(p, q))
            return p;
    }
    return 0;
}
