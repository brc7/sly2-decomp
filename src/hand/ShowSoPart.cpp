typedef unsigned long ulong;
struct A { char pad[0x28]; ulong lo : 60; ulong b60 : 1; ulong hi : 3; };
struct B { char pad[0x10]; ulong lo : 50; ulong b50 : 1; ulong hi : 13; };
extern "C" void AddSoPartToTree(A *, B *);
/** @sly2-annot addr=0x1F2A60
 * Purpose: If part hidden (fl bit50): clear it and, when SO in world, AddSoPartToTree
 * Info: proposed name ShowSoPart (T1); TU so
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6_1D5150+verdicts_tu_r4r6
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void ShowSoPart(A *pa, B *pb)
{
    if (pb->b50) { pb->b50 = 0;
    bool f = pa->b60; if (f) AddSoPartToTree(pa, pb); }
}
