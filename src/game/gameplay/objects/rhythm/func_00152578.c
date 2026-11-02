struct R { int id; int res; int x; };
struct E { int a; struct R ar[3]; };
struct S { char pad[0x2C4]; int c; struct E a[8]; };
int func_00152520(struct S *p, int id);
/** @sly2-annot addr=0x152578
 * Purpose: Resolve every move entry's 3 oids to objects via 0x152520
 * Info: proposed name BindDancerMoves (T2); TU rhythm
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void func_00152578(struct S *p)
{
    int i, j;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->a[i];
        for (j = 0; j < 3; j++) {
            if (pe->ar[j].id != -1)
                pe->ar[j].res = func_00152520(p, pe->ar[j].id);
        }
    }
}
