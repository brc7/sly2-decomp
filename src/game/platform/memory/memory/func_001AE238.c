struct MSG { int cb; int nKind; };
void *func_001AE1D0(struct MSG *pm);
void CopyAb__FPvT0Ui(void *, void *, int);
/** @sly2-annot addr=0x1AE238
 * Purpose: PvAllocSwImpl then CopyAb(dst src cb)
 * Info: proposed name PvAllocSwCopyImpl (T1); TU memory
 * Confidence: high [T1 0.92] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/core-library.md#memory-0x1ae150-0x1ae7d0,
 *   docs/reference/functions/save-memcard.md#save-0x1d7b88-0x1d93d8
 * @end-sly2-annot */
void *func_001AE238(struct MSG *pm, void *pvSrc)
{
    struct MSG m = *pm;
    void *p = func_001AE1D0(&m);
    if (p) CopyAb__FPvT0Ui(p, pvSrc, pm->cb);
    return p;
}
