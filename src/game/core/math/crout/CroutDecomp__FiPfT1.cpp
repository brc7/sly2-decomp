static inline float GAbs(float g) { return __builtin_fabsf(g); }
/** @sly2-annot addr=0x1195E0
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/core-library.md#6-source-files,
 *   docs/reference/functions/core-library.md#crout-0x1195e0-0x119bc8,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
void CroutDecomp(int n, float *a, float *lu)
{
    int i, j, k;
    float sum;
    if (n == 0)
        return;
    for (j = 0; j < n; j++)
        lu[j] = a[j];
    if (GAbs(lu[0]) < 1e-8f) {
        for (i = 0; i < n; i++)
            lu[i * n] = 0.0f;
    } else {
        for (i = 1; i < n; i++)
            lu[i * n] = a[i * n] / lu[0];
    }
    for (i = 1; i < n - 1; i++) {
        for (j = i; j < n; j++) {
            sum = 0.0f;
            for (k = 0; k < i; k++)
                sum += lu[k * n + j] * lu[i * n + k];
            lu[i * n + j] = a[i * n + j] - sum;
        }
        if (GAbs(lu[i * n + i]) < 1e-8f) {
            for (k = i; k < n; k++)
                lu[k * n + i] = 0.0f;
        } else {
            for (k = i + 1; k < n; k++) {
                sum = 0.0f;
                for (j = 0; j < i; j++)
                    sum += lu[j * n + i] * lu[k * n + j];
                lu[k * n + i] = (a[k * n + i] - sum) / lu[i * n + i];
            }
        }
    }
    sum = 0.0f;
    for (k = 0; k < n - 1; k++)
        sum += lu[k * n + n - 1] * lu[(n - 1) * n + k];
    lu[(n - 1) * n + n - 1] = a[(n - 1) * n + n - 1] - sum;
    if (GAbs(lu[(n - 1) * n + n - 1]) < 1e-8f)
        lu[(n - 1) * n + n - 1] = 0.0f;
}
