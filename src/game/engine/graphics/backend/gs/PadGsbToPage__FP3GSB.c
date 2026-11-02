struct B { int n0; int n4; struct B *next; };
struct S { int n0; int n4; int n8; struct B *ap[8]; int c; struct B ab[1]; };
int IgsbBucketFromCgs__Fi(int n);
/** @sly2-annot addr=0x1704B8
 * Purpose: Rounds igsMac up to the next 0x80-unit boundary and records the skipped gap as a free run in the
 *   GSB bucket lists
 * Info: proposed name PadGsbToPage (T1); TU gs
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
void PadGsbToPage__FP3GSB(struct S *p) {
    struct B *pb;
    int n, i;
    if (!(p->n4 & 0x7F)) return;
    pb = &p->ab[p->c++];
    pb->n0 = p->n4;
    n = (p->n4 + 0x7F) & ~0x7F;
    p->n4 = n;
    pb->n4 = n;
    i = IgsbBucketFromCgs__Fi(n - pb->n0);
    pb->next = p->ap[i];
    p->ap[i] = pb;
}
