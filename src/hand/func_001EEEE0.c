struct L { int x; int c; void *a[1]; };
int func_00132B10(void *p, void *key);
/** @sly2-annot addr=0x1EEEE0
 * Purpose: Return the first stacked behaviour (bottom first) derived from cid, else NULL
 * Info: proposed name PbhvFindBhvstk (T1); TU bhvstk
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850,
 *   docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void *func_001EEEE0(struct L *pl, void *key)
{
    int i;
    for (i = 0; i < pl->c; i++) {
        void *p = pl->a[i];
        if (func_00132B10(p, key)) return p;
    }
    return 0;
}
