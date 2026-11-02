/** @sly2-annot addr=0x14D8C0
 * Purpose: nCut(+0x300) = max(nCut,1): request a camera cut (no smoothing) this frame
 * Info: proposed name CutCm (T1); TU cm
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/camera.md#5-key-functions, docs/reference/functions/camera.md#tu_12e600-0x12e600-0x12eb48,
 *   docs/reference/functions/camera.md#cm-0x14d3d8-0x14fac8
 * @end-sly2-annot */
void CutCm__FP2CM(char *p)
{
    int n = *(int *)(p + 0x300);
    int m = 1;
    if (m < n) m = n;
    *(int *)(p + 0x300) = m;
}
