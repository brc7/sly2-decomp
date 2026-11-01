struct A { char pad[0xAC]; int c; int *a; int n; };
/** @sly2-annot addr=0x1F81E0
 * Purpose: +0xB4 = -(index of n in +0xB0 list + 3), or 0
 * Info: proposed name SetSoAnimIndex (T2); TU so
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F81E0(struct A *p, int n)
{
    int i;
    if (n == -1) {
        p->n = 0;
        return;
    }
    for (i = 0; i < p->c; i++) {
        if (n == p->a[i]) {
            p->n = -(i + 3);
            return;
        }
    }
    p->n = 0;
}
