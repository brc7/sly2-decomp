struct O { char pad[0x148]; long fl; };
struct A { int pad; struct O *po; };
struct M { int cb; int align; int pad[2]; };
void *func_001AE318(struct M *pm);
/** @sly2-annot addr=0x20FDC8
 * Purpose: Deduplicate the SOs referenced by an array (mark 0x40000000), output count + slot array
 * Info: proposed name UniqueSwSoList (T2); TU sw
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/splice-scripting.md#bif-0x11f440-0x1258f0,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020FDC8(int n, struct A **apa, int *pc, struct O ***pppo)
{
    struct M m;
    struct O **apo;
    int c = 0;
    int i;
    m.cb = n * 4;
    m.align = 0x10;
    apo = (struct O **)func_001AE318(&m);
    for (i = 0; i < n; i++) {
        struct O *po = apa[i]->po;
        if (!(po->fl & 0x40000000)) {
            po->fl |= 0x40000000;
            apo[c++] = po;
        }
    }
    for (i = 0; i < c; i++)
        apo[i]->fl &= ~0x40000000L;
    *pc = c;
    *pppo = apo;
}
