struct E { int pad[3]; int k; char pad2[0xB0 - 0x10]; };
struct P { char pad[0xE0]; int c; struct E *ae; };
void func_001F2A60(struct P *, struct E *);
/** @sly2-annot addr=0x1F6AC8
 * Purpose: ShowSoPart for parts whose id (+0xC) == k
 * Info: proposed name ShowSoPartsById (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_001F6AC8(struct P *p, int k)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i]; if (pe->k == k) func_001F2A60(p, pe);
    }
}
