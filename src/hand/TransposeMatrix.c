void TransposeMatrix(int cStride, int c, float *agSrc, float *agDst)
{
    int i, j;
    for (i = 0; i < c; i++) {
        for (j = 0; j < c; j++) {
            agDst[j * cStride + i] = agSrc[i * cStride + j];
        }
    }
}
