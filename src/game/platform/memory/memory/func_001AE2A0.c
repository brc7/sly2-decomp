struct MSG { int cb; int nKind; };
void *func_001AE1D0(struct MSG *pm);
void func_002333B4(void *p, int c, int n);
/** @sly2-annot addr=0x1AE2A0
 * Purpose: PvAllocSwImpl then memset 0
 * Info: proposed name PvAllocSwClearImpl (T1); TU memory
 * Notes: names_v2 v1 proposed PvAllocSwImpl__Fi (0.001)
 * Confidence: high [T1 0.93] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#bis-0x1177a8-0x118668,
 *   docs/reference/functions/core-library.md#memory-0x1ae150-0x1ae7d0
 * @end-sly2-annot */
void *func_001AE2A0(struct MSG *pm)
{
    struct MSG m = *pm;
    void *p = func_001AE1D0(&m);
    if (p) func_002333B4(p, 0, pm->cb);
    return p;
}
