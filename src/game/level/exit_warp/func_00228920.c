extern char D_002DEB30[];
void func_00161F10(void *pq, void *a, int n, void *b, int c, int d);
/** @sly2-annot addr=0x228920
 * Purpose: 0x161F10(D_002DEB30, p+0x128, 4, p+0x124, c, d)
 * Info: TU exit_warp
 * Notes: descriptive only: Add to 4-slot list at 0x124 | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r8_21BB88
 * @end-sly2-annot */
void func_00228920(char *p, int c, int d)
{
    func_00161F10(D_002DEB30, p + 0x124 + 4, 4, p + 0x124, c, d);
}
