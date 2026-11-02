struct A { char pad[0x60]; int n; };
void func_002333B4(void *p, int c, int cb);
/** @sly2-annot addr=0x1F3FB0
 * Purpose: SO vt32: optional identity into M4 out, return +0x60
 * Info: proposed name SO_PxfLocal (T2); class SO slot32; TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
int func_001F3FB0(struct A *p, float *pm)
{
    if (pm) {
        func_002333B4(pm, 0, 0x40);
        pm[0] = pm[5] = pm[10] = pm[15] = 1.0f;
    }
    return p->n;
}
