struct E { float f; int x; };
int func_001C14A8(struct E *a, int i, float g, void *p, int f);
/** @sly2-annot addr=0x1C15F8
 * Purpose: Advance min/max cursors of one axis to a new range
 * Info: proposed name SweepObstacleRange (T2); TU path
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void func_001C15F8(struct E *a, int *pi1, int *pi2, void *p, float g1, float g2)
{
    int i1 = *pi1;
    int i2 = *pi2;
    if (a[i1].f < g1) {
        i2 = func_001C14A8(a, i2, g2, p, 1);
        i1 = func_001C14A8(a, i1, g1, p, 0);
    } else {
        i1 = func_001C14A8(a, i1, g1, p, 0);
        i2 = func_001C14A8(a, i2, g2, p, 1);
    }
    *pi1 = i1;
    *pi2 = i2;
}
