typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; } V;
extern float D_002C0ABC;
float func_001E66C8(void *a, float g);
void func_001E6F10(void *a, float t, V *pv, int n);
int func_001C0920(void *b, qword v, void *c);
/** @sly2-annot addr=0x1BCF18
 * Purpose: Search along shape for a point inside the pathzone
 * Info: proposed name FFindShapePointInPathzone (T2); TU tu_1BC050
 * Notes: names_v2 B proposed FMatchJtXmg__FP2JTP3XMGP6ACTADJ (0.856)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
int func_001BCF18(void *a, void *b, float g, float range, float *pf, void *c)
{
    float d, t;
    V v0, v1;
    for (d = 0.0f; d < range; d += D_002C0ABC) {
        t = func_001E66C8(a, g - d);
        func_001E6F10(a, t, &v0, 0);
        if (func_001C0920(b, v0.q, c)) {
            if (pf) *pf = t;
            return 1;
        }
        t = func_001E66C8(a, g + d);
        func_001E6F10(a, t, &v1, 0);
        if (func_001C0920(b, v1.q, c)) {
            if (pf) *pf = t;
            return 1;
        }
    }
    return 0;
}
