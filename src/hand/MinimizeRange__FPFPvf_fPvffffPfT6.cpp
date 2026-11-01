typedef float (*PFNGG)(void *, float);
struct GG { float g; float gResult; };

static inline float GClamp(float g, float lo, float hi)
{
    if (g < lo) return lo;
    if (g > hi) return hi;
    return g;
}
static inline float GMax(float a, float b) { return a < b ? b : a; }
static inline float GMin(float a, float b) { return b < a ? b : a; }

/** @sly2-annot addr=0x11D9E0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
void MinimizeRange(PFNGG pfn, void *pv, float g, float dg, float gMin, float gMax, float *pg, float *pgResult)
{
    GG agg[5];
    int i;

    g = GClamp(g, gMin, gMax);

    agg[0].g = GMax(gMin, g - dg);
    agg[0].gResult = pfn(pv, agg[0].g);
    agg[2].g = g;
    agg[2].gResult = pfn(pv, g);
    agg[4].g = GMin(gMax, g + dg);
    agg[4].gResult = pfn(pv, agg[4].g);

    for (;;) {
        if (agg[0].gResult < agg[2].gResult) {
            agg[4] = agg[2];
            agg[2] = agg[0];
            agg[0].g = GMax(gMin, agg[2].g + agg[2].g - agg[4].g);
            agg[0].gResult = pfn(pv, agg[0].g);
        } else if (agg[4].gResult < agg[2].gResult) {
            agg[0] = agg[2];
            agg[2] = agg[4];
            agg[4].g = GMin(gMax, agg[2].g + agg[2].g - agg[0].g);
            agg[4].gResult = pfn(pv, agg[4].g);
        } else {
            break;
        }
    }

    for (i = 0; i < 10; i++) {
        agg[1].g = (agg[0].g + agg[2].g) * 0.5f;
        agg[1].gResult = pfn(pv, agg[1].g);
        agg[3].g = (agg[2].g + agg[4].g) * 0.5f;
        agg[3].gResult = pfn(pv, agg[3].g);
        if (agg[1].gResult < agg[2].gResult) {
            if (agg[1].gResult < agg[3].gResult) {
                agg[4] = agg[2];
                agg[2] = agg[1];
            } else {
                agg[0] = agg[2];
                agg[2] = agg[3];
            }
        } else if (agg[2].gResult <= agg[3].gResult) {
            agg[0] = agg[1];
            agg[4] = agg[3];
        } else {
            agg[0] = agg[2];
            agg[2] = agg[3];
        }
    }

    if (pg)
        *pg = agg[2].g;
    if (pgResult)
        *pgResult = agg[2].gResult;
}
