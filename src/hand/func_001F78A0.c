/** @sly2-annot addr=0x1F78A0
 * Purpose: Index of lowest set bit among 26 zap kinds
 * Info: proposed name IzpkFromGrfzpk (T2); TU so
 * Notes: names_v2 C proposed LoadMatrixFromPosRot__FP6VECTORP7MATRIX3P7MATRIX4 (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F78A0(int x)
{
    int i; int m;
    for (i = 0, m = 1; i < 26; i++, m <<= 1) if (x & m) break;
    return i;
}
