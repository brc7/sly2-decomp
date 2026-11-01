typedef unsigned long ulong;
struct E { int pad[4]; ulong fl; char pad2[0xB0 - 0x18]; };
struct P { char pad[0xE0]; int c; struct E *ae; };
void AddSoPartToTree(struct P *, struct E *);
/** @sly2-annot addr=0x1F2B20
 * Purpose: AddSoPartToTree for every visible SOPART (+0xE0 count / +0xE4 array, 0xB0 stride)
 * Info: proposed name AddSoParts (T1); TU so
 * Notes: names_v2 C proposed GetSoCpdefi__FP2SOfP6CPDEFI (0.310)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void AddSoParts(struct P *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (!(pe->fl & (1L << 50))) AddSoPartToTree(p, pe);
    }
}
