struct A { int a[5]; };
struct B { int a[10]; };
struct C { int a[6]; };
void func_00169AC8(void *p, struct A *pa0, struct B *pb0, struct C *pc0, struct A *pa, struct B *pb, struct C *pc, int x, int y, int z, int w);
/** @sly2-annot addr=0x1699E8
 * Purpose: For each extra instance slot of a sub-glob, copies the base descriptor and reads the instance
 *   record (0x169AC8)
 * Info: proposed name LoadSubglobInstancesFromBrx (T2); TU glob
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/rendering.md#glob-0x167310-0x16f148
 * @end-sly2-annot */
void func_001699E8(void *p, struct A *aa, struct B *ab, struct C *ac, int c, int x, int y, int z, int w)
{
    int i;
    for (i = 0; i < c; i++) {
        struct A *pa = &aa[i + 1];
        struct B *pb = &ab[i + 1];
        struct C *pcw = ac ? ac + i + 1 : 0;
        func_00169AC8(p, aa, ab, ac, pa, pb, pcw, x, y, z, w);
    }
}
