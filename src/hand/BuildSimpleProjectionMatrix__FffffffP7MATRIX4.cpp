struct MATRIX4 { float mat[4][4]; } __attribute__((aligned(16)));
extern "C" void *memset(void *pv, int b, unsigned int cb);

/** @sly2-annot addr=0x11C448
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
void BuildSimpleProjectionMatrix(float rx, float ry, float dxOffset, float dyOffset, float sFar, float sNear, MATRIX4 *pmat)
{
    memset(pmat, 0, sizeof(MATRIX4));
    float zScale = (sFar + sNear) / (sFar - sNear);
    float zTrans = (sFar * (1.0f - zScale));
    pmat->mat[0][0] = rx;
    pmat->mat[2][3] = 1.0f;
    pmat->mat[2][0] = dxOffset;
    pmat->mat[1][1] = ry;
    pmat->mat[2][1] = dyOffset;
    pmat->mat[2][2] = zScale;
    pmat->mat[3][2] = zTrans;
}
