struct O { char pad[0x10]; unsigned long fl; };
struct S { char p0[0x40]; int x40; char p1[0x140-0x44]; int n140; struct O *po; };
/** @sly2-annot addr=0x1D6310
 * Purpose: Unregister LO from world sphere tree: mark node entry free (fl|=0xF<<32), +0x140=-1, +0x144=0,
 *   clear grf 0x80
 * Info: proposed name RemoveLoFromSphereTree (T2); TU tu_1D57A8
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/animation.md#xf-0x22c818-0x231100,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001D6310(struct S *p) {
    if (p->po) {
        int v;
        p->po->fl |= 0xF00000000UL;
        p->n140 = -1;
        p->po = 0;
        v = p->x40;
        if ((v ^ 1) & 1) *(int *)v &= ~0x80;
        else p->x40 = v & ~0x80;
    }
}
