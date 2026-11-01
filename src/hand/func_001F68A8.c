typedef unsigned long ulong;
struct E { char pad[0x10]; ulong lo : 48; ulong b48 : 1; ulong hi : 15; char pad2[0xB0 - 0x18]; };
struct P { char pad[0xE0]; int c; struct E *ae; };
/** @sly2-annot addr=0x1F68A8
 * Purpose: Set bit48 of every SOPART flag word
 * Info: proposed name SetSoPartsFlag48 (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F68A8(struct P *p, int f)
{
    int i;
    for (i = 0; i < p->c; i++) p->ae[i].b48 = f;
}
