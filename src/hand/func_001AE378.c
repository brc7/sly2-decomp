struct MSG { int cb; int nKind; };
void *func_001AE318(struct MSG *pm);
void func_002333B4(void *p, int c, int n);
/** @sly2-annot addr=0x1AE378
 * Purpose: PvAllocStackImpl then memset 0
 * Info: proposed name PvAllocStackClearImpl (T1); TU memory
 * Notes: names_v2 C proposed PvAllocSwCopyImpl__FiPv (0.438)
 * Confidence: high [T1 0.92] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/core-library.md#memory-0x1ae150-0x1ae7d0
 * @end-sly2-annot */
void *func_001AE378(struct MSG *pm)
{
    struct MSG m = *pm;
    void *p = func_001AE318(&m);
    if (p) func_002333B4(p, 0, pm->cb);
    return p;
}
