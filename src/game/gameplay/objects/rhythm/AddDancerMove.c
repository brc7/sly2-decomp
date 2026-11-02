struct E { int a; int b; int pad8; int c; int d; int pad14; int z18; int e; int pad20; int z24; };
struct S { char pad[0x2C4]; unsigned int c; struct E a[8]; };
/** @sly2-annot addr=0x1524D8
 * Purpose: Append move entry {a,b,c,d,e} (max 8) at +0x2C4
 * Info: proposed name AddDancerMove (T1); TU rhythm
 * Confidence: mid (draft) [T1 0.50] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void AddDancerMove(struct S *p, int a, int b, int c, int d, int e)
{
    if (p->c < 8) {
        struct E *pe = &p->a[p->c++];
        pe->a = a;
        pe->b = b;
        pe->c = c;
        pe->d = d;
        pe->e = e;
        pe->z18 = 0;
        pe->z24 = 0;
    }
}
