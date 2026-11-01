struct E { int pad[3]; int k; char pad2[0xB0 - 0x10]; };
struct P { char pad[0xE0]; int c; struct E *ae; };
void func_001F2AD0(struct P *, struct E *);
/** @sly2-annot addr=0x1F6B48
 * Purpose: HideSoPart for parts whose id == k
 * Info: proposed name HideSoPartsById (T2); TU so
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6_1D5150
 * See: docs/engine/animation.md#open-questions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_001F6B48(struct P *p, int k)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i]; if (pe->k == k) func_001F2AD0(p, pe);
    }
}
