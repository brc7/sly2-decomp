typedef unsigned long ulong;
struct E { int pad[4]; ulong fl; char pad2[0xB0 - 0x18]; };
struct P { char pad[0xE0]; int c; struct E *ae; };
void RemoveSoPartFromTree(struct P *, struct E *);
/** @sly2-annot addr=0x1F2BA8
 * Purpose: RemoveSoPartFromTree for every visible SOPART
 * Info: proposed name RemoveSoParts (T1); TU so
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void RemoveSoParts(struct P *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (!(pe->fl & (1L << 50))) RemoveSoPartFromTree(p, pe);
    }
}
