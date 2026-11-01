/** @sly2-annot addr=0x11C8E8
 * Purpose: Generic c x c transpose with stride
 * Info: proposed name TransposeMatrix (T1); TU mat
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
void func_0011C8E8(int cStride, int c, float *agSrc, float *agDst)
{
    int i, j;
    for (i = 0; i < c; i++) {
        for (j = 0; j < c; j++) {
            agDst[j * cStride + i] = agSrc[i * cStride + j];
        }
    }
}
