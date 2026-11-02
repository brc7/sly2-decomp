struct LM { float gMin; float gMax; };
struct S { char pad[0xC]; int c; struct LM alm[1]; };
/** @sly2-annot addr=0x1EA3B0
 * Purpose: Return the {gMin,gMax} range containing g
 * Info: proposed name PrangeFind (T2); TU shape
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
struct LM *func_001EA3B0(struct S *p, float g)
{
    int i;
    struct LM *alm = p->alm;
    for (i = 0; i < p->c; i++) {
        struct LM *plm = &alm[i];
        if (plm->gMin <= g && g <= plm->gMax) return plm;
    }
    return 0;
}
